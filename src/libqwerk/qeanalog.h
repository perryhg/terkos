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

/** 
 * CQEAnalog allows you to read the analog voltage channels.  
 * For now this class is input-only (read-only).
 * To instantiate this class: 
 * \code
 * // by pointer
 * CQEAnalog *panalog = CQEAnalog::GetPtr(); 
 * // or by reference
 * CQEAnalog &analog = CQEAnalog::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQEAnalog::Release();
 * \endcode
 */ */
class CQEAnalog : public IProperty
  
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */ 
  SINGLETON(CQEAnalog);
  
  /**
   * Read the raw analog-to-digital channel value.  It is recommended that you 
   * call GetADVoltage() instead. The channels 
   * are typically low bandwidth -- no greater than 1 kHz.  
   * @param channel the analog channel number (0 through 24 for VEXPro ARM9)
   * @return the raw a/d channel value.  This value is proportional to the 
   * actual voltage.
   */
  unsigned short GetADRaw(unsigned int channel);

  /**
   * Read the analog-to-digital channel voltage.  The channels 
   * are typically low bandwidth -- no greater than 1 kHz.  
   * @param channel the analog channel number (0 through 24 for VEXPro ARM9)
   * @return the a/d channel voltage in millivolts. 
   * For VexPRO ARM 9, these channels are:
   * - 0=External analog channel 1 (via 3-pin connector banks)
   * - 1=External analog channel 2
   * - 2=External analog channel 3
   * - 3=External analog channel 4
   * - 4=External analog channel 5
   * - 5=External analog channel 6
   * - 6=External analog channel 7
   * - 7=External analog channel 8
   * - 8=External analog channel 9
   * - 9=External analog channel 10
   * - 10=External analog channel 11
   * - 11=External analog channel 12
   * - 12=External analog channel 13
   * - 13=External analog channel 14
   * - 14=External analog channel 15
   * - 15=External analog channel 16
   * - 16=PSD protected voltage for motors 1 through 4
   * - 17=PSD protected voltage for motors 5 through 8
   * - 18=PSD protected voltage for motors 9 through 12
   * - 19=PSD protected voltage for motors 13 and 14
   * - 20=PSD protected voltage for motors 15 and 16
   * - 21=Battery backup voltage
   * - 22=Regulator voltage for analog and digital I/O connectors (5V)
   * - 23=Regulator voltage for VEXPro internal electronics (5V)
   * - 24=Main battery voltage
   */
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
