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

#ifndef _QEMOTORREC_H
#define _QEMOTORREC_H

#include <pthread.h>
#include "qemotor.h"

enum ERBState
  {
    STATE_STANDBY,
    STATE_TRANSITION_RECORD,
    STATE_RECORD,
    STATE_TRANSITION_PLAY,
    STATE_PLAY
  };

class MotorControllerI;
class CQEMotorRec 
{
public:
  CQEMotorRec(unsigned long size=0, unsigned char axes=0, pthread_mutex_t *pMutex=NULL);
  virtual ~CQEMotorRec();

  void Init(unsigned long size, unsigned char axes, pthread_mutex_t *pMutex);
  virtual void Record();
  virtual void Play();
  void RStop();
  bool Playing();
  bool Recording();
  bool UpdatePosition(); // returns true if RecordUpdate needs updated position

protected:
  friend class MotorControllerI;

  short *m_pdata;
  unsigned long m_readIndex;
  unsigned long m_writeIndex;
  unsigned long m_size;
  unsigned char m_axes;


  bool RecordActive();
  bool RecordServo();
  void RecordUpdate(Axis_position position[], Axis_position desiredPosition[]);
  short *SetPlay();
  short *GetRecord();

  Axis_position m_recIntegral[QEMOT_NUM_MOTORS];
  ERBState m_state;
  bool m_servo;

  pthread_mutex_t *m_pMutex;
};


#endif
