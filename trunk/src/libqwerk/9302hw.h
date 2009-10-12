#ifndef _9302HW_H
#define _9302HW_H

#include "singleton.h"
#include "memmap.h"

#define HW_AD_CHANNELS             5

class C9302Hardware
{
public:
  SINGLETON(C9302Hardware);

  unsigned short GetAD(unsigned int channel);

  inline void SetGreenLED(bool state)
  {
    *PortEData() = state ? *PortEData() |= 0x01 : *PortEData() &= ~0x01;
  }
  inline void SetRedLED(bool state)
  {
    *PortEData() = state ? *PortEData() |= 0x02 : *PortEData() &= ~0x02;
  }
  inline volatile unsigned int *PortAData()
  {
    return m_gpio.Uint(0x00);
  }
  inline volatile unsigned int *PortADataDR()
  {
    return m_gpio.Uint(0x10);
  }
  inline volatile unsigned int *PortBData()
  {
    return m_gpio.Uint(0x04);
  }
  inline volatile unsigned int *PortBDataDR()
  {
    return m_gpio.Uint(0x14);
  }
  inline volatile unsigned int *PortCData()
  {
    return m_gpio.Uint(0x08);
  }
  inline volatile unsigned int *PortCDataDR()
  {
    return m_gpio.Uint(0x18);
  }
  inline volatile unsigned int *PortEData()
  {
    return m_gpio.Uint(0x20);
  }
  inline volatile unsigned int *PortEDataDR()
  {
    return m_gpio.Uint(0x24);
  }
  inline volatile unsigned int *PortFData()
  {
    return m_gpio.Uint(0x30);
  }
  inline volatile unsigned int *PortFDataDR()
  {
    return m_gpio.Uint(0x34);
  }
  inline volatile unsigned int *PortGData()
  {
    return m_gpio.Uint(0x38);
  }
  inline volatile unsigned int *PortGDataDR()
  {
    return m_gpio.Uint(0x3c);
  }
  inline volatile unsigned int *PortHData()
  {
    return m_gpio.Uint(0x40);
  }
  inline volatile unsigned int *PortHDataDR()
  {
    return m_gpio.Uint(0x44);
  }

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
