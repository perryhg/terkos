#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#include "timeutil.h"

#include "rcservotraj.h"
#include "rcservo.h"

void *CRCServoTraj::TrajThread(void *arg)
{
  CRCServoTraj *rcservo = (CRCServoTraj *)arg;

  while(rcservo->s_threadRun)
    {
      // wait for next trajectory to start 
      pthread_mutex_lock(&rcservo->s_mutex);
      if (!rcservo->s_trajectory && !rcservo->s_velocityTrajectory) 
	pthread_cond_wait(&rcservo->s_cond, &rcservo->s_mutex);
      pthread_mutex_unlock(&rcservo->s_mutex);
      
      // execute trajectory as long as one is active
      while(1)
	{
	  pthread_mutex_lock(&rcservo->s_mutex);

          if(!rcservo->s_trajectory && !rcservo->s_velocityTrajectory ||
             !rcservo->s_threadRun)
	    {
	      pthread_mutex_unlock(&rcservo->s_mutex);
	      break;
	    }

	  rcservo->ServoTrajectory();

	  pthread_mutex_unlock(&rcservo->s_mutex);

          //be nice to other processes
          usleep(10000);
	}
    }

  return NULL;
}

CRCServoTraj::CRCServoTraj(unsigned char num, unsigned long addr, bool enable) :
  CRCServo(num, addr, enable)
{
//  pthread_attr_t attr;
  pthread_mutexattr_t mattr;
  unsigned int servo;

  s_operAxes = num;

  s_trajectory = 0;
  s_velocityTrajectory = 0;
  for (servo=0; servo<s_operAxes; servo++)
    {
      s_trajectoryVelocity[servo] = 0;
      s_trajectoryStartPosition[servo] = 0;
      s_trajectoryEndPosition[servo] = 0;
    }

  s_threadRun = true;

  // initialize and set mutex as recursive
  pthread_mutexattr_init(&mattr);
  pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE_NP); 
  pthread_mutex_init(&s_mutex, &mattr); 

  // initialize cond variable
  pthread_cond_init(&s_cond, NULL);

#if 0
  // this code tries to set worker thread to higher priority
  // but it isn't clear if it works
  pthread_t this_thread = pthread_self();
  struct sched_param param;
  int policy = SCHED_RR;

  param.sched_priority = 1;
  pthread_setschedparam(this_thread, policy, &param);
  pthread_attr_init(&attr);
  pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&attr, SCHED_RR);
  param.sched_priority = 50;
  pthread_attr_setschedparam(&attr, &param);
  pthread_create(&s_thread, &attr, TrajThread, (void *)this);
#else
  pthread_create(&s_thread, NULL, TrajThread, (void *)this);
#endif
}

CRCServoTraj::~CRCServoTraj()
{
  s_threadRun = false;

  // signal thread 
  pthread_mutex_lock(&s_mutex);
  pthread_cond_signal(&s_cond);
  pthread_mutex_unlock(&s_mutex);

  // wait for it to exit
  pthread_join(s_thread, NULL);
}

bool CRCServoTraj::Done(unsigned int servo)
{
  bool result;

  pthread_mutex_lock(&s_mutex);
  result = (s_trajectory&QES_BIT(servo))==0;
  pthread_mutex_unlock(&s_mutex);
  
  sched_yield(); // yield cpu

  return result;
}

void CRCServoTraj::Stop(unsigned int servo)
{
  pthread_mutex_lock(&s_mutex);

  s_trajectory &= ~QES_BIT(servo);
  s_velocityTrajectory &= ~QES_BIT(servo);
  pthread_mutex_unlock(&s_mutex);
}

/**
 * Move a servo at a constant velocity to a given position
 *
 * @param servo Servo number to move
 * @param endPosition If absolute, final position; else, offset from current pos
 * @param velocity Speed at which to move (in ticks/s); sign ignored
 * @param absolute Whether endPosition is absolute
 */
void CRCServoTraj::Move(unsigned int servo, int endPosition, 
			int velocity, bool absolute)
{
  unsigned char pos;
  int finalPos;

  if (servo >= s_operAxes)
    return;

  pthread_mutex_lock(&s_mutex);

  // abs(velocity)
  if (velocity<0)
    velocity = -velocity;

  pos = GetPosition(servo);

  // handle absolute vs relative
  if (absolute)
    finalPos = endPosition;
  else
    finalPos = endPosition + (int)pos;

  if (finalPos < 0)
    finalPos = 0;
  else if (finalPos > 255)
    finalPos = 255;

  s_trajectoryStartPosition[servo] = (unsigned char)pos;
  s_trajectoryEndPosition[servo] = finalPos;
  if (s_trajectoryEndPosition[servo]>pos)
    s_trajectoryVelocity[servo] = velocity;
  else
    s_trajectoryVelocity[servo] = -velocity;
  
  s_trajectory |= QES_BIT(servo);
  s_velocityTrajectory &= ~QES_BIT(servo);

  gettimeofday(&s_startTime[servo], NULL);

  // signal trajectory thread 
  pthread_cond_signal(&s_cond);
  pthread_mutex_unlock(&s_mutex);
}

/**
 * Move a servo at a constant velocity
 *
 * The direction is specified by the sign of the velocity
 * The servo will stop moving when it reaches either bound (0 or 255)
 *
 * @param servo Servo number to move
 * @param velocity Velocity at which to move (in ticks/s)
 */
void CRCServoTraj::MoveVelocity(unsigned int servo,
				int velocity)
{
  if (servo >= s_operAxes)
    return;
 
  pthread_mutex_lock(&s_mutex);

  s_trajectoryStartPosition[servo] = GetPosition(servo);
  s_trajectoryVelocity[servo] = velocity;

  s_velocityTrajectory |= QES_BIT(servo);
  s_trajectory &= ~QES_BIT(servo);

  gettimeofday(&s_startTime[servo], NULL);

  // signal trajectory thread 
  pthread_cond_signal(&s_cond);
  pthread_mutex_unlock(&s_mutex);
}

void CRCServoTraj::ServoTrajectory()
{
  int axis;
  struct timeval currtime, elapsed;
  int posdelta;
  int newpos;
  int error;

  gettimeofday(&currtime, NULL);

  for (axis=0; axis<s_operAxes; axis++) {
    if (!((s_trajectory & QES_BIT(axis)) || (s_velocityTrajectory & QES_BIT(axis)))) 
      continue;

    TIMEVAL_SUB(&elapsed, &currtime, &s_startTime[axis]);
    posdelta = s_trajectoryVelocity[axis]*elapsed.tv_sec + s_trajectoryVelocity[axis]*elapsed.tv_usec/1000000;

    newpos = s_trajectoryStartPosition[axis]+posdelta;

    if (s_trajectory & QES_BIT(axis)) {
      error = s_trajectoryEndPosition[axis] - newpos;

      if (error == 0 || (error < 0 && s_trajectoryVelocity[axis] > 0) || (error > 0 && s_trajectoryVelocity[axis] < 0)) {
        //we've reached the destination -- stop trajectory
        newpos = s_trajectoryEndPosition[axis];
        s_trajectory &= ~QES_BIT(axis);
      }
    }
    else if (s_velocityTrajectory & QES_BIT(axis)) {
      if (newpos < 0)
        newpos = 0;
      else if (newpos > 255)
        newpos = 255;

      if (newpos == 0 || newpos == 255) {
        //we've reached the endstops -- stop trajectory
        s_velocityTrajectory &= ~QES_BIT(axis);
      }
    }
    //else, kaboom

    SetPosition(axis, newpos);
  }

}

