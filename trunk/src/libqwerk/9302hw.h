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

#ifndef _9302HW_H
#define _9302HW_H

#include "singleton.h"
#include "memmap.h"

/** 
 * The number of A/D channels that the EP9302 supports. 
 */
#define HW_AD_CHANNELS             5

/**
 * C9302Hardware interacts directly with the Cirrus EP9302 hardware 
 * peripherals.  This class is normally only used by other 
 * I/O classes.  You shouldn't need to use this class if you
 * wish to interact with sensors, actuators, lcd and/or keypad.      
 * To instantiate this class: 
 * \code
 * // by pointer
 * C9302Hardware *phw = C9302Hardware::GetPtr(); 
 * // or by reference
 * C9302Hardware &hw = C9302Hardware::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * C9302Hardware::Release();
 * \endcode
 */
class C9302Hardware
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */ 
  SINGLETON(C9302Hardware);

  /** 
   * Get the raw A/D value given the channel.
   * @param channel value between 0 and 4
   * @return raw analog value of channel
   */
  unsigned short GetAD(unsigned int channel);

  /** 
   * Set the state of the green LED on the CPU board.
   * @param state true=on, false=off
   */
  inline void SetGreenLED(bool state)
  {
    *PortEData() = state ? *PortEData() |= 0x01 : *PortEData() &= ~0x01;
  }

  /** 
   * Set the state of the red LED on the CPU board.
   * @param state true=on, false=off
   */
  inline void SetRedLED(bool state)
  {
    *PortEData() = state ? *PortEData() |= 0x02 : *PortEData() &= ~0x02;
  }

  /** 
   * Set/get Port A data register on EP9302.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortAData()
  {
    return m_gpio.Uint(0x00);
  }

  /** 
   * Set/get Port A data direction register on EP9302.
   * @return Address of port A data direction register
   */
  inline volatile unsigned int *PortADataDR()
  {
    return m_gpio.Uint(0x10);
  }

  /** 
   * Set/get Port B data register on EP9302.
   * @return Address of port B data register
   */
  inline volatile unsigned int *PortBData()
  {
    return m_gpio.Uint(0x04);
  }

  /** 
   * Set/get Port B data direction register on EP9302.
   * @return Address of port B data direction register
   */
  inline volatile unsigned int *PortBDataDR()
  {
    return m_gpio.Uint(0x14);
  }

  /** 
   * Set/get Port C data register on EP9302.
   * @return Address of port C data register
   */
  inline volatile unsigned int *PortCData()
  {
    return m_gpio.Uint(0x08);
  }

  /** 
   * Set/get Port C data direction register on EP9302.
   * @return Address of port C data direction register
   */
  inline volatile unsigned int *PortCDataDR()
  {
    return m_gpio.Uint(0x18);
  }

  /** 
   * Set/get Port E data register on EP9302.
   * @return Address of port E data register
   */
  inline volatile unsigned int *PortEData()
  {
    return m_gpio.Uint(0x20);
  }

  /** 
   * Set/get Port E data direction register on EP9302.
   * @return Address of port E data direction register
   */
  inline volatile unsigned int *PortEDataDR()
  {
    return m_gpio.Uint(0x24);
  }

  /** 
   * Set/get Port F data register on EP9302.
   * @return Address of port F data register
   */
  inline volatile unsigned int *PortFData()
  {
    return m_gpio.Uint(0x30);
  }

  /** 
   * Set/get Port F data direction register on EP9302.
   * @return Address of port F data direction register
   */
  inline volatile unsigned int *PortFDataDR()
  {
    return m_gpio.Uint(0x34);
  }

  /** 
   * Set/get Port G data register on EP9302.
   * @return Address of port G data register
   */
  inline volatile unsigned int *PortGData()
  {
    return m_gpio.Uint(0x38);
  }

  /** 
   * Set/get Port G data direction register on EP9302.
   * @return Address of port G data direction register
   */
  inline volatile unsigned int *PortGDataDR()
  {
    return m_gpio.Uint(0x3c);
  }

  /** 
   * Set/get Port H data register on EP9302.
   * @return Address of port H data register
   */
  inline volatile unsigned int *PortHData()
  {
    return m_gpio.Uint(0x40);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortHDataDR()
  {
    return m_gpio.Uint(0x44);
  }

  /** 
   * Get the FPGA bitstream version.
   * @return FPGA bitstream version, msb=major version, lsb=minor version 
   */
  unsigned short GetBitstreamVersion();

  /** 
   * Get the FPGA bitstream minor version.
   * @return FPGA bitstream minor version 
   */
  unsigned char GetBitstreamMinorVersion();

  /** 
   * Get the FPGA bitstream major version.
   * @return FPGA bitstream major version 
   */
  unsigned char GetBitstreamMajorVersion();

  /**
   * memory mapped region of memory controller on EP9302
   */
  CMemMap m_scr;


  /**
   * memory mapped region of general purpose I/O controller on EP9302
   */
  CMemMap m_gpio;

  /**
   * memory mapped region of system controller on EP9302
   */
  CMemMap m_syscon;

  /**
   * memory mapped region of FPGA on EP9302
   */
  CMemMap m_fpga;

  /**
   * memory mapped region of analog-to-digital controller on EP9302
   */
  CMemMap m_adc;

  /**
   * memory mapped region of UART1 (serial controller) on EP9302
   */
  CMemMap m_uart1;

  /**
   * memory mapped region UART2 (serial controller) on EP9302
   */
  CMemMap m_uart2;

  /**
   * memory mapped region for TIMER1-TIMER4 (timers) on EP9302
   */
  CMemMap m_timers;

private:
  C9302Hardware();
  ~C9302Hardware();
};

#endif
