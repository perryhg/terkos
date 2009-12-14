//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v3 (http://www.gnu.org/licenses/gpl-3.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "qemotortraj.h"

void *CQEMotorTraj::TrajThread(void *arg)
{
  CQEMotorTraj *motor = (CQEMotorTraj *)arg;

  while(motor->m_threadRun)
    {
      // wait for next trajectory to start 
      pthread_mutex_lock(&motor->m_mutex);
      if (!motor->m_trajectory) 
	pthread_cond_wait(&motor->m_cond, &motor->m_mutex);      
      pthread_mutex_unlock(&motor->m_mutex);
      
      // execute trajectory as long as one is active
      while(1)
	{
	  // perform read() operations outside of grabbing mutex to 
	  // promote good thread interleaving
	  if (motor->UpdatePosition())
	    motor->ReadPosition();

	  pthread_mutex_lock(&motor->m_mutex);

	  if(!motor->m_trajectory && 
	     !motor->m_velocityTrajectory && 
	     !motor->RecordActive() || 
	     !motor->m_threadRun)
	    {
	      pthread_mutex_unlock(&motor->m_mutex);
	      break;
	    }

	  if (motor->RecordActive())
	    motor->RecordTrajectory();
	  else
	    motor->TrapezoidTrajectory();

	  pthread_mutex_unlock(&motor->m_mutex);
	  
	  // perform write() operations outside of grabbing mutex 
	  // to promote good thread interleaving
	  motor->WriteTrajectory();
	}
    }
  return NULL;
}

CQEMotorTraj::CQEMotorTraj(int axis0, int axis1, int axis2, int axis3) : 
  CQEMotorUser(axis0, axis1, axis2, axis3),
  CQEMotorRec()	 
{
  pthread_mutexattr_t mattr;
  unsigned int axis;

  // TODO: add kinematics
  m_operAxes = m_servoAxes;

  m_trajectory = 0;
  m_velocityTrajectory = 0;
  for (axis=0; axis<m_operAxes; axis++)
    {
      m_accIntegral[axis] = 0;
      m_generatedTrajectoryPosition[axis] = 0;
      m_generatedTrajectoryVelocity[axis] = 0;
      m_generatedTrajectoryVelocityUs[axis] = 0;
      m_trajectoryEndPosition[axis] = 0;
    }

  m_threadRun = true;

  // initialize and set mutex as recursive
  pthread_mutexattr_init(&mattr);
  pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE_NP); 
  pthread_mutex_init(&m_mutex, &mattr); 

  // initialize cond variable
  pthread_cond_init(&m_cond, NULL);

  // initialize record object
  CQEMotorRec::Init(0x20000, m_servoAxes, &m_mutex);

#if 0
  // this code tries to set worker thread to higher priority
  // but it isn't clear if it works
  pthread_attr_t attr;
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
  pthread_create(&m_thread, &attr, TrajThread, (void *)this);
#else
  pthread_create(&m_thread, NULL, TrajThread, (void *)this);
#endif
}

CQEMotorTraj::~CQEMotorTraj()
{
  m_threadRun = false;

  // signal thread 
  pthread_mutex_lock(&m_mutex);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);

  // wait for it to exit
  pthread_join(m_thread, NULL);
}

bool CQEMotorTraj::Done(unsigned int axis)
{
  bool done, result;

  pthread_mutex_lock(&m_mutex);
  done = GetDone(axis);
  result = (m_trajectory&QEM_BIT(axis))==0 && done;
  pthread_mutex_unlock(&m_mutex);
  
  usleep(0); // yield cpu

  return result;
}

void CQEMotorTraj::Stop(unsigned int axis)
{
  pthread_mutex_lock(&m_mutex);
  CQEMotorUser::Stop(axis);
  m_trajectory &= ~QEM_BIT(axis);
  m_velocityTrajectory &= ~QEM_BIT(axis);
  pthread_mutex_unlock(&m_mutex);
}

