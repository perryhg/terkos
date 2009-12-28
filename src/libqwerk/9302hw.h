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

class C9302Hardware
{
public:
  SINGLETON(C9302Hardware);

  /** 
   * Get the A/D value in millivolts given the channel.
   * @param channel value between 0 and 24
   * @return analog value of channel in millivolts.
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
   * Set the state of the green LED on the CPU board.
   * @param state true=on, false=off
   */
  inline void SetRedLED(bool state)
  {
    *PortEData() = state ? *PortEData() |= 0x02 : *PortEData() &= ~0x02;
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortAData()
  {
    return m_gpio.Uint(0x00);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortADataDR()
  {
    return m_gpio.Uint(0x10);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortBData()
  {
    return m_gpio.Uint(0x04);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortBDataDR()
  {
    return m_gpio.Uint(0x14);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortCData()
  {
    return m_gpio.Uint(0x08);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortCDataDR()
  {
    return m_gpio.Uint(0x18);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortEData()
  {
    return m_gpio.Uint(0x20);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortEDataDR()
  {
    return m_gpio.Uint(0x24);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortFData()
  {
    return m_gpio.Uint(0x30);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortFDataDR()
  {
    return m_gpio.Uint(0x34);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortGData()
  {
    return m_gpio.Uint(0x38);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
   */
  inline volatile unsigned int *PortGDataDR()
  {
    return m_gpio.Uint(0x3c);
  }

  /** 
   * Set/get Port A data register.
   * @return Address of port A data register
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
   * @return Address of port A data register
   */
  unsigned short GetBitstreamVersion();

  unsigned char GetBitstreamMinorVersion();
  unsigned char GetBitstreamMajorVersion();

  // you can access these directly
  CMemMap m_scr;
  CMemMap m_gpio;
  CMemMap m_syscon;
  CMemMap m_fpga;
  CMemMap m_adc;
  CMemMap m_uart1;
  CMemMap m_uart2;

private:
  C9302Hardware();
  ~C9302Hardware();
};

#endif
