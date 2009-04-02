/**
 * @file timeutil.h
 *
 * @brief Various utility macros for manipulating timeval and timespec structs
 *
 * Most functions will operate on nonnormalized inputs, but may return incorrect
 *   results if this causes integer overflow.
 *
 * All functions return normalized inputs, except when the seconds field
 *   overflows.
 *
 * Normalized is defined as having nonnegative fields and microsecond or
 *   nanosecond fields less than 10^6 or 10^9 respectively.
 *
 * @author Michael Dille (mdille3)
 * @date Revised 2/21/2007
 */

#ifndef _TIMEUTIL_H_
#define _TIMEUTIL_H_

#include <time.h>
#include <sys/time.h>

/**
 * @brief Compares two timeval structs
 *
 * @param tv1 Pointer to first timeval struct
 * @param tv2 Pointer to second timeval struct
 *
 * @return < 0 if tv1 is lower, 0 if equal, > 0 if tv1 lower
 */
#define TIMEVAL_CMP(tv1,tv2)						\
({									\
  int __ret;								\
									\
  if ((tv1)->tv_sec < (tv2)->tv_sec)					\
    __ret = -1;								\
  else if ((tv1)->tv_sec > (tv2)->tv_sec)				\
    __ret = 1;								\
  else {								\
    if ((tv1)->tv_usec < (tv2)->tv_usec)				\
      __ret = -1;							\
    else if ((tv1)->tv_usec > (tv2)->tv_usec)				\
      __ret = 1;							\
    else								\
      __ret = 0;							\
  }									\
  __ret;								\
})

/**
 * @brief Clears (zeros) a timeval structure
 *
 * @param tv Pointer to timval struct to clear
 */
#define TIMEVAL_CLEAR(tv)						\
({									\
  (tv)->tv_sec = 0;							\
  (tv)->tv_usec = 0;							\
})

/**
 * @brief Normalize a timeval structure
 *
 * Carries excess microsends into seconds field
 * If possible, borrows from second field to cancel negative microsecond field
 *
 * @param tv Pointer to timeval struct to normalize
 */
#define TIMEVAL_NORMALIZE(tv)						\
({									\
  (tv)->tv_sec += ((tv)->tv_usec / 1000000);				\
  (tv)->tv_usec %= 1000000;						\
  if ((tv)->tv_sec > 0 && (tv)->tv_usec < 0) {				\
    (tv)->tv_sec--;							\
    (tv)->tv_usec += 1000000;						\
  }									\
})

/**
 * @brief Adds two timeval structures
 *
 * Performs <tvdest> = <tv1> + <tv2>
 *
 * @param tvdest Pointer to destination timeval struct (MAY be tv1 or tv2)
 * @param tv1 Pointer to first timeval struct
 * @param tv2 Pointer to second timeval struct
 */
#define TIMEVAL_ADD(tvdest,tv1,tv2)					\
({									\
  (tvdest)->tv_sec = (tv1)->tv_sec+(tv2)->tv_sec;			\
  (tvdest)->tv_usec = (tv1)->tv_usec+(tv2)->tv_usec;			\
  TIMEVAL_NORMALIZE(tvdest);						\
})

/**
 * @brief Interpolates two timeval structures
 *
 * Performs <tvdest> = (1.0-t)*<tv1> + t*<tv2>
 *
 * @param tvdest Pointer to destination timeval struct (MAY be tv1 or tv2)
 * @param tv1 Pointer to first timeval struct
 * @param tv2 Pointer to second timeval struct
 * @param t Fraction [0,1] between tv1 and tv2 at which to interpolate
 */
#define TIMEVAL_INTERPOLATE(tvdest,tv1,tv2,t)				\
({									\
  (tvdest)->tv_sec = (time_t)((1.0-t)*(tv1)->tv_sec+t*(tv2)->tv_sec);	\
  (tvdest)->tv_usec = (suseconds_t)((1.0-t)*(tv1)->tv_usec+t*(tv2)->tv_usec);\
  TIMEVAL_NORMALIZE(tvdest);						\
})

