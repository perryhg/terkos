
#include "textlcd.h"

const unsigned int CTextLcd::NUM_ROWS = 2;
const unsigned int CTextLcd::NUM_COLUMNS = 16;
const string CTextLcd::BLANK_LINE(CTextLcd::NUM_COLUMNS, ' ');

CTextLcd::CTextLcd()
{
  m_p9302hw = C9302Hardware::GetObject();
  Init();
}

CTextLcd::~CTextLcd()
{
  C9302Hardware::ReleaseObject();
}

void CTextLcd::Clear()
{
  // RS low
  *m_p9302hw->PortGDataDR() |= 0x02;
  TL_DELAY();

  PutByte(1);
}

void CTextLcd::ClearLine(unsigned int lineNumber)
   {
   if (IsValidRow(lineNumber))
      {
      MoveCursor(lineNumber, 0);
      printf("%s", CTextLcd::BLANK_LINE.c_str());
      }
   }

void CTextLcd::MoveCursor(const unsigned int row, const unsigned int col)
{
   if (IsValidPosition(row, col))
      {
      // RS low
      *m_p9302hw->PortGDataDR() |= 0x02;
      TL_DELAY();

      PutByte(0x80 | (col&0x0f) + (row ? 0x40 : 0));
      }
}

int CTextLcd::printf(const char *format, ...)
{
  int i;
  char buf[256];
  va_list args;

  va_start (args, format);
  vsprintf (buf, format, args);
  va_end (args);

  // RS High
  *m_p9302hw->PortGDataDR() &= ~0x02;
  TL_DELAY();

  for (i=0; buf[i]; i++)
    PutByte(buf[i]);

  return i;
}

void CTextLcd::SetCharacter(const unsigned int row, const unsigned int col, const char character)
   {
   if (IsValidPosition(row, col))
      {
      MoveCursor(row, col);
      printf("%c", character);
      }
   }

void CTextLcd::SetLine(const unsigned int lineNumber, const string& text, const bool willClearLineFirst)
   {
   if (IsValidRow(lineNumber))
      {
      if (willClearLineFirst)
         {
         ClearLine(lineNumber);
         }
      if (text.length() > 0)
         {
         MoveCursor(lineNumber, 0);
         printf("%s", text.substr(0, CTextLcd::NUM_COLUMNS).c_str());
         }
      }
   }

void CTextLcd::SetText(const string& text, const bool willClearFirst)
   {
   if (willClearFirst)
      {
      Clear();
      }

   if (text.length() > 0)
      {
      string theText = text;
      int lineNumber = 0;
      do
         {
         string textPiece = theText.substr(0, CTextLcd::NUM_COLUMNS);
         theText = theText.substr(textPiece.length());
         SetLine(lineNumber++, textPiece, false);
         }
      while (!theText.empty() && IsValidRow(lineNumber));
      }
   }

void CTextLcd::SetBacklight(const bool isOn)
   {
   SetProperty(TL_PROP_BACKLIGHT, isOn);
   }

// RS eedat portg1
// R/W egpio10 portb2
// E egpio11 portb3
// D4 egpio12 portb4
// D5 egpio13 portb5
// D6 egpio14 portb6
// D7 egpio15 portb7


void CTextLcd::Init()
{
  // backlight-- output
  *m_p9302hw->PortADataDR() |= 0x08;

  // R/W and E low
  *m_p9302hw->PortBData() &= ~0xfc;
  *m_p9302hw->PortBDataDR() |= 0x0c;

  // RS low
  *m_p9302hw->PortGData() &= ~0x02;
  *m_p9302hw->PortGDataDR() |= 0x02;

  Clear();
}

void CTextLcd::DefineChars()
{
}

  void CTextLcd::PutNibble(unsigned char c)
{
  *m_p9302hw->PortBDataDR() = (~(c << 4)&0xf0) | (*m_p9302hw->PortBDataDR() & 0x0f);
  TL_DELAY();
  *m_p9302hw->PortBDataDR() &= ~0x08;
  TL_DELAY();
  *m_p9302hw->PortBDataDR() |= 0x08;
  TL_DELAY();

  *m_p9302hw->PortBDataDR() &= ~0xf0;
}

void CTextLcd::PutByte(unsigned char c)
{
  PutNibble(c>>4);
  PutNibble(c);
  TL_DELAYL();
}

void CTextLcd::Delay(unsigned int us)
{
  volatile unsigned int i;

  us *= 10;
  for (i=0; i<us; i++);
}

int CTextLcd::GetProperty(int property, long *value)
{
  if (value==NULL)
    return PROP_ERROR;

  switch(property)
    {
    case TL_PROP_BACKLIGHT:
      *value = *m_p9302hw->PortAData() & 0x0008 ? 1 : 0;
      break;

    case TL_PROP_HEIGHT:
      *value = CTextLcd::NUM_ROWS;
      break;

    case TL_PROP_WIDTH:
      *value = CTextLcd::NUM_COLUMNS;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}

int CTextLcd::SetProperty(int property, long value)
{
  switch (property)
    {
    case TL_PROP_BACKLIGHT:
      if (value)
	*m_p9302hw->PortAData() |= 0x08;
      else
	*m_p9302hw->PortAData() &= ~0x08;
      break;

    case TL_PROP_HEIGHT:
    case TL_PROP_WIDTH:
      return PROP_ERROR_READ_ONLY;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}
