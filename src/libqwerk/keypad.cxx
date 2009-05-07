#include <unistd.h>
#include "keypad.h"

const unsigned int CKeypad::KEY_OK = 0x0002;
const unsigned int CKeypad::KEY_CANCEL = 0x0004;
const unsigned int CKeypad::KEY_RIGHT = 0x0008;
const unsigned int CKeypad::KEY_LEFT = 0x0010;
const unsigned int CKeypad::KEY_UP = 0x0020;
const unsigned int CKeypad::KEY_DOWN = 0x0040;

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

const unsigned int CKeypad::GetKey(bool wait)
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
