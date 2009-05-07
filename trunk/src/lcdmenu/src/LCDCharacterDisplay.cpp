/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "LCDCharacterDisplay.h"

const string LCDCharacterDisplay::LCD_SCROLL_UP_ARROW = "^ ";
const string LCDCharacterDisplay::LCD_SCROLL_DOWN_ARROW = "v ";
const int LCDCharacterDisplay::LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING =
         std::max(LCDCharacterDisplay::LCD_SCROLL_UP_ARROW.length(), LCDCharacterDisplay::LCD_SCROLL_DOWN_ARROW.length());
const string
         LCDCharacterDisplay::LCD_PADDING_FOR_LINES_WITHOUT_ARROWS_WHEN_IN_SCROLLING_MODE(LCDCharacterDisplay::LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING, ' ');

void LCDCharacterDisplay::setText(const string& text)
   {
   this->setText(text, true);
   }

void LCDCharacterDisplay::setText(const string& text, const bool willClearFirst)
   {
   lcd.SetText(text, willClearFirst);
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
            textWithScrollArrows += LCDCharacterDisplay::LCD_SCROLL_UP_ARROW;
            }
         else if (line == numRows - 1)
            {
            textWithScrollArrows += LCDCharacterDisplay::LCD_SCROLL_DOWN_ARROW;
            }
         else
            {
            textWithScrollArrows += LCDCharacterDisplay::LCD_PADDING_FOR_LINES_WITHOUT_ARROWS_WHEN_IN_SCROLLING_MODE;
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
      lcd.SetLine(lineNumber, text, willClearLineFirst);
      }
   }

void LCDCharacterDisplay::setCharacter(const unsigned int row, const unsigned int col, const char character)
   {
   if (isValidPosition(row, col))
      {
      lcd.SetCharacter(row, col, character);
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
      lcd.ClearLine(lineNumber);
      }
   }

