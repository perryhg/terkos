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
 * @file qei2cbus.cxx
 * Implementation of the CQEI2CBus class, which provides
 * support for the VEXpro I2C port.
 *
 * @todo The following optional features are under consideration: <br>
 *  10-bit addressing,
 *  General Call,
 *  Software Reset,
 *  START Byte <br>
 * These features would require reworking the FPGA image: <br>
 *  Multi-master,
 *  Slave mode,
 *  Arbitration,
 *  Synchronization,
 *  Fast-mode (400KHz),
 *  Fast-mode Plus (1MHz)
 */
 
#include "qei2cbus.h"
#include "timer.h"

#ifndef DOXYGEN_IGNORED

#define I2C_SDA		(0x01)
#define I2C_SCL		(0x02)
#define I2C_DDR		(0x04)

SINGLETON_REGISTER(CQEI2CBus);

// I2C_SCL macros are internal helpers that set the FPGA's
// I2C control register.  They are also responsible for enforcing
// minimum transition times and implementing clock stretching.

#define I2C_SCL_High(sda) {																		\
	*m_i2c_reg = (sda&I2C_SDA) | I2C_SCL | I2C_DDR;	/* SDA is open-drain, leave as output */	\
	while ( !(I2C_SCL & *m_i2c_reg) ) ; /* empty loop (clock stretching) */						\
	CTimer::usleep(SCL_MinUsecHigh);															\
}

#define I2C_SCL_Low(sda) {																		\
	*m_i2c_reg = (sda&I2C_SDA) | I2C_DDR; /* SDA is open-drain, leave as output */				\
	CTimer::usleep(SCL_MinUsecLow);																\
}

#define I2C_SCL_Low_Setup_SDA(sda) {															\
	*m_i2c_reg = (sda&I2C_SDA) | I2C_DDR; /* SDA is open-drain, leave as output */				\
	CTimer::usleep(1UL);																		\
}


// BitBang is an internal helper that sends a bit,
// and is also used to read one bit.

#define I2C_Tx(b) {																				\
	I2C_SCL_Low_Setup_SDA(b);		/* SDA goes high, SCL drives low - 1us delay */				\
	I2C_SCL_High(b);				/* SDA stays high, SCL goes high */							\
	I2C_SCL_Low(b)     				/* SDA stays high, SCL returns low */						\
}

#define I2C_Rx(b) {																				\
	I2C_SCL_Low_Setup_SDA(I2C_SDA);	/* SDA goes high, SCL drives low - 1us delay */				\
	I2C_SCL_High(I2C_SDA);			/* SDA stays high, SCL goes high */							\
	b = I2C_SDA & *m_i2c_reg;		/* Read SDA */												\
	I2C_SCL_Low(I2C_SDA);			/* SDA stays high, SCL returns low */						\
}

#endif // DOXYGEN_IGNORED

unsigned int CQEI2CBus::Open(bool wait)
{
	if (wait) {
		pthread_mutex_lock(&bus_mutex); // Acquire the bus mutex (may block)
	} else {
		if (pthread_mutex_trylock(&bus_mutex)) // Acquire the bus mutex
			return 0;	// lock acquisition failed
	}

	// We now hold the bus_mutex
	SessionActive=true;
	do {
		SessionID += 409;	// Randomly chosen prime number
	} while (!SessionID);	// Ensure 0 isn't returned

	return SessionID;
}

bool CQEI2CBus::Close(unsigned int session)
{
    if (!SessionActive)
        return true;
    
	if (SessionID == session) {
		SessionActive = false;
		pthread_mutex_unlock(&bus_mutex); // wake any clients sleeping in Open()
        return true;
	}
    return false;
}


// Start or re-start an I2C packet.
// 7-bit target device address is provided by "addr" parameter.
// Data direction is specified by the "read" parameter.
// Returns true if address was acknowleged and false otherwise.
bool CQEI2CBus::Start(unsigned short addr, bool read, unsigned int session)
{
	if (!SessionActive)
		return false;

	if (SessionID != session)
		return false;

	if (addr >= 0x80)		// Only 7b addressing for now
		return false;

    // Ensure the bus is idle
	I2C_SCL_High( 1 ); // SDA is input, SCL is high
	while ( (*m_i2c_reg & (I2C_SDA | I2C_SCL)) != (I2C_SDA | I2C_SCL) ) {
		; // Empty Wait Loop
	}

	I2C_SCL_High(0);	// SDA goes low, SCL stays high
	I2C_SCL_Low(0);		// SDA stays low, SCL goes low

	unsigned char data = addr<<1 | read;
    return this->WriteByte(data);
}

// Call once at the end of each I2C "packet".  Note that there may be
// multiple successive calls to I2CStart prior to a single call to I2CStop,
// but there should not be multiple successive calls to I2CStop.
void CQEI2CBus::Stop(void)
{
	if (!SessionActive)
		return;

	I2C_SCL_Low(0);		// SDA is low, SCL is low
	I2C_SCL_High(0);	// SDA stays low, SCL goes high
	I2C_SCL_High(1);	// SDA goes high, SCL stays high
}


// Write 1 byte to the I2C bus
bool CQEI2CBus::WriteByte(unsigned char value)
{
	if (!SessionActive)
		return false;

	register unsigned char bit = 0x80;

	while (bit) {
		I2C_Tx((value&bit)!=0);
		bit >>= 1;
	}
	bool ack;
	I2C_Rx(ack);	// read ack bit
	return !ack;
}


// Write 2 bytes to the I2C bus
bool CQEI2CBus::WriteWord(unsigned short value, bool order)
{
	if (!SessionActive)
		return false;

	if (!this->WriteByte( (unsigned short)(value) >> (order?8:0) ))
		return false;
	if (!this->WriteByte( (unsigned short)(value) >> (order?0:8) ))
		return false;
	return true;
}


