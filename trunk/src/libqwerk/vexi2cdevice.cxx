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

/**
 * @file vexi2cdevice.cxx
 * Implementation of the CVexI2CDevice class, which serves as
 * the base class for all Vex I2C device drivers.
 */

#include "vexi2cdevice.h"
#include "timer.h"

/*
 * The CVexI2CDevice provides a base class for all Vex I2C Device Drivers.
 * It interfaces with the CQEI2CBus class to communicate with attached
 * Vex I2C devices.  It provides initialization, identification, address
 * assignement, and termination control functions.
 * This class is intended to be subclassed to create a device-specific
 * driver (e.g.: CVexI2CEncoder).
 */

/*
 * public functions for direct use by clients of the driver object.
 */

void CVexI2CDevice::ResetAll(int count)	// static class method
{
	unsigned int sid;
	CQEI2CBus *i2c = CQEI2CBus::GetPtr();

	CTimer::tick_t last = CTimer::ticks();

	if (count>0x20)	// Number of legal Vex I2C devices addresses
		count=0x20;

	// Ensure all are unterminated, so the reset reaches everybody
	do {
		sid = i2c->Open();
		i2c->Start(I2C_GENERAL_CALL_ADDR, I2C_WRITE, sid);
		i2c->WriteByte(0x4b);	// Disable terminator
		i2c->Stop();
		i2c->Close(sid);
		last = CTimer::mmetro(2, last);
	} while (count--);

	// Send a Vex Reset command to the General Call address.
	sid = i2c->Open();
	i2c->Start(I2C_GENERAL_CALL_ADDR, I2C_WRITE, sid);
	i2c->WriteByte(0x4e);
	i2c->WriteByte(0xca);
	i2c->WriteByte(0x03);
	i2c->Stop();
	i2c->Close(sid);
	CTimer::mmetro(5, last);
	i2c->Release();
}

// Property accessors
int CVexI2CDevice::GetProperty(int property, long *value)
{
	if (value==NULL)
		return PROP_ERROR;

	switch(property)
	{
		case VI2C_PROP_ADDRESS :		*value = Address;				break;
		case VI2C_PROP_VERSION :		*value = DevInfo.version;		break;
		case VI2C_PROP_TYPE :			*value = DevInfo.dev_type;		break;
		case VI2C_PROP_BOARD_ID :		*value = DevInfo.board_id;		break;
		case VI2C_PROP_STATUS_BYTE :	*value = DevInfo.status_byte;	break;
		case VI2C_PROP_TERM_STATE :		*value = DevInfo.terminator;	break;
		case VI2C_PROP_OVFL_STATE :		*value = DevInfo.overflow;		break;
		case VI2C_PROP_DIAG_STATE :		*value = DevInfo.diag_mode;		break;

		default:
			return PROP_ERROR_NOT_SUPPORTED;
	}

	return PROP_OK;
}

int CVexI2CDevice::SetProperty(int property, long value)
{
	return PROP_ERROR_NOT_SUPPORTED;
}

/*
 * protected functions for use by subclasses (Vex I2C device drivers)
 */

CVexI2CDevice::CVexI2CDevice(unsigned short newaddr, bool term)
{
	Status = VI2C_SUCCESS;
	Address = VEX_I2C_DEV_ADDR;

	// Get reference to I2C Bus
	i2c = CQEI2CBus::GetPtr();
	if (!i2c)
		Status = VI2C_ERR_NO_I2C_BUS;

	if (!Status)
		Status = this->Relocate(newaddr);

	if (!Status)
		Status = this->SetTerminator(term);

	if (!Status)
		Status = this->UpdateDevInfo();
}

CVexI2CDevice::~CVexI2CDevice()
{
	i2c->Release();
}

int CVexI2CDevice::UpdateDevInfo(void)
{
	unsigned int sid = i2c->Open();
	i2c->Start(Address, I2C_WRITE, sid);
	i2c->WriteByte(0x20);	// Select status byte
	i2c->Start(Address, I2C_READ, sid);
	DevInfo.dev_info_long = i2c->ReadLong(I2C_LSB_FIRST, I2C_DONE);
	i2c->Stop();
	i2c->Close(sid);

	return Status;
}

