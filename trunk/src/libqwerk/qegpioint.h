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

  inline volatile unsigned short *IntEdge()
  {
    return m_p9302hw->m_fpga.Ushort(m_base + 0x04);
  }

private:
  CQEGpioInt();
  ~CQEGpioInt();

  C9302Hardware *m_p9302hw;
  unsigned int m_base;
};

#endif
