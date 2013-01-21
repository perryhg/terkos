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

/**
 * @file qequadencoder.h
 * Interface for the CQEQuadEncoder class, which provides support for
 * four quadrature encoders on digital I/O ports 1-8.
 */

#ifndef _QEQUADENCODER_H_
#define _QEQUADENCODER_H_

#include "singleton.h"
#include "9302hw.h"
#include "qegpioint.h"
#include "timer.h"

/**
 * VEXpro supports 4 encoders on digital ports 1-8.  These values can be used
 * for the axis parameter of any function in this class.
 */
typedef enum
{
	/** Digital Ports 1 & 2 */  QENC_12 = 0,
	/** Digital Ports 3 & 4 */  QENC_34 = 1,
	/** Digital Ports 5 & 6 */  QENC_56 = 2,
	/** Digital Ports 7 & 8 */  QENC_78 = 3,
                                QENC_AXIS_COUNT
} QEncAxis;

/**
 * Values for the scale parameter of the Configure() function. These values
 * can be used to specify the counts per rotation reported by the
 * ScaledPosition() and ScaledVelocity() functions.  In addition to these
 * predefined values, you can specify any positive integer as a scale.
 */
typedef enum
{
	/** 1 count per rotation */                 QENC_UNIT_TURNS	   =   1,
	/** 256 "Binary" Radians per rotation */    QENC_UNIT_BRADS	   = 256,
	/** 360 Degrees per rotation */             QENC_UNIT_DEGREES  = 360,
	/** 400 Gradians per rotation */            QENC_UNIT_GRADIANS = 400
} QEncUnit;

/**
 * CQEQuadEncoder implements support for four quadrature (incremental)
 * encoders on digital I/O ports 1-8.  This class provides both unscaled
 * and scaled position and velocity data.
 *
 * Unscaled values represent counts of pulses coming from the encoder,
 * and are not converted into any particular angular unit of measure.
 * The angular distance each count represents is determined by the
 * resoultion of the encoder.  Unscaled data is ideal when the position
 * does not need to be translated into an exact rotation count or
 * angle (e.g., keeping two motors in sync, or travel limit detection).
 *
 * Scaled values represent counts that have been normalized to a useful
 * unit such as degrees or brads.  In order to scale the encoder counts,
 * the correct resolution and scale parameters must be provided to the
 * Configure() function.  Scaled values are ideal when the position or
 * velocity needs to be converted into a real unit such as degrees, RPM,
 * or brads (which are convenient for fixed-point math.)
 *
 * To instantiate this class:
 * \code
 * // by pointer
 * CQEQuadEncoder *pqenc = CQEQuadEncoder::GetPtr();
 * // or by reference
 * CQEQuadEncoder &qenc = CQEQuadEncoder::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to
 * GetPtr() or GetRef():
 * \code
 * CQEQuadEncoder::Release();
 * \endcode
 */
class CQEQuadEncoder
{
public:
  /**
   * This internal macro handles instantiation of this class.
   */
  SINGLETON(CQEQuadEncoder);


  /**
   * Configure a quadrature encoder attached to the digital I/O ports.
   * You may call Configure() on an axis that has already been configured
   * in order to change the scale, resolution or direction values.
   *
   * @param axis Selects which encoder to access:
   *	0=(D0,D1), 1=(D2,D3), 2=(D4,D5), 3=(D6,D7).
   *	See QEncAxis for a list of predefined values.
   *
   * @param scale Specifies how many counts should be reported for one
   *	full rotation via the "Scaled" encoder functions.
   *	See QEncUnit for a list of predefined scales, though
   *	any non-zero integer scale factor can be used.
   *	This value is only used by ScaledPosition() and ScaledVelocity().
   *
   * @param resolution Specifies the number of counts the encoder generates
   *	for a full rotation of the axis.
   *	The Vex Quadrature Optical Shaft Encoder (276-2156) uses a produces
   *	360 counts per revolution.  This conveniently means the unscaled
   *	position and velocity values are actually in degrees.
   *	This value is only used by ScaledPosition() and ScaledVelocity().
   *
   * @param invert Reverses the encoder's direction as reported by
   * 	Position() and ScaledPosition(), Velocity() and ScaledVelocity().
   * 	This has the same effect as swapping the two signal cables
   * 	coming from the encoder.
   *
   * @param pio An optional pointer to the CQEGpioInt object.  This is
   *	used to set the Digitial I/O ports associated with the axis to
   *	inputs.  If NULL is provided for pio, then the Configure() function
   *	leaves configuration of the I/O ports to the caller.
   *
   * @return Returns true if configuration was successful and false otherwise.
   */
  bool Configure( unsigned int axis,
				  unsigned long int scale=QENC_UNIT_BRADS,
				  unsigned long int resolution=360,
				  bool invert=false,
				  CQEGpioInt *pio=NULL );

