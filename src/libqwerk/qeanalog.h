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

#ifndef _QEANALOG_H
#define _QEANALOG_H

#include "9302hw.h"
#include "property.h"
#include "singleton.h"

#define QAD_AD_CHANNELS               25
#define QAD_AD_ANALOG_IN_CHANNEL0_7   1
#define QAD_AD_ANALOG_IN_CHANNEL8_15  2
#define QAD_AD_ANALOG_IN_CHANNEL16_23 3
#define QAD_AD_ANALOG_IN_CHANNEL24    0

#define QAD_AD_SCALE                  16

class CQEAnalog : public IProperty
  
{
public:
  SINGLETON(CQEAnalog);
  
  unsigned short GetADRaw(unsigned int channel);
  unsigned short GetADVoltage(unsigned int channel);
  // IProperty
  virtual int GetProperty(int property, long *value);
  virtual int SetProperty(int property, long value);

private:
  CQEAnalog();
  ~CQEAnalog();

  C9302Hardware *m_p9302hw;
  unsigned short m_adcOffset;

  static const unsigned int m_num[25];
};  


#endif
