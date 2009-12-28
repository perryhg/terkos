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

#ifndef _TEXTLCD_H
#define _TEXTLCD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include "singleton.h"
#include "property.h"
#include "9302hw.h"

// Properties
#define TL_OBJECT           1
#define TL_PROP_BACKLIGHT   PROP_ID(TL_OBJECT, 0)
#define TL_PROP_HEIGHT      PROP_ID(TL_OBJECT, 1)
#define TL_PROP_WIDTH       PROP_ID(TL_OBJECT, 2)

#define TL_CHAR_UP          0x01
#define TL_CHAR_DOWN        0xf5
#define TL_CHAR_LEFT        0xf7
#define TL_CHAR_RIGHT       0xf6
#define TL_CHAR_LOWBATT     0x02
#define TL_CHAR_COPYRIGHT   0xcf 
#define TL_CHAR_WSA0        0x03 // wireless strength (just antenna)
#define TL_CHAR_WSA1        0x04 // wireless strength (antenna with 1 bar)
#define TL_CHAR_WSB0        0x20 // wireless strength (2nd char, 0 bars)
#define TL_CHAR_WSB1        0x05 // wireless strength (2nd char, 1 bar)
#define TL_CHAR_WSB2        0x06 // wireless strength (2nd char, 2 bars)
#define TL_CHAR_WSB3        0x07 // wireless strength (2nd char, 2 bars)


#define TL_STRING_UP          "\x01"
#define TL_STRING_DOWN        "\xf5"
#define TL_STRING_LEFT        "\xf7"
#define TL_STRING_RIGHT       "\xf6"
#define TL_STRING_LOWBATT     "\x02"
#define TL_STRING_COPYRIGHT   "\xcf" 
#define TL_STRING_WSA0        "\x03" // wireless strength (just antenna)
#define TL_STRING_WSA1        "\x04" // wireless strength (antenna with 1 bar)
#define TL_STRING_WSB0        "\x20" // wireless strength (2nd char, 0 bars)
#define TL_STRING_WSB1        "\x05" // wireless strength (2nd char, 1 bar)
#define TL_STRING_WSB2        "\x06" // wireless strength (2nd char, 2 bars)
#define TL_STRING_WSB3        "\x07" // wireless strength (2nd char, 2 bars)


#define TL_DELAY()          Delay(20)
#define TL_DELAYL()         Delay(2000)

class CTextLcd : public IProperty
{
public:
  SINGLETON(CTextLcd);

  static const unsigned int NUM_ROWS;
  static const unsigned int NUM_COLUMNS;
          
  void Clear();
  void MoveCursor(const unsigned int row, const unsigned int col);
  int printf(const char *format, ...);
  void SetCharacter(const char character);

  // Save(CContext *context);
  // Restore(CContext context);

  // IProperty
  virtual int GetProperty(int property, long *value);
  virtual int SetProperty(int property, long value);

private:
  CTextLcd();
  virtual ~CTextLcd();

  void Init();
  void DefineChars();
  void PutNibble(unsigned char c);
  void PutByte(unsigned char c);
  void Delay(unsigned int us);

  const bool IsValidRow(const unsigned int row) const
  {
    return (row < CTextLcd::NUM_ROWS);
  }

  const bool IsValidColumn(const unsigned int col) const
  {
    return (col < CTextLcd::NUM_COLUMNS);
  }

  const bool IsValidPosition(const unsigned int row, const unsigned int col) const
  {
    return IsValidRow(row) && IsValidColumn(col);
  }

  C9302Hardware *m_p9302hw;
};

#endif
