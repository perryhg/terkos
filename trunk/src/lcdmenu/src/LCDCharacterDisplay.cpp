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

/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "LCDCharacterDisplay.h"

const char LCDCharacterDisplay::PADDING_CHARACTER = ' ';
const string LCDCharacterDisplay::SCROLL_ARROW_PADDING(1, PADDING_CHARACTER);
const string LCDCharacterDisplay::LCD_SCROLL_UP_ARROW = TL_STRING_UP;
const string LCDCharacterDisplay::LCD_SCROLL_DOWN_ARROW = TL_STRING_DOWN;
const string LCDCharacterDisplay::LCD_SCROLL_UP_ARROW_PLUS_PADDING = LCD_SCROLL_UP_ARROW + SCROLL_ARROW_PADDING;
const string LCDCharacterDisplay::LCD_SCROLL_DOWN_ARROW_PLUS_PADDING = LCD_SCROLL_DOWN_ARROW + SCROLL_ARROW_PADDING;
const int LCDCharacterDisplay::LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING = std::max(LCD_SCROLL_UP_ARROW_PLUS_PADDING.length(), LCD_SCROLL_DOWN_ARROW_PLUS_PADDING.length());
const string LCDCharacterDisplay::LCD_PADDING_FOR_LINES_WITHOUT_ARROWS_WHEN_IN_SCROLLING_MODE(LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING, PADDING_CHARACTER);

void LCDCharacterDisplay::setText(const string& text)
   {
   this->setText(text, true);
   }

void LCDCharacterDisplay::setText(const string& text, const bool willClearFirst)
   {
   if (willClearFirst)
      {
      lcd.Clear();
      }

   if (text.length() > 0)
      {
      string theText = text;
      int lineNumber = 0;
      do
         {
         string textPiece = theText.substr(0, CTextLcd::NUM_COLUMNS);
         theText = theText.substr(textPiece.length());
         setLine(lineNumber++, textPiece, false);
         }
      while (!theText.empty() && isValidRow(lineNumber));
      }
   }

void LCDCharacterDisplay::setTextWithScrollArrows(const string& text)
   {
   if (text.length() > 0)
      {
      string theText = text;

      string textWithScrollArrows;
      for (unsigned int line = 0; line < numRows; line++)
         {
         // chop off at most the first numColumnsWhenInScrollingMode characters
         string lineText = theText.substr(0, std::min(numColumnsWhenInScrollingMode, (const unsigned int) (theText.length())));

         // update the remainder
         theText = theText.substr(lineText.length());

         if (theText.length() == 0)
            {
            lineText.resize(numColumnsWhenInScrollingMode, ' ');
            }

         if (line == 0)
            {
            textWithScrollArrows += LCD_SCROLL_UP_ARROW_PLUS_PADDING;
            }
         else if (line == numRows - 1)
            {
            textWithScrollArrows += LCD_SCROLL_DOWN_ARROW_PLUS_PADDING;
            }
         else
            {
            textWithScrollArrows += LCD_PADDING_FOR_LINES_WITHOUT_ARROWS_WHEN_IN_SCROLLING_MODE;
            }

         textWithScrollArrows += lineText;
         }

      this->setText(textWithScrollArrows);
      }
   }

void LCDCharacterDisplay::setLine(const unsigned int lineNumber, const string& text)
   {
   this->setLine(lineNumber, text, true);
   }

void LCDCharacterDisplay::setLine(const unsigned int lineNumber, const string& text, const bool willClearLineFirst)
   {
   if (this->isValidRow(lineNumber))
      {
      if (willClearLineFirst)
         {
         clearLine(lineNumber);
         }
      if (text.length() > 0)
         {
         lcd.MoveCursor(lineNumber, 0);
         lcd.printf("%s", text.substr(0, numColumns).c_str());
         }
      }
   }

void LCDCharacterDisplay::setCharacter(const unsigned int row, const unsigned int col, const char character)
   {
   if (isValidPosition(row, col))
      {
      lcd.MoveCursor(row, col);
      lcd.SetCharacter(character);
      }
   }

void LCDCharacterDisplay::clear()
   {
   lcd.Clear();
   }

void LCDCharacterDisplay::clearLine(const unsigned int lineNumber)
   {
   if (isValidRow(lineNumber))
      {
      lcd.MoveCursor(lineNumber, 0);
      lcd.printf("%s", blankLine.c_str());
      }
   }

void LCDCharacterDisplay::setBacklight(const bool isOn)
   {
   lcd.SetProperty(TL_PROP_BACKLIGHT, isOn);
   }
