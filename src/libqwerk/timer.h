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


/**
 * @file timer.h
 * Interface for the CTimer class, which provides
 * microsecond-resolution functions for timing and measuring single or
 * repeating events, and implementing timeouts.
 */

#ifndef _TIMER_H
#define _TIMER_H

/// Clock rate for Timer4, which is used by the CTimer class
#define T4HZ			(983040UL)

/**
 * CTimer provides microsecond-resolution functions for timing and
 * measuring single or repeating events, and implementing timeouts. 
 *
 * All time intervals must be constrained to one hour or less.  Time
 * intervals greater than an hour will reported incorrectly.
 *
 * All functions are guaranteed to delay no less than the
 * specified interval, but may delay longer due to preimption.
 *
 * This class should not be instantiated.  To use it, simply call the
 * functions against the class itself.
 */

class CTimer
{
public:
    /**
     * tick_t values represent the state of the internal timer used by this
     * class to measure time.  They are only exposed in this API for
     * efficiency.  Tick values should be considered opaque and unitless.
     *
     * However, they are universal; the tick_t value returned by any function
     * in this class can be used as a 'start', 'end' or 'last' parameter to any
     * other function in this class.
     */
    typedef unsigned long tick_t;

    /**
     * Returns the current timer tick value for use as the
     * 'start', 'end', or 'last' parameter in a subsequent
     * call to one of the other CTimer functions.
     *
     * @return The current timer tick value.
     */
    static tick_t ticks(void);

    /**
     * This function spins until the specified number
     * of microseconds pass.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * Example of a 100 microsecond delay:
     * \code
     *  CTimer::usleep(100);
     * \endcode
     *
     * @param [in]    usec    The number of microseconds to sleep
     */
    static void usleep(unsigned long usec);

    /**
     * This function spins until the specified number
     * of milliseconds pass.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * Example of a 10 millisecond delay:
     * \code
     *  CTimer::msleep(10);
     * \endcode
     *
     * @param [in]    msec    The number of milliseconds to sleep
     */
    static void msleep(unsigned long msec);

    /**
     * This function spins until the specified number
     * of seconds pass.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * Example of a 1 second delay:
     * \code
     *  CTimer::sleep(1);
     * \endcode
     *
     * @param [in]    sec     The number of seconds to sleep
     */
    static void sleep(unsigned long sec);


    /**
     * This function spins until the specified number
     * of microseconds pass.
     *
     * The 'start' parameter allows you to specify the
     * tick_t value that will be used as the starting time.
     *
     * If the specified interval (relative to the start time)
     * has already passed, it will return immediately.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * @param [in]    usec     The number of microseconds to sleep
     * @param [in]    start    The tick value used for the start of the interval
     *
     * Example of a 100 microsecond delay:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  // do some work during the 100us interval
     *  CTimer::usleep(100, start);
     * \endcode
     */
    static void usleep(unsigned long usec, tick_t start);

    /**
     * This function spins until the specified number
     * of millioseconds pass.
     *
     * The 'start' parameter allows you to specify the
     * tick_t value that will be used as the starting time.
     *
     * If the specified interval (relative to the start time)
     * has already passed, it will return immediately.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * @param [in]    msec     The number of milliseconds to sleep
     * @param [in]    start    The tick value used for the start of the interval
     *
     * Example of a 10 millisecond delay:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  // do some work during the 10ms interval
     *  CTimer::msleep(10, start);
     * \endcode
     */
    static void msleep(unsigned long msec, tick_t start);

    /**
     * This function spins until the specified number
     * of seconds pass.
     *
     * The 'start' parameter allows you to specify the
     * tick_t value that will be used as the starting time.
     *
     * If the specified interval (relative to the start time)
     * has already passed, it will return immediately.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * @param [in]    sec      The number of seconds to sleep
     * @param [in]    start    The tick value used for the start of the interval
     *
     * Example of a 1 second delay:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  // do some work during the 1s interval
     *  CTimer::sleep(1, start);
     * \endcode
     */
    static void sleep(unsigned long sec, tick_t start);