/**
 * @brief Subtracts two timeval structures
 *
 * Performs <tvdest> = <tv1> - <tv2>
 *
 * @param tvdest Pointer to destination timeval struct (MAY be tv1 or tv2)
 * @param tv1 Pointer to first timeval struct
 * @param tv2 Pointer to second timeval struct
 */
#define TIMEVAL_SUB(tvdest,tv1,tv2)					\
({									\
  (tvdest)->tv_sec = (tv1)->tv_sec-(tv2)->tv_sec;			\
  (tvdest)->tv_usec = (tv1)->tv_usec-(tv2)->tv_usec;			\
  TIMEVAL_NORMALIZE(tvdest);						\
})

/**
 * @brief Returns difference in seconds between two timeval structures
 *
 * Performs <tv1> - <tv2>
 *
 * @param tv1 Pointer to first timeval struct
 * @param tv2 Pointer to second timeval struct
 */
#define TIMEVAL_DIFF_SEC(tv1,tv2)					\
({									\
  ((tv1)->tv_sec-(tv2)->tv_sec) +					\
    0.000001*((tv1)->tv_usec-(tv2)->tv_usec);				\
})

/**
 * @brief Returns difference in milliseconds between two timeval structures
 *
 * Performs <tv1> - <tv2>
 *
 * @param tv1 Pointer to first timeval struct
 * @param tv2 Pointer to second timeval struct
 */
#define TIMEVAL_DIFF_MSEC(tv1,tv2)					\
({									\
  1000.0*((tv1)->tv_sec-(tv2)->tv_sec) +				\
    0.001*((tv1)->tv_usec-(tv2)->tv_usec);				\
})

/**
 * @brief Returns difference in microseconds between two timeval structures
 *
 * Performs <tv1> - <tv2>
 *
 * @param tv1 Pointer to first timeval struct
 * @param tv2 Pointer to second timeval struct
 */
#define TIMEVAL_DIFF_USEC(tv1,tv2)					\
({									\
  1000000.0*((tv1)->tv_sec-(tv2)->tv_sec) +				\
   ((tv1)->tv_usec-(tv2)->tv_usec);					\
})

/**
 * @brief Adds a specified number of microseconds to a timeval struct
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of microseconds to add
 */
#define TIMEVAL_ADD_USEC(tv,t)						\
({									\
  (tv)->tv_sec += (t)/1000000L;						\
  (tv)->tv_usec += (t)%1000000L;					\
  TIMEVAL_NORMALIZE(tv);						\
})

/**
 * @brief Adds a specified number of milliseconds to a timeval struct
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of milliseconds to add
 */
#define TIMEVAL_ADD_MSEC(tv,t)						\
({									\
  (tv)->tv_sec += (t)/1000L;						\
  (tv)->tv_usec += 1000L*((t)%1000L);				\
  TIMEVAL_NORMALIZE(tv);						\
})

/**
 * @brief Adds a specified number of seconds to a timeval struct
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of seconds to add
 */
#define TIMEVAL_ADD_SEC(tv,t)						\
({									\
  (tv)->tv_sec += (t);							\
})

/**
 * @brief Sets a timeval struct to the current time
 *
 * @param tv Pointer to timeval struct
 */
#define TIMEVAL_SET(tv)							\
({									\
  gettimeofday((tv), NULL);						\
})

/**
 * @brief Sets a timeval struct to the current time + n microseconds
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of microseconds to add
 */
#define TIMEVAL_SET_ADD_USEC(tv,t)					\
({									\
  gettimeofday((tv), NULL);						\
  TIMEVAL_ADD_USEC(tv,t);						\
})

/**
 * @brief Sets a timeval struct to the current time + n milliseconds
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of milliseconds to add
 */
#define TIMEVAL_SET_ADD_MSEC(tv,t)					\
({									\
  gettimeofday((tv), NULL);						\
  TIMEVAL_ADD_MSEC(tv,t);						\
})

/**
 * @brief Sets a timeval struct to the current time + n seconds
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of seconds to add
 */