  /**
   * Set the encoder's position to zero.  Zero() is used to "home" or
   * "null" an axis so that the last reported position becomes the new
   * zero position.
   *
   * This will reset both Postion() and ScaledPosition() to zero.
   *
   * NOTE: The zero is applied relative to the last position returned from
   * Position() or ScaledPosition() rather than the current position.
   * This is to ensure any motion that happened between the last known
   * position and Zero() isn't lost.  If you use a limit switch or some
   * other means to identify the home position, you should call Position()
   * immediately before calling Zero() to ensure the correct position is
   * being set as the new zero position.
   *
   * @param axis Selects which encoder to zero:
   *	0=D0&D1, 1=D2&D3, 2=D4&D5, 3=D6&D7.
   *	See QEncAxis for a list of predefined values.
   */
  void Zero(unsigned int axis);

  /**
   * Reads the current position of the axis and returns it as an
   * unscaled value.  The position and time are also stored for
   * a subsequent call to Velocity() or ScaledVelocity().
   *
   * NOTE: The lower 16b are accumulated in hardware, and the upper
   * 16b are synthesized in the driver.  In order to avoid missing
   * counts, Position() or ScaledPosition() must be called at least
   * once every 32,000 encoder counts.  For a VEX quadrature
   * encoder operating at its design limit, this function should be
   * called no less than once every four seconds.  For more typical
   * operation, calling once per minute should be sufficient.
   *
   * @param axis Selects which encoder to read:
   *	0=D0&D1, 1=D2&D3, 2=D4&D5, 3=D6&D7.
   *	See QEncAxis for a list of predefined values.
   *
   * @return The encoder position (unscaled, signed value).
   */
  long int Position(unsigned int axis);

  /**
   * Calculates the angular velocity of the axis and returns it in
   * encoder counts per minute.
   *
   * This function calculates the average velocity of the encoder
   * based on the two most recent calls to Position() or ScaledPosition().
   * Programs interested in unscaled velocity data should first call
   * Postion() then Velocity() on a regular interval.
   *
   * @param axis Selects which encoder to read:
   *	0=D0&D1, 1=D2&D3, 2=D4&D5, 3=D6&D7.
   *	See QEncAxis for a list of predefined values.
   *
   * @return The average encoder velocity in encoder counts per minute.
   */
  long int Velocity(unsigned int axis);

  /**
   * Reads the current position of the axis and returns it as a
   * scaled value.  The position and time are also stored for
   * a subsequent call to Velocity() or ScaledVelocity().
   *
   * NOTE: This function makes use of Position() to read the
   * encoder, so the same sample rate requirement must be met.
   *
   * @param axis Selects which encoder to read:
   *	0=D0&D1, 1=D2&D3, 2=D4&D5, 3=D6&D7.
   *	See QEncAxis for a list of predefined values.
   *
   * @return The encoder position, scaled to the units specified in Configure().
   */
  long int ScaledPosition(unsigned int axis);

  /**
   * Calculates the angular velocity of the axis and returns it as a
   * scaled value.
   *
   * This function calculates the average velocity of the encoder
   * based on the two most recent calls to Position() or ScaledPosition().
   * Programs interested in scaled velocity data should first call
   * ScaledPostion() then ScaledVelocity() on a regular interval.
   *
   * @param axis Selects which encoder to read:
   *	0=D0&D1, 1=D2&D3, 2=D4&D5, 3=D6&D7.
   *	See QEncAxis for a list of predefined values.
   *
   * @return The encoder velocity, scaled to units per minute, where
   * 	units is the scale value provided to the Configure() function.
   */
  long int ScaledVelocity(unsigned int axis);


private:
  CQEQuadEncoder();
  ~CQEQuadEncoder();

  long int		 		Offset[QENC_AXIS_COUNT];
  long int	 	 		Count[QENC_AXIS_COUNT];
  unsigned short int 	dCount[QENC_AXIS_COUNT];
  CTimer::tick_t 		tThis[QENC_AXIS_COUNT];
  CTimer::tick_t 		tLast[QENC_AXIS_COUNT];

  bool			 		Invert[QENC_AXIS_COUNT];
  long int	 	Resolution[QENC_AXIS_COUNT];
  long int	 	ScaleUnits[QENC_AXIS_COUNT];

  C9302Hardware 		*p9302hw;

  volatile unsigned short int	*qenc_reg; // Pointer to quad encoder register (VEXpro FPGA)
};

#endif // _QEQUADENCODER_H_
