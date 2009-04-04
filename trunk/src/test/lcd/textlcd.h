#ifndef _TEXTLCD_H
#define _TEXTLCD_H

#include <stdlib.h>
#include <stdarg.h>
#include "9302hw.h"

#define TL_DELAY()       Delay(20)
#define TL_DELAYL()      Delay(2000)

class CTextLcd
{
public:
  CTextLcd();
  ~CTextLcd();

  void Clear();
  void MoveCursor(int x, int y);
  int printf(char *format, ...);
  // Save(CContext *context);
  // Restore(CContext context);
  // GetCursor(int *px, int *py);
  void SetBacklight(bool state);

private:
  void Init();
  void DefineChars();
  void PutNibble(unsigned char c);
  void PutByte(unsigned char c);

  void Delay(unsigned int us);
  C9302Hardware *m_p9302hw;
};

#endif