#define TIMEVAL_SET_ADD_SEC(tv,t)					\
({									\
  gettimeofday((tv), NULL);						\
  TIMEVAL_ADD_SEC(tv,t);						\
})

/**
 * @brief Sets a timeval struct to contain a certain number of microseconds
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of microseconds to place in timeval
 */
#define TIMEVAL_SET_USEC(tv,t)						\
({									\
  (tv)->tv_sec = (t)/1000000L;						\
  (tv)->tv_usec = (t)%1000000L;						\
  TIMEVAL_NORMALIZE(tv);						\
})

/**
 * @brief Sets a timeval struct to contain a certain number of milliseconds
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of milliseconds to place in timeval
 */
#define TIMEVAL_SET_MSEC(tv,t)						\
({									\
  (tv)->tv_sec = (t)/1000L;						\
  (tv)->tv_usec = 1000L*((t)%1000L);					\
  TIMEVAL_NORMALIZE(tv);						\
})

/**
 * @brief Sets a timeval struct to contain a certain number of seconds
 *
 * The result is normalized before returning
 *
 * @param tv Pointer to timeval struct
 * @param t Number of seconds to place in timeval
 */
#define TIMEVAL_SET_SEC(tv,t)						\
({									\
  (tv)->tv_sec = (t);							\
  (tv)->tv_usec = 0;							\
})

/**
 * @brief Converts a timeval struct to a timespec struct
 *
 * @param tv Pointer to input timeval struct
 * @param tv Pointer to output timespec struct
 */
#ifndef TIMEVAL_TO_TIMESPEC
#define TIMEVAL_TO_TIMESPEC(tv,ts)					\
({									\
  (ts)->tv_sec = (tv)->tv_sec;						\
  (ts)->tv_nsec = 1000L*(tv)->tv_usec;					\
})
#endif

/**
 * @brief Converts a timespec struct to a timeval struct
 *
 * The extra precision captured by timespec structs is naturally lost
 *
 * @param tv Pointer to input timespec struct
 * @param tv Pointer to output timeval struct
 */
#ifndef TIMESPEC_TO_TIMEVAL
#define TIMESPEC_TO_TIMEVAL(tv,ts)					\
({									\
  (tv)->tv_sec = (ts)->tv_sec;						\
  (tv)->tv_usec = (ts)->tv_nsec/1000L;					\
})
#endif

/**
 * @brief Compares two timespec structs
 *
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 *
 * @return < 0 if ts1 is lower, 0 if equal, > 0 if ts1 lower
 */
#define TIMESPEC_CMP(ts1,ts2)						\
({									\
  int __ret;								\
									\
  if ((ts1)->tv_sec < (ts2)->tv_sec)					\
    __ret = -1;								\
  else if ((ts1)->tv_sec > (ts2)->tv_sec)				\
    __ret = 1;								\
  else {								\
    if ((ts1)->tv_nsec < (ts2)->tv_nsec)				\
      __ret = -1;							\
    else if ((ts1)->tv_nsec > (ts2)->tv_nsec)				\
      __ret = 1;							\
    else								\
      __ret = 0;							\
  }									\
  __ret;								\
})

/**
 * @brief Clears (zeros) a timespec structure
 *
 * @param ts Pointer to timval struct to clear
 */
#define TIMESPEC_CLEAR(ts)						\
({									\
  (ts)->tv_sec = 0;							\
  (ts)->tv_nsec = 0;							\
})

/**
 * @brief Normalize a timespec structure
 *
 * Carries excess microsends into seconds field
 * If possible, borrows from second field to cancel negative microsecond field
 *
 * @param ts Pointer to timespec struct to normalize
 */
#define TIMESPEC_NORMALIZE(ts)						\
({									\
  (ts)->tv_sec += ((ts)->tv_nsec / 1000000000L);			\
  (ts)->tv_nsec %= 1000000000L;						\
  if ((ts)->tv_sec > 0 && (ts)->tv_nsec < 0L) {				\
    (ts)->tv_sec--;							\
    (ts)->tv_nsec += 1000000000L;					\
  }									\
})

