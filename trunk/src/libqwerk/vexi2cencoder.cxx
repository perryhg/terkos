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
 * @file vexi2cencoder.cxx
 * Implementation of the CVexI2CEncoder class, which serves as
 * the device driver for Vex Integrated Moter Encoders (IME).
 */

#include "vexi2cencoder.h"


int CVexI2CEncoder::Position(long int *pos)
{
	union {
		long int count;
		unsigned char byte[4];
	};

	if (!pos)
		return VI2C_ERR_BAD_PARAMETER;

	count = 0x80000000;
	unsigned int sid = i2c->Open();				// <=== Open
	i2c->Start(Address, I2C_WRITE, sid);
	bool ack = i2c->WriteByte(0x40);
	if (ack) {
		i2c->Start(Address, I2C_READ, sid);
		byte[1] = i2c->ReadByte(I2C_READ);
		byte[0] = i2c->ReadByte(I2C_READ);
		byte[3] = i2c->ReadByte(I2C_READ);
		byte[2] = i2c->ReadByte(I2C_DONE);
	}
	i2c->Stop();
	i2c->Close(sid);							// <=== Close

	if (!ack)
		return VI2C_ERR_NAK_RECEIVED;

	if (Invert)
		count = -count;

	*pos = count-Offset;
	Count = count;

	return VI2C_SUCCESS;
}

int CVexI2CEncoder::Speed(unsigned long int *speed)
{
	if (!speed)
		return VI2C_ERR_BAD_PARAMETER;

	unsigned short int delta = 0xffff;
	unsigned int sid = i2c->Open();				// <=== Open
	i2c->Start(Address, I2C_WRITE, sid);
	bool ack = i2c->WriteByte(0x44);
	if (ack) {
		i2c->Start(Address, I2C_READ, sid);
		delta = i2c->ReadWord(I2C_MSB_FIRST, I2C_READ);
	}
	i2c->Stop();
	i2c->Close(sid);							// <=== Close

	if (!ack)
		return VI2C_ERR_NAK_RECEIVED;

	*speed = (unsigned long int) delta;

	return VI2C_SUCCESS;
}

void CVexI2CEncoder::Zero(void)
{
	Offset = -Count;
}

int CVexI2CEncoder::ScaledPosition(long int *spos)
{
	if (!Status) {
		long int pos;
		int err = Position(&pos);
		if (err)
			return err;

		long long int intermediate = pos; // 64b
		intermediate *= (long long int) ScaleMul;
		intermediate /= (long long int) ScaleDiv;

		*spos = (long int) intermediate;
	}
	return Status;
}

int CVexI2CEncoder::ScaledSpeed(unsigned long int *sspeed)
{
	unsigned long int speed;
	if (!Status) {
		int err = Speed(&speed);
		if (err)
			return err;
	}
	if ((speed >= 0xffff) || (speed == 0))
		*sspeed = 0;
	else
		*sspeed = ScaleSpeed / speed;

	return Status;
}


int CVexI2CEncoder::Reset(void)
{
	unsigned int sid = i2c->Open();				// <=== Open
	i2c->Start(Address, I2C_WRITE, sid);
	bool ack = i2c->WriteByte(0x4a);
	i2c->Stop();
	i2c->Close(sid);							// <=== Close

	if (!ack)
		return VI2C_ERR_NAK_RECEIVED;

	Offset = 0;
	Count = 0;

	return VI2C_SUCCESS;
}

int CVexI2CEncoder::PrintDevInfo(void)
{
	CVexI2CDevice::PrintDevInfo();

	if (!Status) {
		UpdateDevInfo();

		printf("\tCount     = %ld\t\t\tGear Mul = %ld\n", Count, ScaleMul);
		printf("\tOffset    = %ld\t\t\tGear Div = %ld\n", Offset, ScaleDiv);
		switch (IMEType) {
			case VIME_TYPE_269 : 		printf("\tIME Type  = (1) IME269\n"); 			break;
			case VIME_TYPE_393_TORQUE : printf("\tIME Type  = (2) IME393 (Torque)\n");	break;
			case VIME_TYPE_393_SPEED : 	printf("\tIME Type  = (3) IME393 (Speed)\n");	break;
		}
	}
    return Status;
}

