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
 * @file qei2cbus.h
 * Interface for the CQEI2CBus class, which provides
 * support for the VEXpro I2C port.
 */

#ifndef _QEI2CBUS_H
#define _QEI2CBUS_H


#include <pthread.h>
#include "property.h"
#include "singleton.h"
#include "9302hw.h"


#define I2C_MSB_FIRST					(true)
#define I2C_LSB_FIRST					(false)

#define I2C_READ						(true)
#define I2C_WRITE						(false)
#define I2C_DONE						(false)

#define I2C_GENERAL_CALL_ADDR			(0)

#define QI2C_OBJECT						4
#define QI2C_PROP_SCL_MIN_USEC_LOW		PROP_ID(QI2C_OBJECT, 0)
#define QI2C_PROP_SCL_MIN_USEC_HIGH		PROP_ID(QI2C_OBJECT, 1)

/**
 * CQEI2CBus Provides support for the VEXpro I2C port.
 *
 * Typically, this class is used by I2C device driver objects,
 * but it possible for programs to use this class to communicate
 * directly with devices on the I2C bus.
 *
 * This implementation upports all mandatory features of the I2C
 * Bus Spec Rev 03:
 *  Standard-mode (<=100KHz ... achieves about 70KHz)
 *  Single master
 *  7-bit addressing
 *  Clock stretching
 */
 
class CQEI2CBus : public IProperty
{
public:
  /**
   * This internal macro handles instantiation of this class.
   */
  SINGLETON(CQEI2CBus);


  /**
   * Gain exclusive access to the I2C bus.
   *
   * Open() MUST be called, and must return a non-zero session id prior
   * to use of any other bus access methods.  The session id must be
   * provided to every call to Start(), as well as Close() after
   * completing each unit of I2C bus traffic.
   *
   * This is done to ensure drivers sharing the bus do not issue
   * overlapping I/O.  Only the driver that has the bus open will be
   * able to perform I/O.
   *
   * @param wait indicates if the caller is willing to wait
   *    for the bus to become available.  If true is specified, Open()
   *    will wait until the bus is available.  If false is specified,
   *    Open() will fail and return 0 immediately if the bus is busy.
   *
   * @return This function returns the session id, or 0 if the I2C
   *    bus is busy.  This session id is passed as an argument to
   *    subsequent calls of Start() and Close().
   */
  unsigned int Open(bool wait=true);


  /**
   * Release exclusive access of the I2C bus.
   *
   * After completing I/O to the I2C bus, Close() MUST be called to
   * release control and allow other device drivers access.  Until
   * the active session closed, no other calls to Open() will succeed.
   *
   * The Open()/Close() function pair is used to ensure device drivers
   * running on different threads do not interfere with each other by
   * overlapping I2C bus traffic.  A responsibly designed driver will
   * Open() the bus, perform a single bus operation, and then Close()
   * the bus immediately when it is safe for other traffic to occur.
   * It is NOT acceptable for a driver to Open() the bus and leave it
   * open indefinitely, since no other driver will ever gain access to
   * the bus.
   *
   * @param session The current session id provided by Open().
   *
   * @return This function returns false if the specified session id does
   *    not match the last id returned by Open().  If this function returns
   *    true, then the session was successfully closed.
   */
  bool Close(unsigned int session);


  /**
   * Call I2CStart to begin a packet, or any time you need to change
   * the direction of I/O within a packet.  It is common to see multiple
   * calls to I2CStart with one single call to I2CStop at the end of
   * the packet.
   *
   * @param addr The 7-bit I2C address of the target device, which must
   * 	be in the range of 0..127.  If you are having trouble selecting
   *    the device, or if the device documentation provides an oven/odd
   *    address pair for reading and writing, try dividing the address by 2.
   *
   * @param read Specifies the direction of the transfer.
   * 	If you specify I2C_READ, you should only call I2CRead* functions
   * 	until you change the direction with another call to I2CStart().
   * 	If you specify I2C_WRITE, you should only call I2CWrite* functions
   * 	until you change the direction with another call to I2CStart().
   *
   * @param session The session id returned from Open().  If the session id
   * 	id does not match the current session, then the start operation will
   * 	not proceed and Start() will return false.  The session id will
   * 	remain valid until Close() is called, so you may perform multiple
   * 	start/stop operations without interference from other drivers.
   *
   * @return  This function will return true if the target device
   * 	acknowledged the bus start, or false if otherwise.
   */
  bool Start(unsigned short addr, bool read, unsigned int session);