/**
 * @brief Adds two timespec structures
 *
 * Performs <tsdest> = <ts1> + <ts2>
 *
 * @param tsdest Pointer to destination timespec struct (MAY be ts1 or ts2)
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 */
#define TIMESPEC_ADD(tsdest,ts1,ts2)					\
({									\
  (tsdest)->tv_sec = (ts1)->tv_sec+(ts2)->tv_sec;			\
  (tsdest)->tv_nsec = (ts1)->tv_nsec+(ts2)->tv_nsec;			\
  TIMESPEC_NORMALIZE(tsdest);						\
})

/**
 * @brief Interpolates two timespec structures
 *
 * Performs <tsdest> = (1.0-t)*<ts1> + t*<ts2>
 *
 * @param tsdest Pointer to destination timespec struct (MAY be ts1 or ts2)
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 * @param t Fraction [0,1] between ts1 and ts2 at which to interpolate
 */
#define TIMESPEC_INTERPOLATE(tsdest,ts1,ts2,t)				\
({									\
  (tsdest)->ts_sec = (time_t)((1.0-t)*(ts1)->ts_sec+t*(ts2)->ts_sec);	\
  (tsdest)->ts_nsec = (long)((1.0-t)*(ts1)->ts_nsec+t*(ts2)->ts_nsec);	\
  TIMESPEC_NORMALIZE(tsdest);						\
})

/**
 * @brief Subtracts two timespec structures
 *
 * Performs <tsdest> = <ts1> - <ts2>
 *
 * @param tsdest Pointer to destination timespec struct (MAY be ts1 or ts2)
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 */
#define TIMESPEC_SUB(tsdest,ts1,ts2)					\
({									\
  (tsdest)->tv_sec = (ts1)->tv_sec-(ts2)->tv_sec;			\
  (tsdest)->tv_nsec = (ts1)->tv_nsec-(ts2)->tv_nsec;			\
  TIMESPEC_NORMALIZE(tsdest);						\
})

/**
 * @brief Returns difference in seconds between two timespec structures
 *
 * Performs <ts1> - <ts2>
 *
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 */
#define TIMESPEC_DIFF_SEC(ts1,ts2)					\
({									\
  ((ts1)->ts_sec-(ts2)->ts_sec) +					\
    0.000000001*((ts1)->ts_nsec-(ts2)->ts_usec);			\
})

/**
 * @brief Returns difference in milliseconds between two timespec structures
 *
 * Performs <ts1> - <ts2>
 *
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 */
#define TIMESPEC_DIFF_MSEC(ts1,ts2)					\
({									\
  1000.0*((ts1)->ts_sec-(ts2)->ts_sec) +				\
    0.000001*((ts1)->ts_nsec-(ts2)->ts_usec);				\
})

/**
 * @brief Returns difference in microseconds between two timespec structures
 *
 * Performs <ts1> - <ts2>
 *
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 */
#define TIMESPEC_DIFF_USEC(ts1,ts2)					\
({									\
  1000000.0*((ts1)->ts_sec-(ts2)->ts_sec) +				\
   0.001((ts1)->ts_nsec-(ts2)->ts_usec);				\
})

/**
 * @brief Returns difference in nanoseconds between two timespec structures
 *
 * Performs <ts1> - <ts2>
 *
 * @param ts1 Pointer to first timespec struct
 * @param ts2 Pointer to second timespec struct
 */
#define TIMESPEC_DIFF_NSEC(ts1,ts2)					\
({									\
  1000000000.0*((ts1)->ts_sec-(ts2)->ts_sec) +				\
   ((ts1)->ts_nsec-(ts2)->ts_usec);					\
})

/**
 * @brief Adds a specified number of microseconds to a timespec struct
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to a timespec struct
 * @param t Number of microseconds to add
 */
#define TIMESPEC_ADD_NSEC(ts,t)						\
({									\
  (ts)->tv_sec += (t)/1000000000L;					\
  (ts)->tv_nsec += (t)%1000000000L;					\
  TIMESPEC_NORMALIZE(ts);						\
})

