#ifndef _QEGPIOINT_H
#define _QEGPIOINT_H

#include "memmap.h"

#define QEG_DEFAULT_BASE                                0x440
#define QEG_DEFAULT_VECTOR_START                        8
#define QEG_NUM_VECTORS                                 8


class CQEGpioInt 
{
public:
  CQEGpioInt(unsigned int base=QEG_DEFAULT_BASE, 
	     unsigned int vectorStart=QEG_DEFAULT_VECTOR_START);
  ~CQEGpioInt();


  // Each bit in this register reflects the state of corresponding pins.  
  // Bits 0 thru 7 are input.  Bits 8 thru 15 are output
  inline volatile unsigned short *Data()
  {
    return m_map.Ushort(m_base + 0x00);
  }


  // Bits 0 thru 7 in this register determine whether the corresponding input
  // will trigger an interrupt.  If the bit value is 0, the corresponding 
  // input will not trigger an interrupt.  If the bit value is 1, the 
  // corresponding pin will trigger an interrupt on each rising or falling 
  // edge.
  inline volatile unsigned short *IntMask()
  {
    return m_map.Ushort(m_base + 0x02);
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
    return m_map.Ushort(m_base + 0x04);
  }

private:
  CMemMap m_map;
  unsigned int m_base;
  int m_interrupts[QEG_NUM_VECTORS];
};

#endif