  /**
   * Call once at the end of each I2C bus operation.  Note that there may be
   * multiple successive calls to Start() prior to a single call to Stop(),
   * but there should not be multiple successive calls to Stop().
   * See the documentation for the I2C device in question to determine the
   * appropriate points to call Stop().
   */
  void Stop(void);
  
  
  /**
   * Write 1 byte to the I2C device.  This function should only be called
   * after a successful call to Start() with the I2C_WRITE option specified.
   * Bits are always sent most-significant-bit first.
   *
   * @param value The byte to be written to the I2C device.
   *
   * @return Returns true if the I2C device acknowledged the write, and
   *    false if the device did not.
   */
  bool WriteByte(unsigned char value);


  /**
   * Write a word (2 bytes) to the I2C device.  This function should only
   * be called after a successful call to Start() with the I2C_WRITE option
   * specified.
   *
   * @param value The word to be written to the I2C device.
   *
   * @param order This specifies the order that the bytes are sent on the
   *    bus; either I2C_MSB_FIRST or I2C_LSB_FIRST.
   *    Bits are always sent most-significant-bit first.
   *
   * @return Returns true if the I2C device acknowledged both bytes of the
   *    write operation, and returns false if either byte was not acknowledged.
   *    If the first byte is not acknowledged, then the second byte is not sent.
   */
  bool WriteWord(unsigned short value, bool order);


  /**
   * Write a long (4 bytes) to the I2C device.  This function should only
   * be called after a successful call to Start() with the I2C_WRITE option
   * specified.
   *
   * @param value The long to be written to the I2C device.
   *
   * @param order This specifies the order that the bytes are sent on the
   *    bus; either I2C_MSB_FIRST or I2C_LSB_FIRST.
   *    Bits are always sent most-significant-bit first.
   *
   * @return Returns true if the I2C device acknowledged all 4 bytes of the
   *    write operation, and returns false if any byte was not acknowledged.
   *    If any byte is not acknowledged, then the remaining bytes are not sent.
   */
  bool WriteLong(unsigned long value, bool order);


  /**
   * Write a variable number of bytes to the I2C device.  This function
   * should only be called after a successful call to Start() with the
   * I2C_WRITE option specified.
   *
   * Bytes are sent in the order the are present in the buffer (address 0
   * is sent first, then address 1, etc).  Bits are always sent
   * most-significant-bit first.
   *
   * @param buffer A pointer to an array of bytes to be sent.  The buffer
   *    must be at least as large as the count parameter.
   *
   * @param count This specifies the number of bytes to transmit.
   *
   * @return If all bytes are acknowledged by the I2C device, then 0 is
   *    returned.  If any byte was not acknowledged, then no further bytes
   *    are sent and the number of unsent bytes is returned.
   */
  unsigned short WriteData(void *buffer, unsigned short count);


  /**
   * Read 1 byte from the I2C device.  This function should only be called
   * after a successful call to Start() with the I2C_READ option specified.
   * Bits are always received most-significant-bit first.
   *
   * @param ack Indicates if the byte received should be acknowledged.
   *    The byte is acknowledged if ack is I2C_READ (true), and is nak'd if
   *    I2C_DONE (false).
   *
   * @return The byte read from the I2C device.
   */
  unsigned char  ReadByte(bool ack=I2C_READ);


  /**
   * Read a word (2 bytes) from the I2C device.  This function should only
   * be called after a successful call to Start() with the I2C_READ option
   * specified.  Bits are always received most-significant-bit first.
   *
   * @param order This specifies the order that the bytes are read from the
   *    bus; either I2C_MSB_FIRST or I2C_LSB_FIRST.
   *    Bits are always received most-significant-bit first.
   *
   * @param ack Indicates if the last byte received should be acknowledged.
   *    The byte is acknowledged if ack is I2C_READ (true), and is nak'd if
   *    I2C_DONE (false). The first byte received are always acknowledged.
   *
   * @return The word read from the I2C device.
   */
  unsigned short ReadWord(bool order, bool ack=I2C_READ);


