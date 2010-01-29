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

#ifndef QEMOTORTRAJ_H
#define QEMOTORTRAJ_H

#include <pthread.h>
#include "qemotoruser.h"
#include "qemotorrec.h"

#define QEM_BIT(n)                 (1<<n)
#define QEM_POS_SCALE              8


/**
 * CQEMotorTraj allows the regular motors in ports 13, 14, 15 and 16 to 
 * have their position controlled via smooth controlled motion.  
 * This motion is 
 * called a "trapezoid" trajectory with a constant 
 * acceleration at the beginning of the trajectory, 
 * a constant desired maximum velocity in the middle,
 * and a constant desired maximum deceleration to the goal position 
 * at the end of the trajectory.
 * To instantiate this class: 
 * \code
 * // by pointer
 * CQEMotorTraj *pmt = CQEMotorTraj::GetPtr(); 
 * // or by reference
 * CQEMotorTraj &mt = CQEMotorTraj::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQEMotorTraj::Release();
 * \endcode
 */
class CQEMotorTraj : public CQEMotorUser, public CQEMotorRec
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */ 
  SINGLETON(CQEMotorTraj);

  /**
   * Checks to see if the given axis is done with the last Move command.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @return true if move is done, false otherwise.
   */
  bool Done(unsigned int axis);

  /**
   * Stop the last move command.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   */
  void Stop(unsigned int axis);

  /**
   * Initiate motor movement with trapezoid trajectory and position endpoint.  
   * A trapezoid
   * trajectory starts  with a constant 
   * acceleration at the beginning of the trajectory, 
   * a constant desired maximum velocity in the middle,
   * and a constant desired maximum deceleration to the goal position 
   * at the end of the trajectory.  The units for these values (position, 
   * velocity, acceleration) are in ticks, ticks/s and ticks/s/s, where
   * ticks are an angular unit based on the back-emf constant of the motor.  
   * In general, if you wish to obtain real units such as meters, millimeters,
   * degrees, radians, etc., you need to perform a simple calibration 
   * to determine the correct scaling constant for your system, such as 
   * ticks/meter, ticks/millimeter, ticks/degree, or ticks/radian depending
   * on what you need/prefer.
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @param endPosition goal position of axis, can be positive or negative.
   * @param velocity desired maximum velocity.
   * @param acceleration desired acceleration/deceleration,
   * strictly positive value.
   * @param absolute this value determines if the endPosition value is 
   * relative to the current position or if the endPosition is relative to 
   * a fixed origin (zero position)-- if it is true it is relative to a 
   * fixed origin and
   * if it is false it is relative to the current position of the motor.
   */
  virtual void Move(unsigned int axis,
		    Axis_position endPosition, int velocity, 
		    unsigned int acceleration, bool absolute=false);
  /**
   * Initiate a motor movement with specified velocity.
   * If the current motor velocity is not equal to the specified 
   * velocity, the motor will either accelerate or decelerate with 
   * the specified acceleration  to the
   * specified velocity. 
   * @param axis the axis in question, either 0, 1, 2, or 3, corresponding
   * to motor ports 13, 14, 15, and 16, respectively.
   * @param velocity the desired velocity, can be positive or negative.
   * @param acceleration desired acceleration/deceleration,
   * strictly positive value.
   */
  virtual void MoveVelocity(unsigned int axis,
			    int velocity, unsigned int acceleration);

  // CQEMotorRec methods
  virtual void Record();
  virtual void Play();

protected:
  CQEMotorTraj();
  ~CQEMotorTraj();

  // trajectory generating methods
  void TrapezoidTrajectory();
  void RecordTrajectory();

  // write trajectory waypoints to device
  void WriteTrajectory();

  // read positions from motors
  void ReadPosition();

  bool m_threadRun;
  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;
  
  pthread_t m_thread;
  unsigned char  m_operAxes;
  unsigned short m_trajectory;
  unsigned short m_velocityTrajectory;

  static void *TrajThread(void *arg);
  Axis_position m_trajectoryEndPosition[QEMOT_NUM_MOTORS]; // enc << posScale
  int m_trajectoryVelocity[QEMOT_NUM_MOTORS]; // enc/period << posScale
  unsigned int m_trajectoryAcceleration[QEMOT_NUM_MOTORS]; // enc/period/period
 
  // Trajectory generator output   
  int           m_generatedTrajectoryVelocity[QEMOT_NUM_MOTORS];   
  int           m_generatedTrajectoryVelocityUs[QEMOT_NUM_MOTORS];   
  long long     m_generatedTrajectoryPosition[QEMOT_NUM_MOTORS];
  Axis_position m_generatedTrajectoryPositionUs[QEMOT_NUM_MOTORS]; 
  int           m_accIntegral[QEMOT_NUM_MOTORS]; // enc << posScale
  Axis_position m_readPositionUs[QEMOT_NUM_MOTORS];
};

#endif