CVexI2CEncoder::CVexI2CEncoder(int ime_type, bool invert, unsigned short newaddr, bool term)
	: CVexI2CDevice(newaddr,term)	// Start by envoking superclass constructor
{
	if (Status)	// superclass constructor reported an error
		return;

	// This class makes use of the overflow semantics of two's-complement integer
	// math.  In order for it to work as expected, the int types must be specific
	// sizes, as enforced by the following init-time checks:

	if ( sizeof(short int) != 2 ) // Size of short int must be 2.
		Status = VI2C_ERR_BAD_PARAMETER;

	if ( sizeof(long int) != 4 ) // Size of long int must be 4.
		Status = VI2C_ERR_BAD_PARAMETER;

	if ( sizeof(long long int) != 8 ) // Size of long long int must be 8.
		Status = VI2C_ERR_BAD_PARAMETER;

	if (Status)	// one or more types are the wrong size!
		return;

	if (DevInfo.dev_type != 1) { // Not an IME
		Status = VI2C_ERR_WRONG_DEV_TYPE;
		return;
	}

	if (!Status)
		Status = SetProperty(VIME_PROP_INVERT, invert);

	if (!Status)
		Status = SetProperty(VIME_PROP_IME_TYPE, ime_type);

	Reset();
	Zero();
}

CVexI2CEncoder::~CVexI2CEncoder()
{
}

// Property accessors
int CVexI2CEncoder::GetProperty(int property, long *value)
{
	if (value==NULL)
		return PROP_ERROR;

	switch(property) {
	  case VIME_PROP_IME_TYPE :
		*value = IMEType;
		break;

	  case VIME_PROP_INVERT :
		*value = Invert;
		break;

	  case VIME_PROP_SCALE_MUL :
		*value = ScaleMul;
		break;

	  case VIME_PROP_SCALE_DIV :
		*value = ScaleDiv;
		break;

	  case VIME_PROP_SCALE_SPEED :
		*value = ScaleSpeed;
		break;

	  default:	// Give parent class an opportunity...
		return CVexI2CDevice::GetProperty(property, value);
	}

	return PROP_OK;
}

int CVexI2CEncoder::SetProperty(int property, long value)
{
	switch(property)
	{
	  case VIME_PROP_IME_TYPE :
		{
			switch (value) {
			  case VIME_TYPE_269 : 		// 240.448 counts/rev
				if (DevInfo.board_id != 2)
					return VI2C_ERR_WRONG_DEV_TYPE;
				ScaleMul = 4000;
				ScaleDiv = 3757;  	// 13*17*17
				ScaleSpeed = 7985094;	// 750000 * 4000 / 3757

				break;

			  case VIME_TYPE_393_TORQUE : 	// 627.2 counts/rev
				if (DevInfo.board_id != 3)
					return VI2C_ERR_WRONG_DEV_TYPE;
				ScaleMul = 20;
				ScaleDiv = 49;
				ScaleSpeed = 3061224;	// 750000 * 20 / 49;
				break;

			  case VIME_TYPE_393_SPEED : 	// 392.0 counts/rev
				if (DevInfo.board_id != 3)
					return VI2C_ERR_WRONG_DEV_TYPE;
				ScaleMul = 32;
				ScaleDiv = 49;
				ScaleSpeed = 4897959;	// (750000 * 32) / 49;
				break;

			  default :
				return VI2C_ERR_BAD_PARAMETER;
			}
			IMEType = value;
		}
		break;

	  case VIME_PROP_INVERT :
		Invert = value ? 1 : 0;
		break;

	  case VIME_PROP_SCALE_MUL :
		if (value == 0)
			return VI2C_ERR_BAD_PARAMETER;
		ScaleMul = value;
		break;

	  case VIME_PROP_SCALE_DIV :
		if (value == 0)
			return VI2C_ERR_BAD_PARAMETER;
		ScaleDiv = value;
		break;

	  case VIME_PROP_SCALE_SPEED :
		if (value == 0)
			return VI2C_ERR_BAD_PARAMETER;
		ScaleSpeed = value;
		break;

	  default:	// Give parent class an opportunity...
			return CVexI2CDevice::SetProperty(property, value);
	}

	return PROP_OK;
}
