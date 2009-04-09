#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "9302hw.h"

#define KP_OK             0x0002
#define KP_CANCEL         0x0004
#define KP_RIGHT          0x0008
#define KP_LEFT           0x0010
#define KP_UP             0x0020
#define KP_DOWN           0x0040

class CKeypad
{
public:
  CKeypad();
  ~CKeypad();

  unsigned int GetKey(bool wait=true);

  inline bool KeyOk()
  {
    return GetKey(false)&KP_OK;
  }
  inline bool KeyCancel()
  {
    return GetKey(false)&KP_CANCEL;
  }
  inline bool KeyUp()
  {
    return GetKey(false)&KP_UP;
  }
  inline bool KeyDown()
  {
    return GetKey(false)&KP_DOWN;
  }
  inline bool KeyLeft()
  {
    return GetKey(false)&KP_LEFT;
  }
  inline bool KeyRight()
  {
    return GetKey(false)&KP_RIGHT;
  }

private:
  void KeyRelease();
  C9302Hardware *m_p9302hw;
};

#endif
