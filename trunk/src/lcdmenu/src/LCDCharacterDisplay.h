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

#ifndef LCDCHARACTERDISPLAY_H_
#define LCDCHARACTERDISPLAY_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <textlcd.h>
#include "CharacterDisplay.h"

using namespace std;

class LCDCharacterDisplay : public CharacterDisplay
   {
   public:

      LCDCharacterDisplay(const unsigned int numRows, const unsigned int numColumns) :
         lcd(NULL),
         numRows(numRows),
         numColumns(numColumns),
         totalCharacterCount(numRows * numColumns),
         numColumnsWhenInScrollingMode(numColumns - LCDCharacterDisplay::LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING),
         blankLine(numColumns, ' ')
         {
         // nothing to do
         }

      virtual ~LCDCharacterDisplay()
         {
         if (lcd)
            {
            lcd->Release();
            lcd = NULL;
            }
         }

      const unsigned int getRows() const
         {
         return numRows;
         }

      const unsigned int getColumns() const
         {
         return numColumns;
         }

      const unsigned int getColumnsWhenInScrollingMode() const
         {
         return numColumnsWhenInScrollingMode;
         }

      void setText(const string& text);
      void setText(const string& text, const bool willClearFirst);
      void setTextWithScrollArrows(const string& text);
      void setLine(const unsigned int lineNumber, const string& text);
      void setLine(const unsigned int lineNumber, const string& text, const bool willClearLineFirst);
      void setCharacter(const unsigned int row, const unsigned int col, const char character);
      void clear();
      void clearLine(const unsigned int lineNumber);
      void setBacklight(const bool isOn);

   private:

      static const char PADDING_CHARACTER;
      static const string SCROLL_ARROW_PADDING;
      static const string LCD_SCROLL_UP_ARROW;
      static const string LCD_SCROLL_DOWN_ARROW;
      static const string LCD_SCROLL_UP_ARROW_PLUS_PADDING;
      static const string LCD_SCROLL_DOWN_ARROW_PLUS_PADDING;
      static const int LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING;
      static const string LCD_PADDING_FOR_LINES_WITHOUT_ARROWS_WHEN_IN_SCROLLING_MODE;

      CTextLcd *lcd;
      const unsigned int numRows;
      const unsigned int numColumns;
      const unsigned int totalCharacterCount;
      const unsigned int numColumnsWhenInScrollingMode;
      const string blankLine;

      const bool isValidRow(const unsigned int row) const
         {
         return (row >= 0 && row < numRows);
         }

      const bool isValidColumn(const unsigned int col) const
         {
         return (col >= 0 && col < numColumns);
         }

      const bool isValidPosition(const unsigned int row, const unsigned int col)
         {
         return isValidRow(row) && isValidColumn(col);
         }

      void waitForLCDPtr();

      void releaseLCDPtrIfRequested();

   };

#endif /* LCDCHARACTERDISPLAY_H_ */