void CQEMotorTraj::Move(unsigned int axis,
			Axis_position endPosition, int velocity, 
			unsigned int acceleration, bool absolute)
{
  Axis_position pos;

  pthread_mutex_lock(&m_mutex);

  // can't interrupt current move for now -- could change in the future
  if (!Done(axis))
    {
      pthread_mutex_unlock(&m_mutex);
      return;
    }

  // abs(velocity)
  if (velocity<0)
    velocity = -velocity;

  pos = GetPosition(axis);

  // sync trajectory
  m_generatedTrajectoryPosition[axis] = (long long)pos << QEM_POS_SCALE;   

  // handle absolute vs relative
  if (!absolute)
    endPosition += pos;

  m_trajectoryEndPosition[axis] = endPosition << QEM_POS_SCALE;
  if (m_trajectoryEndPosition[axis]>m_generatedTrajectoryPosition[axis])
    m_trajectoryVelocity[axis] = velocity << QEM_POS_SCALE;
  else
    m_trajectoryVelocity[axis] = -(velocity << QEM_POS_SCALE);
  
  m_trajectoryAcceleration[axis] = acceleration;
  
  m_generatedTrajectoryVelocity[axis] = 0;
  m_generatedTrajectoryVelocityUs[axis] = 0;
  m_accIntegral[axis] = 0;
  m_trajectory |= QEM_BIT(axis);
  m_velocityTrajectory &= ~QEM_BIT(axis);

  // signal trajectory thread 
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
}

void CQEMotorTraj::MoveVelocity(unsigned int axis,
				int velocity, unsigned int acceleration)
{
  Axis_position pos;
  
  pthread_mutex_lock(&m_mutex);

  if (Done(axis))
    {
      pos = GetPosition(axis);
      // sync trajectory
      m_generatedTrajectoryPosition[axis] = (long long)pos<<QEM_POS_SCALE;
      m_generatedTrajectoryVelocity[axis] = 0;
      m_generatedTrajectoryVelocityUs[axis] = 0;
    }

  m_trajectoryVelocity[axis] = velocity<<QEM_POS_SCALE;
  m_trajectoryAcceleration[axis] = acceleration;

  m_velocityTrajectory |= QEM_BIT(axis);
  m_trajectory &= ~QEM_BIT(axis);

  // signal trajectory thread 
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
}

void CQEMotorTraj::Record()
{
  CQEMotorRec::Record();

  // signal trajectory thread to start
  pthread_mutex_lock(&m_mutex);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
}

void CQEMotorTraj::Play()
{
  CQEMotorRec::Play();

  // signal trajectory thread to start
  pthread_mutex_lock(&m_mutex);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);
}

