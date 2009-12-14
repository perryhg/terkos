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

#ifndef _QEGPIOINT_H
#define _QEGPIOINT_H

#include "property.h"
#include "singleton.h"
#include "9302hw.h"

// Properties
#define QEG_OBJECT                           3
#define QEG_PROP_WIDTH                       PROP_ID(QEG_OBJECT, 0)
#define QEG_PROP_DATA_REG                    PROP_ID(QEG_OBJECT, 1)
#define QEG_PROP_DATA_DIR_REG                PROP_ID(QEG_OBJECT, 2)
#define QEG_PROP_INTERRUPT_MODE              PROP_ID(QEG_OBJECT, 3)
#define QEG_PROP_INTERRUPT_STATUS            PROP_ID(QEG_OBJECT, 4)
#define QEG_PROP_INTERRUPT_CALLBACK          PROP_ID(QEG_OBJECT, 5)

#define QEG_DEFAULT_BASE                     0x440
#define QEG_NUM_IO                           16

// set interrupt mode, pos, neg, disabled (set/getprop)
// register callback (SetCallback(int vec, callback))
// read timestamps from queue (ReadTimestamp()
// set/read ddr (set/getprop)
// set/read data (set/getprop)

class CQEGpioInt : public IProperty
{
public:
  SINGLETON(CQEGpioInt);

 // IProperty
  virtual int GetProperty(int property, long *value);
  virtual int SetProperty(int property, long value);

private:
  CQEGpioInt();
  ~CQEGpioInt();

  C9302Hardware *m_p9302hw;
  volatile unsigned short *m_data;
  volatile unsigned short *m_dataDir;
  volatile unsigned short *m_intMode;

};

#endif
