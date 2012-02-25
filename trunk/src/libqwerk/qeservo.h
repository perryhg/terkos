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
#define QES_DYNAMIC_BRAKE        50   // 50*4us = 200us

#define QES_DEFAULT_ADDR         0x400

/** 
 * CQEServo allows control of motor ports 1 through 12.  These 3-wire ports
 * are for use with servos, motor controllers, and motors that accept a standard
 * R/C servo PWM signal.
 * - Vex Motor Controller 29 (276-2193)
 * - Vex 3-Wire Servo
 * - Vex 3-Wire Motor (discontinued)
 *
 * The R/C PWM signal is a high-going pulse between 500us and 1500us long.
 * The pulse repeats every 18.6ms, or about 54 times per second.
 *
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
   * Make the motor or servo go idle.
   * This method disables the specified motor port.  Power is still
   * avaialble at the port, but no PWM signal is generated.  In the
   * absence of a regular control signal, most hobby servos and motors
   * will go idle, though some may continue to hold the last commanded
   * position (or speed).  All Vex servos, motors, and motor controllers
   * will go idle if the port is disabled.
   * @param index the motor port to disable; 0 through 11 corresponding to 
   * motor ports 1 through 12.  
   */
  void Disable(unsigned int index);

  /**
   * Activate Dynamic Braking for Vex Motor Controller 29.
   * Enables dynamic braking on the specific motor port.
   * Resume normal motor operation by calling SetCommand(),
   * or set the motor to idle by calling Disable().
   *
   * This is only for use with the Vex Motor Controller 29; it may cause other
   * servos, motors, and motor controllers to shut down or behave erratically.
   * @param index the motor port to brake; 0 through 11 corresponding to 
   * motor ports 1 through 12.
   *
   * Braking is accomplished by setting the control pulse to 200us, which is
   * outside the normal 500us-1500us pulse width range.
   */
  void DynamicBrake(unsigned int index);

  /**
   * Set the position of a servo or the speed of a motor.
   * This method also enables the selected port, and disables
   * dynamic braking.
   *
   * @param index the motor port to set, 0 through 11 corresponding to 
   * motor ports 1 through 12.  
   * @param val The value of the command can be between 0 and 250.
   * - A value of 0 commands the servo to seek the most-clockwise position and a
   * motor to run at its fastest clockwise speed.  This results in a 500us
   * pulse at the 3-wire port.
   * - A value of 125 will command a servo to seek its center position and will
   * command a motor to stop.  This results in a 1000us pulse at the 3-wire
   * port.
   * - A value of 250 will command a servo to seek its most counter-clockwise
   * position and will command a motor to its fastest counter-clockwise speed.
   * This results in a 1500us pulse at the 3-wire port.
   *
   * Notes:
   * - The Vex 3-Wire Motor's direction of rotation is reversed from other
   * motors and servos.
   * - 2-wire motors attached via a Vex Motor Controller 29 can be reversed by
   * plugging the 2-wire connector in "backwards" (red to black, and
   * black-to-red).
   * - The acceptable range of values, and the timing of the pulse they generate
   * can be modified by calling the SetTiming() method.
   */
  void SetCommand(unsigned int index, unsigned short val);

  /** 
   * Get the position/speed of the given servo/motor.
   * The command is the position of a servo or the speed of a motor, 
   * depending on what is attached to the port, either a servo module
   * or a motor module.
   * @param index the motor port to read, 0 through 11 corresponding to 
   * motor ports 1 through 12. 
   * @return Returns the current command, or 0 if the port is disabled
   * or in dynamic braking mode.
   * @see SetCommand() for details regarding command values.
   */    
  unsigned short GetCommand(unsigned int index);
  
  /** 
   * Set the timing of the servo/motor controller, namely the minimum and 
   * maximum of command.  This method is an advanced command that shouldn't
   * need to be called except in special applications.
   *
   * After calling this method, the allowable range for command values will
   * be 0 to (max-min).
   *
   * Parameter values are in 4us units, but this is implementation-specific
   * and is subject to change in future revisions.
   * @param min sets the duration of the shortest PWM pulse, corresponding
   * with a command value of 0.<br>
   * The default value is QES_DEFAULT_MIN
   * @param max sets the duration of the longest PWM pulse allowed.
   * The default value is QES_DEFAULT_MAX.  A max value of QES_DEFAULT_FREQ (or
   * greater) will result in no PWM pulse being generated (100% pulse width)
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
