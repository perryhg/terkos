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
 * @file vexi2cdevice.h
 * Interface for the CVexI2CDevice class, which serves as
 * the base class for all Vex I2C device drivers.
 */

#ifndef _VEXI2CDEVICE_H
#define _VEXI2CDEVICE_H


#include "property.h"
#include "qei2cbus.h"


#define VEX_I2C_DEV_ADDR	(0x30)

// Identifiers for GetProperty() and SetProperty()
#define VI2C_OBJECT				5
#define VI2C_PROP_ADDRESS		PROP_ID(VI2C_OBJECT, 0)
#define VI2C_PROP_VERSION		PROP_ID(VI2C_OBJECT, 1)
#define VI2C_PROP_TYPE			PROP_ID(VI2C_OBJECT, 2)
#define VI2C_PROP_BOARD_ID		PROP_ID(VI2C_OBJECT, 3)
#define VI2C_PROP_STATUS_BYTE	PROP_ID(VI2C_OBJECT, 4)
#define VI2C_PROP_TERM_STATE	PROP_ID(VI2C_OBJECT, 5)
#define VI2C_PROP_OVFL_STATE	PROP_ID(VI2C_OBJECT, 6)
#define VI2C_PROP_DIAG_STATE	PROP_ID(VI2C_OBJECT, 7)

// Return codes for all functions other than the constructor & destructor
#define VI2C_SUCCESS		 	(0)
#define VI2C_ERR_NO_I2C_BUS		(1)
#define VI2C_ERR_NO_I2C_DEV		(2)
#define VI2C_ERR_INVALID_ADDR	(3)
#define VI2C_ERR_ADDR_IN_USE	(4)
#define VI2C_ERR_NOT_VEX_DEV	(5)
#define VI2C_ERR_WRONG_DEV_TYPE	(6)
#define VI2C_ERR_RELOC_FAILED	(7)
#define VI2C_ERR_NAK_RECEIVED	(8)
#define VI2C_ERR_BAD_PARAMETER	(9)

/**
 * Provides a base class for all Vex I2C Device Drivers.
 * It interfaces with the CQEI2CBus class to communicate with attached
 * Vex I2C devices.  It provides initialization, identification, address
 * assignement, and termination control functions.
 * This class is intended to be subclassed to create a device-specific
 * driver (e.g.: CVexI2CEncoder).  Most of the elements of this class
 * are delared "protected" because the are for use only by device driver
 * subclasses.
 */
class CVexI2CDevice : public IProperty
{
public:
  /**
   * Function for resetting all attached Vex I2C devices.  This function
   * is static, so it is generally called immediately before instances of
   * this class are created.
   * @param count The number of Vex I2C devices chained on the bus.  This
   * must be greater than or equal to the actual number of Vex I2C devices
   * to ensure that all devices in the chain are properly reset.
   * The default is 8, since that is the maximum chain length recommended
   * by IFI.
   */
  static void ResetAll(int count=8);

  /**
   * See the definition of IProperty::GetProperty for more information
   * on this method.  The following properties are supported:
   * 	VI2C_PROP_VERSION,		VI2C_PROP_TYPE,			VI2C_PROP_BOARD_ID,
   * 	VI2C_PROP_STATUS_BYTE,	VI2C_PROP_TERM_STATE,
   * 	VI2C_PROP_OVFL_STATE,	VI2C_PROP_DIAG_STATE
   */
  virtual int GetProperty(int property, long *value);

  /**
   * See the definition of IProperty::SetProperty for more information
   * on this method.
   */
  virtual int SetProperty(int property, long value);

// Symbols for use by device drivers.  The following symbols are marked
// "protected" because the are for use only by device drivers (subclasses).
protected:
  /**
   * Constructor for the Vex I2C device driver parent class.
   *
   * @param newaddr The I2C address to be assigned to this device.
   *    The address must be in the range 0x10..0x2f
   *
   * @param term Termination control.  This should be set to true for
   *    the last VEX I2C device chained on the bus, and false for all others.
   */
  CVexI2CDevice(unsigned short newaddr, bool term=true);

  /**
   * Destructor for the Vex I2C device driver parent class.
   */
  virtual ~CVexI2CDevice();

  /**
   * Function to re-read device identification and status details.
   * This function is intended for use only by device drivers (subclasses).
   * Subclasses should overload this function if device-specific data needs
   * to be updated at the same time.
   *
   * @return VI2C_SUCCESS is returned if the device info is successfully updated.
   *    Otherwise, a suitable VI2C_ERR code is returned.
   */
  virtual int UpdateDevInfo(void);

  /**
   * Utility function to print device information to stdout.
   * Subclasses should overload this function with a public implementation,
   * printting any additional device-specific data after the call to this class.
   *
   * This function is not used internally by this class, and is
   * provided primarily as a diagnostic tool for use during
   * developlement and testing.
   *
   * @return VI2C_SUCCESS is returned if the device info is successfully printed.
   *    Otherwise, a suitable VI2C_ERR code is returned.
   */
  virtual int PrintDevInfo(void);

#ifndef DOXYGEN_IGNORED
  typedef union {
	struct {
	  unsigned char version;
	  unsigned char dev_type;
	  unsigned char board_id;
	  union {
		struct {
		  unsigned char terminator:1;
		  unsigned char overflow:1;
		  unsigned char diag_mode:1;
		  unsigned char reserved:5;
		};
		unsigned char status_byte;
	  };
	};
	unsigned long dev_info_long;
  } dev_info_t;
#endif

  /// A reference to the singleton I2C bus
  CQEI2CBus *i2c;
  
  /// The I2C address for this Vex device
  unsigned short Address;
  
  /// Driver status (see VI2CD_ERR defines)
  unsigned int Status;
  
  /// Device identification record as provided by the device itself
  dev_info_t DevInfo;

private:
  /**
   * Function controling device termination.  Termination is only changed
   * during device initialization, since it can cause chained I2C devices
   * to disconnect from the bus.
   *
   * @param term Termination setting:
   *    <true> to enable termination and disconnect downstream chained devices.
   *    <false> to disable termination and connect downstream chained devices.
   *
   * @return VI2C_SUCCESS is returned if termination is successfully set.
   *    Otherwise, a suitable VI2_ERR_ code is returned.
   */
  int SetTerminator(bool term);

  /**
   * Vex I2C device chaining requires that devices be assigned a new address
   * when the are initialized.  This function implements the process of
   * reconfiguring the device to its new address, as well as some rudimentary
   * checks to preserve the integrity of the bus.
   *
   * @param newaddr The I2C address to be assigned to this device.
   *    The address must be in the range 0x10..0x2f.
   *
   *    @return VI2C_SUCCESS is returned if the device is succesfully relocated.
   * Otherwise, a suitable VI2C_ERR code is returned.
   */
  int Relocate(unsigned short newaddr);
};

#endif // _VEXI2CDEVICE_H