  /**
   * Read a long (4 bytes) from the I2C device.  This function should only
   * be called after a successful call to Start() with the I2C_READ option
   * specified.  Bits are always received most-significant-bit first.
   *
   * @param order This specifies the order that the bytes are read from the
   *    bus; either I2C_MSB_FIRST or I2C_LSB_FIRST.
   *    Bits are always received most-significant-bit first.
   *
   * @param ack Indicates if the last byte received should be acknowledged.
   *    The byte is acknowledged if ack is I2C_READ (true), and is nak'd if
   *    I2C_DONE (false). The first three bytes received are always acknowledged.
   *
   * @return The long read from the I2C device.
   */
  unsigned long  ReadLong(bool order, bool ack=I2C_READ);


  /**
   * Read a variable number of bytes from the I2C device.  This function
   * should only be called after a successful call to Start() with the
   * I2C_READ option specified.
   *
   * The buffer is filled in the order that bytes are read from the bus
   * (first byte recieved into address 0 , then address 1, etc).  Bits are
   * always received most-significant-bit first.
   *
   * @param buffer A pointer to an array of bytes to be filled.  The buffer
   *    must be at least as large as the count parameter.
   *
   * @param count This specifies the number of bytes to receive.
   *
   * @param ack Indicates if the last byte received should be acknowledged.
   *    The byte is acknowledged if ack is I2C_READ (true), and is nak'd if
   *    I2C_DONE (false). The first (count-1) bytes received are always acknowledged.
   */
  void ReadData(void *buffer, unsigned short count, bool ack);


  /**
   * Utility function to probe an address for the presence of a
   * device.
   *
   * It determines this by starting a write operation to
   * the specified address and seeing if it is acknowleged.
   * No bytes are actually written to the device, however it is
   * possible that this approach will not work correctly with all
   * types of I2C devices.
   *
   * Also note that Probe() may return true if multiple I2C devices
   * are present at the same address.  It cannot determine if
   * the acknowledgement was issued by multiple devices.
   *
   * This function is not used internally by this class, and is
   * provided primarily as a diagnostic tool for use during
   * developlement and testing.
   *
   * @param addr The 7-bit I2C address of the target device, which must
   * 	be in the range of 0..127.  If you are having trouble probing
   *    the device, or if the device documentation provides an oven/odd
   *    address pair for reading and writing, try dividing the address by 2.
   *
   * @return Returns true if the probe was acknowleged and false if not.
   */
  bool Probe(unsigned short addr);
  
  
  /**
   * Utility function to scan an address range for the presence of
   * devices.  It calls probe for each address in the specified
   * range, and prints a summary of the results to stdout.
   *
   * This function is not used internally by this class, and is
   * provided primarily as a diagnostic tool for use during
   * developlement and testing.
   *
   * @param min The 7-bit I2C address of the start of the scan, which must
   * 	must be in the range of 0..127.
   *
   * @param max The 7-bit I2C address of the end of the scan, which must
   * 	must be in the range of 0..127.
   *
   * @return Returns the number of devices that appear to be present
   *    within the within the address range
   */
  short Scan(unsigned short min=0x08, unsigned short max=0x77);


  /**
   * See the definition of IProperty::GetProperty for more information
   * on this method.  The following properties are supported:
   *   	QI2C_PROP_SCL_MIN_USEC_LOW,		QI2C_PROP_SCL_MIN_USEC_HIGH
   */
  virtual int GetProperty(int property, long *value);


  /**
   * See the definition of IProperty::SetProperty for more information
   * on this method.  The following properties are supported:
   *   	QI2C_PROP_SCL_MIN_USEC_LOW,		QI2C_PROP_SCL_MIN_USEC_HIGH
   */
  virtual int SetProperty(int property, long value);


private:
  CQEI2CBus();
  ~CQEI2CBus();

  pthread_mutex_t bus_mutex;

  unsigned long int SCL_MinUsecLow;
  unsigned long int SCL_MinUsecHigh;
  unsigned int SessionID;
  bool SessionActive;
  C9302Hardware *m_p9302hw;
  volatile unsigned short int	*m_i2c_reg; // Pointer to I2C register (VEXpro FPGA)
};

#endif // _QEI2CBUS_H
