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

#include <unistd.h>
#include "keypad.h"

SINGLETON_REGISTER(CKeypad);

const unsigned int CKeypad::KEY_OK = KP_KEY_OK;
const unsigned int CKeypad::KEY_CANCEL = KP_KEY_CANCEL;
const unsigned int CKeypad::KEY_RIGHT = KP_KEY_RIGHT;
const unsigned int CKeypad::KEY_LEFT = KP_KEY_LEFT;
const unsigned int CKeypad::KEY_UP = KP_KEY_UP;
const unsigned int CKeypad::KEY_DOWN = KP_KEY_DOWN;

CKeypad::CKeypad()
{
  m_p9302hw = C9302Hardware::GetPtr();

  // set as input
  *m_p9302hw->PortFDataDR() &= ~0x0e;
  *m_p9302hw->PortADataDR() &= ~0x70;
}

CKeypad::~CKeypad()
{
  C9302Hardware::Release();
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

      usleep(10000);
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