// Write 4 bytes to the I2C bus
bool CQEI2CBus::WriteLong(unsigned long value, bool order)
{
	if (!SessionActive)
		return false;

	if (!this->WriteByte( (unsigned long)(value) >> (order?24:0) ))
		return false;
	if (!this->WriteByte( (unsigned long)(value) >> (order?16:8) ))
		return false;
	if (!this->WriteByte( (unsigned long)(value) >> (order?8:16) ))
		return false;
	if (!this->WriteByte( (unsigned long)(value) >> (order?0:24) ))
		return false;
	return true;
}

// Read variable number of bytes from the I2C bus
unsigned short CQEI2CBus::WriteData(void *buffer, unsigned short count)
{
	if (!SessionActive)
		return false;

	bool result;
	unsigned char* buf = (unsigned char *)buffer;
	
    do {
		result = (this->WriteByte(*buf++));
	} while (result && (count--));
    
	return count;
}

// Read 1 byte from the I2C bus
unsigned char CQEI2CBus::ReadByte(bool ack)
{
	if (!SessionActive)
		return 0;

	register unsigned char bit = 0x80, byte = 0x00;

	while (bit) {
		bool rx;
		I2C_Rx(rx)
		if (rx)
			byte |= bit;
		bit >>= 1;
	}
	I2C_Tx(!ack)
	return byte;
}


// Read 2 bytes from the I2C bus
unsigned short CQEI2CBus::ReadWord(bool order, bool ack)
{
	if (!SessionActive)
		return 0;

	unsigned short value;
	value  = (unsigned short)(this->ReadByte(I2C_READ)) << (order?8:0);
	value |= (unsigned short)(this->ReadByte(ack))      << (order?0:8);
	return value;
}


// Read 4 bytes from the I2C bus
unsigned long CQEI2CBus::ReadLong(bool order, bool ack)
{
	if (!SessionActive)
		return 0;

	unsigned long value;
	value  = (unsigned long)(this->ReadByte(I2C_READ)) << (order?24:0);
	value |= (unsigned long)(this->ReadByte(I2C_READ)) << (order?16:8);
	value |= (unsigned long)(this->ReadByte(I2C_READ)) << (order?8:16);
	value |= (unsigned long)(this->ReadByte(ack))      << (order?0:24);
	return value;
}

// Read variable number of bytes from the I2C bus
void CQEI2CBus::ReadData(void *buffer, unsigned short count, bool ack)
{
	unsigned char* buf = (unsigned char *)buffer;
	while (count--) {
		*buf++ = (this->ReadByte(count?I2C_READ:ack));
	}
}



// Handy functions to probe addresses on the bus.
// Probe() checks a single address and returns true/false.
//
// Note:  This function calls Open() so it shold not be called from a
// driver or thread that already has the bus open.
bool CQEI2CBus::Probe(unsigned short addr)
{
	unsigned int sid = this->Open();
	bool ack = this->Start(addr, I2C_WRITE, sid);
	this->Stop();
	this->Close(sid);
	return ack;
}

// Scan() probes all the addresses in the specified range.
// Prints a line to stdout for each device and returns the total number
// of devices discovered.
//
// Note:  This function calls Open() so it shold not be called from a
// driver or thread that already has the bus open.
short CQEI2CBus::Scan(unsigned short min, unsigned short max)
{
	unsigned short addr, count=0;

	printf("\nScanning I2C Bus range 0x%02x..0x%02x\n", min, max);
	for (addr=min; addr<=max; addr++) {
		if (this->Probe(addr)) {
			printf("\tFound device at 0x%02x (%d)\n", addr, addr);
			count++;
		}
	}
	printf("Found %d devices\n", count);
	return count;
}


// Property accessors
int CQEI2CBus::GetProperty(int property, long *value)
{
	if (value==NULL)
		return PROP_ERROR;

	switch(property)
	{
		case QI2C_PROP_SCL_MIN_USEC_LOW:
			*value = SCL_MinUsecLow;
			break;

		case QI2C_PROP_SCL_MIN_USEC_HIGH:
			*value = SCL_MinUsecHigh;
			break;

		default:
			return PROP_ERROR_NOT_SUPPORTED;
	}

	return PROP_OK;
}

int CQEI2CBus::SetProperty(int property, long value)
{
	switch(property)
	{
		case QI2C_PROP_SCL_MIN_USEC_LOW:
			SCL_MinUsecLow = value;
			break;

		case QI2C_PROP_SCL_MIN_USEC_HIGH:
			SCL_MinUsecHigh = value;
			break;

		default:
			return PROP_ERROR_NOT_SUPPORTED;
	}

	return PROP_OK;
}


// Constructor & destructor
CQEI2CBus::CQEI2CBus()
{
	// initialize and set mutex as normal
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_FAST_NP);
	pthread_mutex_init(&bus_mutex, &mattr);

	SessionID = 0;
	SessionActive = true;
	SCL_MinUsecLow = 5UL;
	SCL_MinUsecHigh = 5UL;

	m_p9302hw = C9302Hardware::GetPtr();

    if (m_p9302hw->GetBitstreamMajorVersion()!=0xa0)
        throw std::runtime_error("wrong FPGA bitstream version");

    m_i2c_reg = m_p9302hw->m_fpga.Ushort(0x480);	// get a pointer to the I2C register

    I2C_SCL_High(1);            					// SDA is input, SCL is high
    this->ReadByte();								// Clock in one byte just to clear the bus
    this->Stop();
    this->Close(SessionID);
}

CQEI2CBus::~CQEI2CBus()
{
    I2C_SCL_High(1);            					// SDA is input, SCL is high
	C9302Hardware::Release();
}