int CVexI2CDevice::PrintDevInfo(void)
{
	char string[25];

	printf("VEX I2C Device Info:\n"
		   "\tI2C Addr  = 0x%02x\n", Address);

	if (Status) {
		printf("\tError (%d) = ", Status);
		switch (Status) {
			case VI2C_ERR_NO_I2C_BUS :		printf("I2C Bus not accessible!\n");		break;
			case VI2C_ERR_NO_I2C_DEV :		printf("I2C Device not responding!\n");		break;
			case VI2C_ERR_INVALID_ADDR :	printf("I2C Address not valid!\n");			break;
			case VI2C_ERR_ADDR_IN_USE :		printf("I2C Address already in use!\n");	break;
			case VI2C_ERR_NOT_VEX_DEV :		printf("Not VEX I2C dev!\n");				break;
			case VI2C_ERR_WRONG_DEV_TYPE :	printf("Incorrect driver for device!\n");	break;
			case VI2C_ERR_RELOC_FAILED :	printf("Address reassignment failed!\n");	break;
			case VI2C_ERR_NAK_RECEIVED :	printf("Device replied with NAK!\n");		break;
			default :						printf("Unknown code!\n");					break;
		}
	}

	if (!Status) {
		UpdateDevInfo();

		unsigned int sid = i2c->Open();
		i2c->Start(Address, I2C_WRITE, sid);
		i2c->WriteByte(0x00);	// Select ASCII fields
		i2c->Start(Address, I2C_READ, sid);
		i2c->ReadData((unsigned char *)string, 24, I2C_DONE);
		i2c->Stop();
		i2c->Close(sid);

		string[24]='\0';	// Null terminate 3rd string (Dev ID)
		printf("\tDevice ID = %s\t\tDev Type = %d\n", string+16, DevInfo.dev_type);
		string[16]='\0';	// Null terminate 2nd string (Vendor)
		printf("\tVendor    = %s\t\tBoard ID = %d\n", string+8, DevInfo.board_id);
		string[8]='\0';	// Null terminate 1st string (Version)
		printf("\tVersion   = %s\t\tVersion  = %d\n", string, DevInfo.version);
		printf("\tStat Byte = 0x%02x", DevInfo.status_byte);
		if (DevInfo.terminator) printf(" (terminated)");
		if (DevInfo.overflow) printf(" (overflow)");
		if (DevInfo.diag_mode) printf(" (diagnostic mode)");
		printf("\n");
	}
    return Status;
}

/**
 * private functions for use only during device initialization
 */

int CVexI2CDevice::Relocate(unsigned short newaddr)
{
	// Make sure address is in valid range
	if ((newaddr<0x10) || (newaddr>=0x30))
		return VI2C_ERR_INVALID_ADDR;

	// Make sure target address is not already in use
	if (i2c->Probe(newaddr))
		return VI2C_ERR_ADDR_IN_USE;

	// Verify device is present before the move
	if (!i2c->Probe(Address))
		Status = VI2C_ERR_NO_I2C_DEV;

	// Attempt the relocation
	unsigned int sid = i2c->Open();
	i2c->Start(Address, I2C_WRITE, sid);
	i2c->WriteByte(0x4d);		// Set new address
	i2c->WriteByte(newaddr<<1);
	i2c->Stop();
	i2c->Close(sid);
	Address = newaddr;

	CTimer::msleep(1);	// Pause after setting address

	// Verify device is present after the move
	if (!i2c->Probe(Address))
		Status = VI2C_ERR_RELOC_FAILED;


	return Status;
}

int CVexI2CDevice::SetTerminator(bool term)
{
	unsigned int sid = i2c->Open();
	i2c->Start(Address, I2C_WRITE, sid);
	bool ack = i2c->WriteByte(term ? 0x4c : 0x4b);	// Set terminator
	i2c->Stop();
	i2c->Close(sid);

	CTimer::msleep(2);	// Pause after setting terminator
	return (ack ? Status : VI2C_ERR_NAK_RECEIVED);
}
