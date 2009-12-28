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

#include <stdio.h>
#include <stdlib.h>
#include "qemotorrec.h"

CQEMotorRec::CQEMotorRec(unsigned long size, unsigned char axes, pthread_mutex_t *pMutex)
{
  Init(size, axes, pMutex);
}

void CQEMotorRec::Init(unsigned long size, unsigned char axes, pthread_mutex_t *pMutex)
{
  m_size = size;
  m_axes = axes;
  m_pMutex = pMutex;

  m_readIndex = 0;
  m_writeIndex = 0;
  m_pdata = 0;
  m_state = STATE_STANDBY;
  m_servo = false;
}

CQEMotorRec::~CQEMotorRec()
{
  if (m_pdata)
    free(m_pdata);
}

short *CQEMotorRec::GetRecord()
{
  short *result;

  if (m_writeIndex>=m_size)
    return NULL;

  result = m_pdata + m_writeIndex;
  m_writeIndex += m_axes;

  return result;
}

short *CQEMotorRec::SetPlay()
{
  short *result;

  if (m_readIndex>=m_size || m_readIndex>=m_writeIndex)
    return NULL;

  result = m_pdata + m_readIndex;
  m_readIndex += m_axes;

  return result;
}

void CQEMotorRec::Record()
{
  pthread_mutex_lock(m_pMutex);

  if (!m_pdata)
    m_pdata = (short *)malloc(m_size*sizeof(short));
  m_writeIndex = 0;
  m_state = STATE_TRANSITION_RECORD;

  pthread_mutex_unlock(m_pMutex);
}

void CQEMotorRec::Play()
{
  pthread_mutex_lock(m_pMutex);

  m_readIndex = 0;
  m_state = STATE_TRANSITION_PLAY;

  pthread_mutex_unlock(m_pMutex);
}

void CQEMotorRec::RStop()
{
  pthread_mutex_lock(m_pMutex);

  m_servo = false;
  m_state = STATE_STANDBY;

  pthread_mutex_unlock(m_pMutex);
}

bool CQEMotorRec::Playing()
{
  bool result;

  pthread_mutex_lock(m_pMutex);
  
  result = m_state==STATE_TRANSITION_PLAY || m_state==STATE_PLAY;

  pthread_mutex_unlock(m_pMutex);
  
  return result;
}

bool CQEMotorRec::Recording()
{
  bool result;

  pthread_mutex_lock(m_pMutex);

  result = m_state==STATE_TRANSITION_RECORD || m_state==STATE_RECORD;

  pthread_mutex_unlock(m_pMutex);

  return result;
}

void CQEMotorRec::RecordUpdate(Axis_position position[], Axis_position desiredPosition[])
{
  unsigned char axis;
  short *data;
  if (m_state==STATE_TRANSITION_PLAY)
    {
      for (axis=0; axis<m_axes; axis++)
	{
	  m_recIntegral[axis] = position[axis];
	  desiredPosition[axis] = m_recIntegral[axis];
	}
      m_servo = true;
      m_state = STATE_PLAY;
    }
  else if (m_state==STATE_PLAY)
    {
      if ((data=SetPlay())==0)
	RStop();
      else
	{
	  for (axis=0; axis<m_axes; axis++)
	    {
	      m_recIntegral[axis] += (Axis_position)data[axis];
	      desiredPosition[axis] = m_recIntegral[axis];
	    }
	}
    }
  else if (m_state==STATE_TRANSITION_RECORD) 
    {
      for (axis=0; axis<m_axes; axis++)
	m_recIntegral[axis] = position[axis];
      m_servo = false;
      m_state = STATE_RECORD;
    }
  else if (m_state==STATE_RECORD) 
    {
      if ((data=GetRecord())==0)
	m_state = STATE_STANDBY;
      else
	{
	  for (axis=0; axis<m_axes; axis++)
	    {
	      data[axis] = (short)(position[axis] - m_recIntegral[axis]);
	      m_recIntegral[axis] = position[axis];
	    }
	}
    }
}

bool CQEMotorRec::RecordActive()
{
  return m_state != STATE_STANDBY;
}

bool CQEMotorRec::RecordServo()
{
  return m_servo;
}

bool CQEMotorRec::UpdatePosition()
{
  return m_state != STATE_PLAY;
}


