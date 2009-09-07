#include <stdio.h>
#include "qeservo.h"

SINGLETON_REGISTER(CQEServo);

CQEServo::CQEServo()
{
  long addr = QES_DEFAULT_ADDR;

  m_p9302hw = C9302Hardware::GetPtr();

  m_num = QES_DEFAULT_SERVOS;
  m_divider = m_p9302hw->m_fpga.Ushort(addr);
  m_freq = m_p9302hw->m_fpga.Ushort(addr+2);
  m_up = m_p9302hw->m_fpga.Ushort(addr+4);

  *m_divider = QES_DEFAULT_DIVIDER; 
  *m_freq = QES_DEFAULT_FREQ; 

  m_min = QES_DEFAULT_MIN;
  m_max = QES_DEFAULT_MAX;
}

CQEServo::~CQEServo()
{
  C9302Hardware::Release();
}

void CQEServo::SetTiming(unsigned short min, unsigned short max)
{
  m_min = min;
  m_max = max;
}

void CQEServo::Disable(unsigned int index)
{
  m_up[index] = 0;
}

unsigned short CQEServo::GetPosition(unsigned int index)
{
  if (index>=m_num)
    return 0;

  return m_up[index] - m_min;
}

void CQEServo::SetPosition(unsigned int index, unsigned short pos)
{
  unsigned short val;

  if (index>=m_num)
    return;

  val = pos + m_min;

  if (val>m_max)
    val = m_max;

  m_up[index] = val;
}


