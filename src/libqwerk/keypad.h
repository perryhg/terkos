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

/** 
 * CKeypad allows your program to gather user input from the 
 * up, down, left, right, OK, and cancel buttons.
 * To instantiate this class: 
 * \code
 * // by pointer
 * CKeypad *pkp = CKeypad::GetPtr(); 
 * // or by reference
 * CKeypad &kp = CKeypad::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CKeypad::Release();
 * \endcode
 */
class CKeypad
{
public:
  /**
   * This internal macro handles instantiation of this class.
   * Note, since this device is input-only (read-only), multiple processes
   * are allowed to use it.  In this case, the SINGLETON macro is used
   * to prevent multiple instances within the same process.  
   */ 
  SINGLETON(CKeypad);

  static const unsigned int KEY_OK;
  static const unsigned int KEY_CANCEL;
  static const unsigned int KEY_RIGHT;
  static const unsigned int KEY_LEFT;
  static const unsigned int KEY_UP;
  static const unsigned int KEY_DOWN;
   
  /**
   * Get key input.
   * @param wait true=wait for key to be pressed before returning, 
   * false=do not wait, return immediately with key value, even if no 
   * key is being pressed. This method handles switch debouncing.  
   * @return Bitmap of keys being pressed, or 0 if no keys are being pressed. 
   * Bitmap values consist of bit values KP_KEY_OK, KP_KEY_CANCEL, KP_KEY_UP, 
   * KP_KEY_DOWN, KP_KEY_LEFT, KP_KEY_RIGHT. 
   */
  const unsigned int GetKey(bool wait = true);


  /**
   * Wait for key to be released.  Returns immediately
   * if no key is being pressed. 
   */
  void KeyRelease();

  /**
   * Return state of OK key. 
   */
  inline bool KeyOk()
  {
    return GetKey(false) & KEY_OK;
  }

  /**
   * Return state of Cancel key. 
   */
  inline bool KeyCancel()
  {
    return GetKey(false) & KEY_CANCEL;
  }

  /**
   * Return state of Up key. 
   */
  inline bool KeyUp()
  {
    return GetKey(false) & KEY_UP;
  }

  /**
   * Return state of Down key. 
   */
  inline bool KeyDown()
  {
    return GetKey(false) & KEY_DOWN;
  }

  /**
   * Return state of Left key. 
   */
  inline bool KeyLeft()
  {
    return GetKey(false) & KEY_LEFT;
  }

  /**
   * Return state of Right key. 
   */
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
