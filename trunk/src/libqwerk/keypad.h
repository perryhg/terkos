#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "singleton.h"
#include "9302hw.h"

#define KP_KEY_OK      0x0002
#define KP_KEY_CANCEL  0x0004
#define KP_KEY_RIGHT   0x0008
#define KP_KEY_LEFT    0x0010
#define KP_KEY_UP      0x0020
#define KP_KEY_DOWN    0x0040

class CKeypad
{
public:
  SINGLETON(CKeypad);

  static const unsigned int KEY_OK;
  static const unsigned int KEY_CANCEL;
  static const unsigned int KEY_RIGHT;
  static const unsigned int KEY_LEFT;
  static const unsigned int KEY_UP;
  static const unsigned int KEY_DOWN;
     
  // returns 0 if no key is being pressed
  // or waits until key is pressed and returns key value
  const unsigned int GetKey(bool wait = true);

  // wait for key to be released-- can be used with methods below
  void KeyRelease();

  inline bool KeyOk()
  {
    return GetKey(false) & KEY_OK;
  }
  inline bool KeyCancel()
  {
    return GetKey(false) & KEY_CANCEL;
  }
  inline bool KeyUp()
  {
    return GetKey(false) & KEY_UP;
  }
  inline bool KeyDown()
  {
    return GetKey(false) & KEY_DOWN;
  }
  inline bool KeyLeft()
  {
    return GetKey(false) & KEY_LEFT;
  }
  inline bool KeyRight()
  {
    return GetKey(false) & KEY_RIGHT;
  }

private:
  CKeypad();
  ~CKeypad();

  C9302Hardware *m_p9302hw;
};

#endif
