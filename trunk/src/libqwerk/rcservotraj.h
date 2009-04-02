#ifndef RCSERVOTRAJ_H
#define RCSERVOTRAJ_H

#include <sys/time.h>
#include <pthread.h>

#include "rcservo.h"

#define QES_BIT(n)                 (1<<n)

class CRCServoTraj : public CRCServo
{
public:
  CRCServoTraj(unsigned char num=QES_DEFAULT_SERVOS, unsigned long addr=QES_DEFAULT_ADDR, bool enable=true);
  ~CRCServoTraj();

  bool Done(unsigned int servo);
  void Stop(unsigned int servo);

  /**
   * Move a servo at a constant velocity to a given position
   *
   * @param servo Servo number to move
   * @param endPosition If absolute, final position; else, offset from current pos * @param velocity Speed at which to move (in ticks/s); sign ignored
   * @param absolute Whether endPosition is absolute
   */
  void Move(unsigned int servo, int endPosition, 
	    int velocity, bool absolute=false);
  /**
   * Move a servo at a constant velocity
   *
   * The direction is specified by the sign of the velocity
   * The servo will stop moving when it reaches either bound (0 or 255)
   *
   * @param servo Servo number to move
   * @param velocity Velocity at which to move (in ticks/s)
   */
  void MoveVelocity(unsigned int servo, int velocity);

  bool s_threadRun;
  pthread_mutex_t s_mutex;
  pthread_cond_t s_cond;

  void ServoTrajectory();
  
protected:
  pthread_t s_thread;
  unsigned char  s_operAxes;
  unsigned short s_trajectory;
  unsigned short s_velocityTrajectory;

  static void *TrajThread(void *arg);
  unsigned char s_trajectoryStartPosition[QES_DEFAULT_SERVOS];
  unsigned char s_trajectoryEndPosition[QES_DEFAULT_SERVOS];
  int s_trajectoryVelocity[QES_DEFAULT_SERVOS];
  struct timeval s_startTime[QES_DEFAULT_SERVOS];
};

#endif