/**
 * @brief Adds a specified number of microseconds to a timespec struct
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of microseconds to add
 */
#define TIMESPEC_ADD_USEC(ts,t)						\
({									\
  (ts)->tv_sec += (t)/1000000L;						\
  (ts)->tv_nsec += 1000L*((t)%1000000L);				\
  TIMESPEC_NORMALIZE(ts);						\
})

/**
 * @brief Adds a specified number of milliseconds to a timespec struct
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of milliseconds to add
 */
#define TIMESPEC_ADD_MSEC(ts,t)						\
({									\
  (ts)->tv_sec += (t)/1000L;						\
  (ts)->tv_nsec += 1000000L*((t)%1000L);				\
  TIMESPEC_NORMALIZE(ts);						\
})

/**
 * @brief Adds a specified number of seconds to a timespec struct
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of seconds to add
 */
#define TIMESPEC_ADD_SEC(ts,t)						\
({									\
  (ts)->tv_sec += (t);							\
})

/**
 * @brief Sets a timespec struct to the current time
 *
 * The result may not have full nanosecond precision
 *
 * @param ts Pointer to timespec struct
 */
#define TIMESPEC_SET(ts)						\
({									\
  struct timeval __tvtmp;						\
  gettimeofday(&__tvtmp, NULL);						\
  TIMEVAL_TO_TIMESPEC(&__tvtmp, (ts));					\
})

/**
 * @brief Sets a timespec struct to the current time + n nanoseconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of nanoseconds to add
 */
#define TIMESPEC_SET_ADD_NSEC(ts,t)					\
({									\
  TIMESPEC_SET(ts);							\
  TIMESPEC_ADD_NSEC(ts,t);						\
})

/**
 * @brief Sets a timespec struct to the current time + n microseconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of microseconds to add
 */
#define TIMESPEC_SET_ADD_USEC(ts,t)					\
({									\
  TIMESPEC_SET(ts);							\
  TIMESPEC_ADD_USEC(ts,t);						\
})

/**
 * @brief Sets a timespec struct to the current time + n milliseconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of milliseconds to add
 */
#define TIMESPEC_SET_ADD_MSEC(ts,t)					\
({									\
  TIMESPEC_SET(ts);							\
  TIMESPEC_ADD_MSEC(ts,t);						\
})

/**
 * @brief Sets a timespec struct to the current time + n seconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of seconds to add
 */
#define TIMESPEC_SET_ADD_SEC(ts,t)					\
({									\
  TIMESPEC_SET(ts);							\
  TIMESPEC_ADD_SEC(ts,t);						\
})

/**
 * @brief Sets a timespec struct to contain a certain number of nanoseconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of nanoseconds to place in timespec
 */
#define TIMESPEC_SET_NSEC(ts,t)						\
({									\
  (ts)->tv_sec = t/1000000000L;						\
  (ts)->tv_nsec = (t)%1000000000L;					\
})

/**
 * @brief Sets a timespec struct to contain a certain number of microseconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of microseconds to place in timespec
 */
#define TIMESPEC_SET_USEC(ts,t)						\
({									\
  (ts)->tv_sec = t/1000000L;						\
  (ts)->tv_nsec = 1000L*((t)%1000000L);					\
})

/**
 * @brief Sets a timespec struct to contain a certain number of milliseconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of milliseconds to place in timespec
 */
#define TIMESPEC_SET_MSEC(ts,t)						\
({									\
  (ts)->tv_sec = t/1000L;						\
  (ts)->tv_nsec = 1000000L*((t)%1000L);					\
})

/**
 * @brief Sets a timespec struct to contain a certain number of seconds
 *
 * The result is normalized before returning
 *
 * @param ts Pointer to timespec struct
 * @param t Number of seconds to place in timespec
 */
#define TIMESPEC_SET_SEC(ts,t)						\
({									\
  (ts)->tv_sec = (t);							\
  (ts)->tv_nsec = 0;							\
})

#endif /* _TIMEUTIL_H_ */

