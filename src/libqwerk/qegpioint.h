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

#ifndef _QEGPIOINT_H
#define _QEGPIOINT_H

#include "property.h"
#include "singleton.h"
#include "9302hw.h"

// Properties
#define QEG_OBJECT                           3
#define QEG_PROP_WIDTH                       PROP_ID(QEG_OBJECT, 0)

// interrupt modes, can possibly be used as bitmap
#define QEG_INTERRUPT_POSEDGE                0x0001
#define QEG_INTERRUPT_NEGEDGE                0x0002

#define QEG_DEFAULT_BASE                     0x440
#define QEG_NUM_IO                           16

// set interrupt mode, pos, neg, disabled (set/getprop)
// register callback (SetCallback(int vec, callback))
// read timestamps from queue (ReadTimestamp()
// set/read ddr (set/getprop)
// set/read data (set/getprop)

/**
 * CQEGpioInt is used to interact with the external digital ports, 
 * such as reading, writing and triggering on interruptible input 
 * events. 
 * To instantiate this class: 
 * \code
 * // by pointer
 * CQEGpioInt *pio = CQEGpioInt::GetPtr(); 
 * // or by reference
 * CQEGpioInt &io = CQEGpioInt::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQEGpioInt::Release();
 * \endcode
 */
class CQEGpioInt : public IProperty
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */ 
  SINGLETON(CQEGpioInt);

  /**
   * See the definition of IProperty::GetProperty for more information 
   * on this method.  The following properties are supported:
   */
  virtual int GetProperty(int property, long *value);

  /**
   * See the definition of IProperty::SetProperty for more information 
   * on this method.  The following properties re supported:
   */
  virtual int SetProperty(int property, long value);

  /**
   * Changes the state of 
   * the external digital I/O signals.
   * @param data a bitmap with bit 0 (LSB)
   * corresponding to digital signal 1 and bit 15 corresponding 
   * to digital signal 16 -- used to change the state of signals
   * configured as outputs.  A 0 (zero) bit value set the 
   * corresponding I/O signal 
   * to be logic low.  A 1 (one) bit value set the corresponding 
   * I/O signal to be logic high.
   */
  void SetData(unsigned int data);

  /**
   * Sets the data direction of the I/O signals.  
   * @param direction a bitmap with bit 0 (LSB)
   * corresponding to digital signal 1 and bit 15 corresponding 
   * to digital signal 16.  A 0 (zero) bit value configures the 
   * corresponding I/O signal 
   * to be input.  A 1 (one) bit value configures the corresponding I/O signal 
   * to be an output.
   */
  void SetDataDirection(unsigned int direction);

  /**
   * Returns the state of 
   * the external digital I/O signals as a bitmap with bit 0 (LSB)
   * corresponding to digital signal 1 and bit 15 corresponding 
   * to digital signal 16 -- typically used to read signals
   * configured as inputs, although the state of output signals are also
   * returned. 
   */
  unsigned int GetData();

  /**
   * Returns the contents of the data
   * direction register -- only the least significant 16 bits are used.
   */
  unsigned int GetDataDirection();

  /**
   * Sets the interrupt mode of the specified I/O.  
   * @param io the I/O signal in question, 0 through 15 corresponding 
   * digital connectors 1 through 16, respectively.
   * @param mode the following modes are supported
   * - QEG_INTERRUPT_POSEDGE=interrupt on transition from 
   * logic low to logic high.
   * QEB_INTERRUPT_NEGEDGE=interrupt on transition from 
   * logic high to logic low.
   * @return 0 if success -1 if error.
   */ 
  int SetInterruptMode(unsigned int io, unsigned int mode);

  /** 
   * Enable or disable the interrupt for the specified I/O.  
   * @param the I/O signal in question, 0 through 15 corresponding 
   * digital connectors 1 through 16, respectively.
   * @param enable false=disable interrupt, true=enable interrupt
   * @return 0 if success -1 if error.
   */
  int SetInterrupt(unsigned int io, bool enable);
  
  /**
   * Register an interrupt callback -- ie a function that gets called
   * when an interrupt event occurs on the specified I/O signal.
   * @param io the I/O signal in question, 0 through 15 corresponding 
   * digital connectors 1 through 16, respectively.
   * @param callback the callback function which has arguments for the  
   * @return 0 if success -1 if error.
   */
  int RegisterCallback(unsigned int io, void (*callback)(unsigned int, struct timeval *));

 /**
   * Remove (unregister) an interrupt callback.
   * @param io the I/O signal in question, 0 through 15 corresponding 
   * digital connectors 1 through 16, respectively.
   * @return 0 if success -1 if error.
   */
   int UnregisterCallback(unsigned int io);

private:
  CQEGpioInt();
  ~CQEGpioInt();

  int OpenDevice(unsigned int io);
  void CloseDevice(unsigned int io);
    
  static void SigHandler(int signum);

  static int m_fd[QEG_NUM_IO];
  static void (*m_callback[QEG_NUM_IO])(unsigned int, struct timeval *);
  C9302Hardware *m_p9302hw;
  volatile unsigned short *m_data;
  volatile unsigned short *m_dataDir;
  volatile unsigned short *m_intMode;
};

#endif
