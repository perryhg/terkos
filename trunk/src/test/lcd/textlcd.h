#ifndef _TEXTLCD_H
#define _TEXTLCD_H

#include <stdlib.h>
#include <stdarg.h>
#include "property.h"
#include "9302hw.h"

#define TL_HEIGHT           2
#define TL_WIDTH            16

// Properties
#define TL_OBJECT           1
#define TL_PROP_BACKLIGHT   PROP_ID(TL_OBJECT, 0)
#define TL_PROP_HEIGHT      PROP_ID(TL_OBJECT, 1)
#define TL_PROP_WIDTH       PROP_ID(TL_OBJECT, 2)

#define TL_DELAY()          Delay(20)
#define TL_DELAYL()         Delay(2000)

class CTextLcd : public IProperty
{
public:
  CTextLcd();
  ~CTextLcd();

  void Clear();
  void MoveCursor(int x, int y);
  int printf(char *format, ...);
  // Save(CContext *context);
  // Restore(CContext context);
  void SetBacklight(bool state);

  // IProperty
  virtual int GetProperty(int property, long *value);
  virtual int SetProperty(int property, long value);

private:
  void Init();
  void DefineChars();
  void PutNibble(unsigned char c);
  void PutByte(unsigned char c);

  void Delay(unsigned int us);
  C9302Hardware *m_p9302hw;
};

#endif
