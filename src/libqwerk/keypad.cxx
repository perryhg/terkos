#include <unistd.h>
#include "keypad.h"

CKeypad::CKeypad()
{
  m_p9302hw = C9302Hardware::GetObject();
  
  // set as input
  *m_p9302hw->PortFDataDR() &= ~0x0e;
  *m_p9302hw->PortADataDR() &= ~0x70;
}

CKeypad::~CKeypad()
{
  C9302Hardware::ReleaseObject();
}

unsigned int CKeypad::GetKey(bool wait)
{
  unsigned int key;

  while(1)
    {
      key = *m_p9302hw->PortFData() & 0x0e;
      key |=  *m_p9302hw->PortAData() & 0x70;
      key ^= 0x7c;
     
      if (!wait || key)
	break;
    }

  if (wait)
    KeyRelease();

  return key;
}

void CKeypad::KeyRelease()
{
  // wait for debouncing to dissipate
  usleep(10000);
  
  while(GetKey(false))
    usleep(10000);
}