    /**
     * This function simply until the specified number
     * of microseconds pass, and returns the ending time
     * for use as the starting time in subsequent calls
     * to umetro().  Use this for running repeating events
     * on a regular interval.
     *
     * If the specified interval (relative to the last time)
     * has already passed, it will return immediately.  The
     * tick value returned represents the calculated end of the
     * interval rather than the actual end.  This means that
     * a series of umetro calls should "catch up" to the desired
     * cadence as long as the caller doesn't habitually overrun
     * the interval time.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * @param [in]    usec    The number of microseconds to sleep
     * @param [in]    last    The tick value for the end of the previous interval
     * @return Returns the calculated end-of-interval tick_t value.
     *
     * Example of a 250 microsecond repeating event:
     * \code
     *  CTimer::tick_t last = CTimer::ticks();
     *  while (1) {
     *      // insert code to run once every 250us;
     *      CTimer::umetro(250, last);
     *  }
     * \endcode
     */
    static tick_t umetro(unsigned long usec, tick_t last);

    /**
     * This function spins until the specified number
     * of milliseconds pass, and returns the ending time
     * for use as the starting time in subsequent calls
     * to umetro().  Use this for running repeating events
     * on a regular interval.
     *
     * If the specified interval (relative to the last time)
     * has already passed, it will return immediately.  The
     * tick value returned represents the calculated end of the
     * interval rather than the actual end.  This means that
     * a series of umetro calls should "catch up" to the desired
     * cadence as long as the caller doesn't habitually overrun
     * the interval time.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * @param [in]    msec    The number of milliseconds to sleep
     * @param [in]    last    The tick value for the end of the previous interval
     * @return Returns the calculated end-of-interval tick_t value.
     *
     * Example of a 2Hz (1/4s on, 1/4s off) LED flasher:
     * \code
     *  CTimer::tick_t last = CTimer::ticks();
     *  while (<keep flashing>) {
     *      <toggle LED>;
     *      CTimer::mmetro(250, last);
     *  }
     * \endcode
     */
    static tick_t mmetro(unsigned long msec, tick_t last);

    /**
     * This function spins until the specified number
     * of seconds pass, and returns the ending time
     * for use as the starting time in subsequent calls
     * to umetro().  Use this for running repeating events
     * on a regular interval.
     *
     * If the specified interval (relative to the last time)
     * has already passed, it will return immediately.  The
     * tick value returned represents the calculated end of the
     * interval rather than the actual end.  This means that
     * a series of umetro calls should "catch up" to the desired
     * cadence as long as the caller doesn't habitually overrun
     * the interval time.
     *
     * This function is guaranteed to delay no less than the
     * specified interval, but may delay longer due to preimption.
     *
     * @param [in]    sec     The number of seconds to sleep
     * @param [in]    last    The tick value for the end of the previous interval
     * @return Returns the calculated end-of-interval tick_t value.
     *
     * Example of a 2 second repeating event:
     * \code
     *  CTimer::tick_t last = CTimer::ticks();
     *  while (1) {
     *      // insert code to run once every 2 seconds;
     *      CTimer::metro(2, last);
     *  }
     * \endcode
     */
    static tick_t metro(unsigned long sec, tick_t last);

    /**
     * This function implements a simple timeout solution.
     * This function checks the elapsed number of microseconds
     * and returns true if the timeout has expired, or false if not.
     *
     * @param [in]    usec     Duration of the timeout in microseconds
     * @param [in]    start    The tick value at the start of the timout
     * @return Returns true if timout has expired, false if not
     *
     * Example of a 500 microsecond timeout:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  while (<condition>) {
     *      if (utimeout(500,start)) break;
     *  }
     * \endcode
     */
    static bool utimeout(unsigned long usec, tick_t start);

    /**
     * This function implements a simple timeout solution.
     * This function checks the elapsed number of milliseconds
     * and returns true if the timeout has expired, or false if not.
     *
     * @param [in]    msec     Duration of the timeout in milliseconds
     * @param [in]    start    The tick value at the start of the timout
     * @return Returns true if timout has expired, false if not
     *
     * Example of a 42 millisecond timeout:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  while (<condition>) {
     *      if (mtimeout(42,start)) break;
     *  }
     * \endcode
     */
    static bool mtimeout(unsigned long msec, tick_t start);

