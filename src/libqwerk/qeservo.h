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

#ifndef _QESERVO_H
#define _QESERVO_H

#include "singleton.h"
#include "9302hw.h"

#define QES_DEFAULT_SERVOS       12

#define QES_DEFAULT_DIVIDER      400  // divide 100mhz by 400->250khz, 4us 
#define QES_DEFAULT_FREQ         4650 // 4650 * 4us = 18.6ms->53.76hz
#define QES_DEFAULT_MIN          250  // 250*4us = 1ms
#define QES_DEFAULT_MAX          500  // 500*4us = 2ms

#define QES_DEFAULT_ADDR         0x400

/** 
 * CQEServo allows control of motor ports 1 through 12.  These motor ports
 * have 3 wires instead of just 2.
 * To instantiate this class: 
 * \code
 * // by pointer
 * CQEServo *pservo = CQEServo::GetPtr(); 
 * // or by reference
 * CQEServo &servo = CQEServo::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQEServo::Release();
 * \endcode
 */
class CQEServo
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */
  SINGLETON(CQEServo);

  /**
   * Turn off servo/motor.
   * @param index the motor in question, 0 through 11 corresponding to 
   * motor ports 1 through 12.  
   */
  void Disable(unsigned int index);

  /**
   * Set the command of the given servo/motor.  
   * The command is the position of a servo or the speed of a motor, 
   * depending on what is attached to the port, either a servo module
   * or a motor module.  
   * @param index the motor in question, 0 through 11 corresponding to 
   * motor ports 1 through 12.  
   * @param val The value of the command can be between 0 and 250. 
   */
  void SetCommand(unsigned int index, unsigned short val);

  /** 
   * Get the command of the given servo/motor.
   * The command is the position of a servo or the speed of a motor, 
   * depending on what is attached to the port, either a servo module
   * or a motor module.  
   * @param index the motor in question, 0 through 11 corresponding to 
   * motor ports 1 through 12.  
   */    
  unsigned short GetCommand(unsigned int index);
  
  /** 
   * Set the timing of the servo/motor controller, namely the minimum and 
   * maximum of command.  This method shouldn't 
   * need to be called except in special applications.
   */
  void SetTiming(unsigned short min, unsigned short max);

private:
  CQEServo();
  ~CQEServo();

  void SetSignal(bool set);
  
  static void SigHandler(int signum);

  unsigned short m_min;
  unsigned short m_max;
  unsigned int m_num;

  volatile unsigned short *m_divider;
  volatile unsigned short *m_freq;
  volatile unsigned short *m_up;

  C9302Hardware *m_p9302hw; 
};

#endif