void CQEMotorTraj::TrapezoidTrajectory()
{
  Axis_position decelDiff;
  bool decelerate;
  bool finish;
  unsigned char axis;
  
  for (axis=0; axis<m_operAxes; axis++)
    {
      decelerate = false;
      finish = false;
      
      if (!(m_trajectory&QEM_BIT(axis)) && !(m_velocityTrajectory&QEM_BIT(axis)))
	continue;
      
      if (m_trajectory&QEM_BIT(axis))
	{
	  decelDiff = m_trajectoryEndPosition[axis]-m_generatedTrajectoryPosition[axis]-m_accIntegral[axis];
	  
	  if (m_generatedTrajectoryVelocity[axis]>0)
	    decelerate = decelDiff<=0;// && decelDiff>=-(m_trajectoryVelocity[axis] >> (QEM_POS_SCALE-1));
	  else if (m_generatedTrajectoryVelocity[axis]<0)
	    decelerate = decelDiff>=0;// && decelDiff<=-(m_trajectoryVelocity[axis] >> (QEM_POS_SCALE-1));
	}
      
      // if we can decelerate within good tolerance...
      if (decelerate)
	{
	  // decelerate
	  if (m_generatedTrajectoryVelocity[axis]>0)
	    {
	      if (m_generatedTrajectoryVelocity[axis]>=(int)(m_trajectoryAcceleration[axis]<<1))
		m_generatedTrajectoryVelocity[axis] -= m_trajectoryAcceleration[axis];
	      finish = m_generatedTrajectoryPosition[axis]>=(int)m_trajectoryEndPosition[axis]; 
	    }
	  else 
	    {
	      if (m_generatedTrajectoryVelocity[axis]<=-(int)(m_trajectoryAcceleration[axis]<<1))
		m_generatedTrajectoryVelocity[axis] += m_trajectoryAcceleration[axis];
	      finish = m_generatedTrajectoryPosition[axis]<=(int)m_trajectoryEndPosition[axis];
	    }
	  
	  m_generatedTrajectoryVelocityUs[axis] = m_generatedTrajectoryVelocity[axis] >> QEM_POS_SCALE;
	  m_accIntegral[axis] -= m_generatedTrajectoryVelocityUs[axis];
	  // if our position error is within tolerance, we've arrived 
	  if (finish)
	    {
	      m_generatedTrajectoryPosition[axis] = m_trajectoryEndPosition[axis];
	      m_trajectory &= ~QEM_BIT(axis);
	    }
	}
      else if (m_generatedTrajectoryVelocity[axis]<m_trajectoryVelocity[axis])
	{
	  m_accIntegral[axis] += m_generatedTrajectoryVelocityUs[axis];
	  m_generatedTrajectoryVelocity[axis] += m_trajectoryAcceleration[axis];
	  if (m_generatedTrajectoryVelocity[axis]>m_trajectoryVelocity[axis])
	    m_generatedTrajectoryVelocity[axis] = m_trajectoryVelocity[axis];
	  m_generatedTrajectoryVelocityUs[axis] = m_generatedTrajectoryVelocity[axis] >> QEM_POS_SCALE;
	}
      else if (m_generatedTrajectoryVelocity[axis]>m_trajectoryVelocity[axis])
	{
	  m_accIntegral[axis] += m_generatedTrajectoryVelocityUs[axis];
	  m_generatedTrajectoryVelocity[axis] -= m_trajectoryAcceleration[axis];
	  if (m_generatedTrajectoryVelocity[axis]<m_trajectoryVelocity[axis])
	    m_generatedTrajectoryVelocity[axis] = m_trajectoryVelocity[axis];
	  m_generatedTrajectoryVelocityUs[axis] = m_generatedTrajectoryVelocity[axis] >> QEM_POS_SCALE;
	}
      // else do nothing (hold current velocity)
      
      // velocity stop = done
      if ((m_velocityTrajectory&QEM_BIT(axis)) && m_trajectoryVelocity[axis]==0 && m_generatedTrajectoryVelocity[axis]==0)
	m_velocityTrajectory &= ~QEM_BIT(axis);
      
      m_generatedTrajectoryPosition[axis] += m_generatedTrajectoryVelocityUs[axis];
      m_generatedTrajectoryPositionUs[axis] = m_generatedTrajectoryPosition[axis] >> QEM_POS_SCALE;
    }
}

void CQEMotorTraj::RecordTrajectory()
{
  RecordUpdate(m_readPositionUs, m_generatedTrajectoryPositionUs);
}

void CQEMotorTraj::WriteTrajectory()
{
  unsigned int axis;

  for (axis=0; axis<m_servoAxes; axis++)
    {
      if (m_trajectory&QEM_BIT(axis) || 
	  m_velocityTrajectory&QEM_BIT(axis) ||
	  RecordServo())
	write(m_handle[axis], 
	      (char *)&m_generatedTrajectoryPositionUs[axis], sizeof(long));
    }

}

void CQEMotorTraj::ReadPosition()
{
  unsigned int axis;
  // Axis_position pos[m_servoAxes];

  for (axis=0; axis<m_servoAxes; axis++)
    read(m_handle[axis], (char *)&m_readPositionUs[axis], sizeof(long));
}