    /**
     * This function implements a simple timeout solution.
     * This function checks the elapsed number of seconds
     * and returns true if the timeout has expired, or false if not.
     *
     * @param [in]    sec      Duration of the timeout in seconds
     * @param [in]    start    The tick value at the start of the timout
     * @return Returns true if timout has expired, false if not
     *
     * Example of a 5 second timeout:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  while (<condition>) {
     *      if (timeout(5,start)) break;
     *  }
     * \endcode
     */
    static bool timeout(unsigned long sec, tick_t start);

    /**
     * This function returns the number of microseconds
     * that have elapsed since the tick value specified
     * by 'start'.  This can be used to time events.
     *
     * @param [in]    start    The starting tick value
     * @return Returns the elapsed time in microseconds
     *
     * Example of microsecond-resolution event timer:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  while (<condition>) { ... }
     *  usec = CTimer::uelapsed(start);
     * \endcode
     */
    static unsigned long uelapsed(tick_t start);

    /**
     * This function returns the number of milliseconds
     * that have elapsed since the tick value specified
     * by 'start'.  This can be used to time events.
     *
     * @param [in]    start    The starting tick value
     * @return Returns the elapsed time in milliseconds
     *
     * Example of millisecond-resolution event timer:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  while (<condition>) { ... }
     *  msec = CTimer::melapsed(start);
     * \endcode
     */
    static unsigned long melapsed(tick_t start);

    /**
     * This function returns the number of seconds
     * that have elapsed since the tick value specified
     * by 'start'.  This can be used to time events.
     *
     * @param [in]    start    The starting tick value
     * @return Returns the elapsed time in seconds
     *
     * Example of second-resolution event timer:
     * \code
     *  CTimer::tick_t start = CTimer::ticks();
     *  while (<condition>) { ... }
     *  sec = CTimer::elapsed(start);
     * \endcode
     */
    static unsigned long elapsed(tick_t start);

    /**
     * This function returns the difference in microseconds
     * between the 'start' and 'end' tick values.
     * This can be used to time events, and is handy
     * for post-processing collected timestamp data.
     *
     * Note: This returns a signed value, and will report a
     * negative result if 'end' happened before 'start'.
     * The two events must be within 30min of each other or
     * the returned value will be incorrect.
     *
     * @param [in]    start    The starting tick value
     * @param [in]    end      The ending tick value
     * @return Returns (end-start) in microseconds
     *
     * Example of timstamp post-processing of two events:
     * \code
     *  while(<condition>) {
     *      timestamp[i++] = CTimer::ticks();
     *  }
     *  ...
     *  usec = CTimer::udelta(timestamp[n],timestamp[n+1]);
     * \endcode
     */
    static signed long udelta(tick_t start, tick_t end);

    /**
     * This function returns the difference in milliseconds
     * between the 'start' and 'end' tick values.
     * This can be used to time events, and is handy
     * for post-processing collected timestamp data.
     *
     * Note: This returns a signed value, and will report a
     * negative result if 'end' happened before 'start'.
     * The two events must be within 30min of each other or
     * the returned value will be incorrect.
     *
     * @param [in]    start    The starting tick value
     * @param [in]    end      The ending tick value
     * @return Returns (end-start) in milliseconds
     *
     * Example of timstamp post-processing of two events:
     * \code
     *  while(<condition>) {
     *      timestamp[i++] = CTimer::ticks();
     *  }
     *  ...
     *  msec = CTimer::mdelta(timestamp[n],timestamp[n+1]);
     * \endcode
     */
    static signed long mdelta(tick_t start, tick_t end);

    /**
     * This function returns the difference in seconds
     * between the 'start' and 'end' tick values.
     * This can be used to time events, and is handy
     * for post-processing collected timestamp data.
     *
     * Note: This returns a signed value, and will report a
     * negative result if 'end' happened before 'start'.
     * The two events must be within 30min of each other or
     * the returned value will be incorrect.
     *
     * @param [in]    start    The starting tick value
     * @param [in]    end      The ending tick value
     * @return Returns (end-start) in seconds
     *
     * Example of timstamp post-processing of two events:
     * \code
     *  while(<condition>) {
     *      timestamp[i++] = CTimer::ticks();
     *  }
     *  ...
     *  sec = CTimer::delta(timestamp[n],timestamp[n+1]);
     * \endcode
     */
    static signed long delta(tick_t start, tick_t end);
};

#endif // _TIMER_H
