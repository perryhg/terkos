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

#ifndef _QEMOTORUSER_H
#define _QEMOTORUSER_H

#include "qemotor.h"
#include "singleton.h"

#define QEM_DEVICE                      "/dev/qemot"
#define QEM_OFFSET_UPPER_THRESHOLD      150*16 // 16 is the number of bemf measurements
#define QEM_OFFSET_WINDOW_SCALE         9
#define QEM_OFFSET_WINDOW               (1<<QEM_OFFSET_WINDOW_SCALE)
#define QEM_INIT_TIMEOUT                (1<<(QEM_OFFSET_WINDOW_SCALE+2))
#define QEM_DELAY                       9
#define QEM_PERIOD                      110
#define QEM_WRITE_QUEUE_SIZE            5*sizeof(long)

class CQwerkHardware;

/** 
 * CQEMotorUser is the class that interacts directly with the motor 
 * control device driver for regular motors 13, 14, 15 and 16.  
 * It performs low-level motor control tasks
 * such as setting control frequency, setting motor PWM, 
 * getting motor position and setting PID gain values.
 * To instantiate this class:
 * \code
 * // by pointer
 * CQEMotorUser *pmotor = CQEMotorUser::GetPtr(); 
 * // or by reference
 * CQEMotorUser &motor = CQEMotorUser::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQEMotorUser::Release();
 * \endcode
 */
class CQEMotorUser  
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */ 
  SINGLETON(CQEMotorUser);

  /**
   * Get control frequency for all axes (motors 13, 14, 15 and 16).
   * @return value expressed in updates per second.
   */
  unsigned int GetFrequency();

  /**
   * Set control frequency for all axes (motor 13, 14, 15 and 16).  It is
   * highly recommended that you not call this method unless you have a 
   * a good understanding of how it affects axis control.  
   * @param desired control frequency expressed in updates per second.
   * @return 0 if success, nonzero otherwise. 
   */
  int          SetFrequency(unsigned int freq);

  /**
   * Get current position of axis.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @return the axis position expressed in "ticks".
   * Ticks are an angular unit based on the back-emf constant of the motor.
   */
  virtual Axis_position GetPosition(unsigned int axis);             

  /** 
   * Get position of all axes as a vector.
   * @param array of Axis_position where the axis positions will be written.
   */
  virtual void GetPositionVector(Axis_position position[]);  

  /**
   * Set PWM command value of the specified axis.  The PWM value determines
   * the voltage that the motor will receive and hence the motor's speed
   * and torque.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @param pwm the desired PWM value ranging between -255 and 255.  Values
   * outside this range will be limited to the respective maximum or minimum 
   * value.  
   */
  void         SetPWM(unsigned int axis, int pwm);

  /**
   * Get the PWM command value of the specified axis.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @return the current PWM value ranging between -255 and 255. 
   */ 
  int          GetPWM(unsigned int axis);

  /**
   * Set PID and Velocity feedforward gains.  It is
   * highly recommended that you not call this method unless you have a 
   * a good understanding of how it affects axis control.  
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @param pGain proportional gain value.
   * @param iGain integral gain value.
   * @param dGain derivative gain value.
   * @param vGain velocity feedforward gain value.
   */ 
  void         SetPIDVGains(unsigned int axis, unsigned int pGain, 
			   unsigned int iGain, unsigned int dGain,
			   unsigned int vGain);

  /**
   * Set the "hold" state of the axis, which determines if the current 
   * axis position is actively held stationary or not when there is no
   * movement command.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @param val true actively holds the current position, false releases 
   * axis from active control when there is no movement command.
   */ 
  void         Hold(unsigned int axis, bool val);

  /**
   * Stop the specified axis at its current position.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   */
  void         Stop(unsigned int axis);

  // other functions...
  /**
   * Set the position of the specified axis.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @param pos the position to set.
   */
  void         SetPosition(unsigned int axis, Axis_position pos);

  long	       GetVelocity(unsigned int axis); 
  void         GetVelocityVector(long velocity[]);
  void         SetCalibrate(unsigned int axis, unsigned int cal);
  void         SetTiming(unsigned int axis, unsigned int waitInterval);
  bool         GetDone(unsigned int axis);
  void         HandleOffset();
  void         FlushWriteQueue(unsigned int axis);
  void         SetWriteQueueSize(unsigned int size);

protected:
  CQEMotorUser();
  virtual ~CQEMotorUser();

  int ConstructCall(int axis, int findex, ...);

  unsigned int m_servoAxes;
  int m_handle[QEMOT_NUM_MOTORS];
};

#endif
