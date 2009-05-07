#ifndef _KEYPAD_H
#define _KEYPAD_H

#include "9302hw.h"

class CKeypad
   {
   public:

      static const unsigned int KEY_OK;
      static const unsigned int KEY_CANCEL;
      static const unsigned int KEY_RIGHT;
      static const unsigned int KEY_LEFT;
      static const unsigned int KEY_UP;
      static const unsigned int KEY_DOWN;

      CKeypad();
      ~CKeypad();

      // returns 0 if no key is being pressed
      // or waits until key is pressed and returns key value
      unsigned int GetKey(bool wait = true);

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
      C9302Hardware *m_p9302hw;
   };

#endif
