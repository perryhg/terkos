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
 * @file vexi2cencoder.h
 * Interface for the CVexI2CEncoder class, which serves as
 * the device driver for Vex Integrated Moter Encoders (IME).
 */

#ifndef _VEXI2CENCODER_H
#define _VEXI2CENCODER_H


#include "property.h"
#include "vexi2cdevice.h"


#define VIME_OBJECT				6
#define VIME_PROP_IME_TYPE		PROP_ID(VIME_OBJECT, 1)
#define VIME_PROP_INVERT		PROP_ID(VIME_OBJECT, 2)
#define VIME_PROP_SCALE_MUL		PROP_ID(VIME_OBJECT, 3)
#define VIME_PROP_SCALE_DIV		PROP_ID(VIME_OBJECT, 4)
#define VIME_PROP_SCALE_SPEED	PROP_ID(VIME_OBJECT, 5)

#define VIME_TYPE_269			(1)
#define VIME_TYPE_393_TORQUE	(2)
#define VIME_TYPE_393_SPEED		(3)


/**
 * Driver class for Vex Integrated Moter Encoders (IME).
 * It interfaces with the CQEI2CBus class to communicate with attached
 * Vex I2C motor encoders.  One instance of this class is needed for
 * each encoder.
 */
class CVexI2CEncoder : public CVexI2CDevice
{
public:
  /**
   * Constructor for the Vex Integrated Motor Encoder (IME) driver.
   * Create an instance of this class for each IME attached to the
   * I2C port.  Instances should be created in the order of chaining,
   * starting with IME connected directly to the VEXpro, and ending
   * with the last IME in the chain.
   *
   * @param ime_type This sets the type of IME and the gear ratio for 393
   * 	type IMEs.  Initialization will fail if the wrong type of IME is
   * 	specified.  If the wrong gearing is specified, then the scaled
   * 	position and speed values will be inaccurate.
   *
   * @param invert Reverses the encoder's direction as reported by
   * 	Position() and ScaledPosition(). It does not effect Speed() or
   * 	ScaledSpeed(), since those functions always report positive
   *	values.
   *
   * @param newaddr The I2C address to be assigned to this encoder.
   * 	The address must be in the range 0x10..0x2f
   *
   * @param term Termination control.  This should be set to true for
   * 	the last VEX I2C device chained on the bus, and false for all others.
   *
   */
  CVexI2CEncoder(int ime_type, bool invert, unsigned short newaddr, bool term=true);
  
  
  /**
   * Destructor for the Vex Integrated Motor Encoder (IME) driver.
   */
  ~CVexI2CEncoder();

  /**
   * Set the encoder's position to zero.  This is used to "home" or
   * "null" an axis so that it reads zero at a specific position.
   *
   * This will reset both Postion() and ScaledPosition() to zero.
   *
   * NOTE: The zero is applied relative to the last position returned by
   * Position() or ScaledPosition() rather than the current position.
   * This is to ensure any motion that happened between the last position
   * read and the Zero() isn't lost.  If you use a limit switch or some
   * other means to identify the home position, you should call Position()
   * immediately before calling Zero().
   */
  void Zero(void);

  /**
   * Reads the current position of the motor and returns it as an
   * unscaled value.  This is ideal for use when the position does
   * not need to be translated into an exact rotation count (e.g.,
   * keeping two motors in sync).
   *
   * @param pos Pointer to a long int to hold the position value.
   *
   * @return VI2C_SUCCESS is returned if pos is successfully updated.
   * 	Otherwise, a suitable VI2C_ERR code is returned.
   */
  int Position(long int *pos);

  /**
   * Reads the angular speed of the motor and returns it as an
   * unscaled value.  This is ideal for use when the speed does
   * not need to be translated into an exact RPM (e.g., keeping
   * two motors running at the same speed).
   *
   * The IME does not report direction, so the speed will
   * always be non-negative regardless of the direction of rotation
   *
   * NOTE: The unscaled speed is actualy a "time delta" value
   * 	which is the reciprical of the speed.
   *
   * @param speed Pointer to an int to hold the speed value.
   * 	0xffff indicates a rotation rate of 0 RPM.
   *
   * @return VI2C_SUCCESS is returned if speed is successfully updated.
   * 	Otherwise, a suitable VI2C_ERR code is returned.
   */
  int Speed(unsigned long int *speed);

  /**
   * Reads the current position of the motor and returns it as a
   * scaled value.
   *
   * @param spos Pointer to a long int to hold the position value.
   *
   * @return VI2C_SUCCESS is returned if spos is successfully updated.
   * 	Otherwise, a suitable VI2C_ERR code is returned.
   */
  int ScaledPosition(long int *spos); 			// Rotation in "brads".

  /**
   * Reads the angular speed of the motor and returns it as a
   * scaled value.
   *
   * The IME does not report direction, so the speed will
   * always be non-negative regardless of the direction of rotation
   *
   * @param sspeed Pointer to an int to hold the speed value, scaled to
   * 	units per second.
   *
   * @return VI2C_SUCCESS is returned if sspeed is successfully updated.
   * 	Otherwise, a suitable VI2C_ERR code is returned.
   */
  int ScaledSpeed(unsigned long int *sspeed);	// RPM in "brads/minute".

  /**
   * This method resets the IME unit, which sets its position to zero.
   * Zero() is the preferred way to accomplish this, since it is handled
   * entirely in the driver without needing to clear the IME hardware iself.
   *
   * @return VI2C_SUCCESS is returned if the encoder is successfully cleared.
   * 	Otherwise, a suitable VI2C_ERR code is returned.
   */
  int Reset(void);

  /**
   * Utility function to print device information to stdout.
   *
   * @return VI2C_SUCCESS is returned if the device info is successfully printed.
   * 	Otherwise, a suitable VI2C_ERR code is returned.
   */
  virtual int PrintDevInfo(void);

  /**
   * See the definition of IProperty::GetProperty for more information
   * on this method.
   *
   * The following properties are supported: <br>
   * 	VIME_PROP_IME_TYPE,		VIME_PROP_INVERT,
   * 	VIME_PROP_SCALE_MUL,	VIME_PROP_SCALE_DIV,
   * 	VIME_PROP_SCALE_SPEED
   */
  virtual int GetProperty(int property, long *value);

  /**
   * See the definition of IProperty::SetProperty for more information
   * on this method.
   *
   * The following properties are supported: <br>
   * 	VIME_PROP_IME_TYPE,		VIME_PROP_INVERT,
   * 	VIME_PROP_SCALE_MUL,	VIME_PROP_SCALE_DIV,
   * 	VIME_PROP_SCALE_SPEED
   */
  virtual int SetProperty(int property, long value);

private:
  long int Offset;
  long int Count;
  long int ScaleMul, ScaleDiv, ScaleSpeed;
  bool Invert;
  int IMEType;
};

#endif // _VEXI2CENCODER_H
