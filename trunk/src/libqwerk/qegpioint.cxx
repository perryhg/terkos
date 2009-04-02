#include "qegpioint.h"

CQEGpioInt::CQEGpioInt(unsigned int base, unsigned int vectorStart) :
  m_map(0x20000000, 0x1000)
{
  m_base = base;
}
  
CQEGpioInt::~CQEGpioInt()
{
}
