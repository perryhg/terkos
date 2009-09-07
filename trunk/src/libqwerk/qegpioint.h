#ifndef _QEGPIOINT_H
#define _QEGPIOINT_H

#include "singleton.h"
#include "9302hw.h"

#define QEG_DEFAULT_BASE                                0x440
#define QEG_DEFAULT_VECTOR_START                        8
#define QEG_NUM_VECTORS                                 8


class CQEGpioInt 
{
public:
  SINGLETON(CQEGpioInt);

  inline volatile unsigned short *Data()
  {
    return m_p9302hw->m_fpga.Ushort(m_base + 0x00);
  }
  inline volatile unsigned short *DataDir()
  {
    return m_p9302hw->m_fpga.Ushort(m_base + 0x02);
  }


  // Bits 0 thru 7 in this register determine whether the corresponding input
  // will trigger an interrupt.  If the bit value is 0, the corresponding 
  // input will not trigger an interrupt.  If the bit value is 1, the 
  // corresponding pin will trigger an interrupt on each rising or falling 
  // edge.
  inline volatile unsigned short *IntMask()
  {
    return m_p9302hw->m_fpga.Ushort(m_base + 0x04);
  }

  // Bits 0 thru 7 in this register determines which edge (rising or falling) 
  // the corresponding input will trigger an interrupt.
  // If the bit value is 0, the corresponding pin will trigger on each 
  // negative edge of the corresponding pin.
  // If the bit value is 1, the corresponding pin will trigger on each 
  // positive edge of the corresponding pin.
  // An interrupt is only generated for pins that are unmasked.
  inline volatile unsigned short *IntEdge()
  {
    return m_p9302hw->m_fpga.Ushort(m_base + 0x06);
  }

private:
  CQEGpioInt();
  ~CQEGpioInt();

  C9302Hardware *m_p9302hw;
  unsigned int m_base;
};

#endif
