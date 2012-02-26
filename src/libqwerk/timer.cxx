//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

/*
 * timer.cxx
 *
 * The CTimer class provides (nearly) microsecond-resolution busy
 * wait functions that can be used to provide reasonably precise
 * timing for single and repeating events, as well as timeouts.
 *
 * CPU Timer 4 is used for all timing functions.  The actual clock
 * frequency is 983.04KHz, making the time resolution approximately
 * 1.017us.  For efficiency, we only use the lower 32b of Timer 4,
 * resulting in an approximate overflow time of 72 min.  All intervals
 * should be kept under an hour.
 */

#include "timer.h"
#include "9302hw.h"     // m_timers

volatile unsigned int *t4 = C9302Hardware::GetPtr()->m_timers.Uint(0x60);

// Cleanly cast to a tick_t (a.k.a. unsigned long)
// This is over used below, but we count on unsigned modulo-math for
// timer wrap to work, so better to over-cast than under-cast.
#define T(t)			( (tick_t)(t) )

// Constants
#define T4HZ			(983040UL)

// Basic tick-evaluation functions
#define	T4DELTA(start,end)		T( T(end) - T(start) )
#define T4EXPIRED(ticks,start)	(bool)( T4DELTA(start,(*t4)) > T(ticks) )
#define T4SLEEP(ticks,start)	while (!T4EXPIRED(ticks,start)) { }

// Time-to-ticks conversions
#define USEC_TO_T4(usec)	T( (T(usec)<60UL) ? T(usec) : (T(usec) - (T(usec)>>6 + T(usec)>>10)) )
#define MSEC_TO_T4(msec)	T( T(msec) * T(T4HZ/1000UL) )
#define SEC_TO_T4(sec)      T( T(sec) * T4HZ )

// Ticks-to-time conversions
#define T4_TO_USEC(ticks)	T( T(ticks) + T(ticks)>>6 + T(ticks)>>10 + T(ticks)>>11 )
#define T4_TO_MSEC(ticks)	T( T(ticks) / T(T4HZ/1000UL) )
#define T4_TO_SEC(ticks)	T( T(ticks) / T4HZ )


/*
 * ticks() returns the current timer tick value.
 */
CTimer::tick_t CTimer::ticks(void)
{
	return(*t4);
}


/*
 * These three functions simply busy-wait for the specified
 * number of microseconds, milliseconds, or seconds.
 */
void CTimer::usleep(unsigned long usec)
{
	register tick_t start = *t4;
	register tick_t ticks = USEC_TO_T4(usec);
	T4SLEEP(ticks, start);
}

void CTimer::msleep(unsigned long msec)
{
	register tick_t start = *t4;
	register tick_t ticks = MSEC_TO_T4(msec);
	T4SLEEP(ticks, start);
}

void CTimer::sleep(unsigned long sec)
{
	register tick_t start = *t4;
	register tick_t ticks = SEC_TO_T4(sec);
	T4SLEEP(ticks, start);
}


/*
 * These three functions busy-wait for the specified
 * number of microseconds, milliseconds, or seconds.
 * The 'start' parameter specifies the tick value to
 * use as the starting time.
 */
void CTimer::usleep(unsigned long usec, tick_t start)
{
	register tick_t ticks = USEC_TO_T4(usec);
	T4SLEEP(ticks, start);
}

void CTimer::msleep(unsigned long msec, tick_t start)
{
	register tick_t ticks = MSEC_TO_T4(msec);
	T4SLEEP(ticks, start);
}

void CTimer::sleep(unsigned long sec, tick_t start)
{
	register tick_t ticks = SEC_TO_T4(sec);
	T4SLEEP(ticks, start);
}


/*
 * These three functions busy-wait for the specified
 * number of microseconds, milliseconds, or seconds.
 * The 'last' parameter specifies the tick value to use
 * as the starting time.  These functions return the
 * scheduled wake time for use as the 'last' parameter
 * to subsequent calls.
 */
CTimer::tick_t CTimer::umetro(unsigned long usec, tick_t last)
{
	register tick_t ticks = USEC_TO_T4(usec);
	T4SLEEP(ticks, last);
	return (ticks+last);
}

CTimer::tick_t CTimer::mmetro(unsigned long msec, tick_t last)
{
	register tick_t ticks = MSEC_TO_T4(msec);
	T4SLEEP(ticks, last);
	return (ticks+last);
}

CTimer::tick_t CTimer::metro(unsigned long sec, tick_t last)
{
	register tick_t ticks = SEC_TO_T4(sec);
	T4SLEEP(ticks, last);
	return (ticks+last);
}


/*
 * These three functions implement a simple timeout
 * solution.  Rather than waiting, they simply check
 * the elapsed time and return true if it has elapsed
 * and false if not.
 */
bool CTimer::utimeout(unsigned long usec, tick_t start)
{
	register tick_t ticks = USEC_TO_T4(usec);
	return T4EXPIRED(ticks, start);
}

bool CTimer::mtimeout(unsigned long msec, tick_t start)
{
	register tick_t ticks = MSEC_TO_T4(msec);
	return T4EXPIRED(ticks, start);
}

bool CTimer::timeout(unsigned long sec, tick_t start)
{
	register tick_t ticks = SEC_TO_T4(sec);
	return T4EXPIRED(ticks, start);
}


/*
 * These three functions return the time that has elapsed
 * since the tick value specified by 'start'.
 */
unsigned long CTimer::uelapsed(tick_t start)
{
	return T4_TO_USEC(T4DELTA(start,*t4));
}

unsigned long CTimer::melapsed(tick_t start)
{
	return T4_TO_MSEC(T4DELTA(start,*t4));
}

unsigned long CTimer::elapsed(tick_t start)
{
	return T4_TO_SEC(T4DELTA(start,*t4));
}


/*
 * These three functions return the time difference
 * between the 'start' and 'end' tick values.
 *
 * Note: These return signed values, and will report a
 * negative result if 'end' happened before 'start'.
 * The two events must be within 30min of each other or
 * the returned value will be inaccurate and unpredictable.
 */
signed long CTimer::udelta(tick_t start, tick_t end)
{
	return (signed long)(T4_TO_USEC(T4DELTA(start,end)));
}

signed long CTimer::mdelta(tick_t start, tick_t end)
{
	return (signed long)(T4_TO_MSEC(T4DELTA(start,end)));
}

signed long CTimer::delta(tick_t start, tick_t end)
{
	return (signed long)(T4_TO_SEC(T4DELTA(start,end)));
}
