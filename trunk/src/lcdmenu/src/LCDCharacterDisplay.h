/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef LCDCHARACTERDISPLAY_H_
#define LCDCHARACTERDISPLAY_H_

#include <algorithm>
#include <iostream>
#include <string>
#include "CharacterDisplay.h"
#include "textlcd.h"

using namespace std;

class LCDCharacterDisplay : public CharacterDisplay
   {
   public:

      static const string LCD_SCROLL_UP_ARROW;
      static const string LCD_SCROLL_DOWN_ARROW;
      static const int LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING;
      static const string LCD_PADDING_FOR_LINES_WITHOUT_ARROWS_WHEN_IN_SCROLLING_MODE;

      LCDCharacterDisplay(const unsigned int numRows, const unsigned int numColumns) :
         numRows(numRows), numColumns(numColumns), totalCharacterCount(numRows * numColumns), numColumnsWhenInScrollingMode(numColumns
                  - LCDCharacterDisplay::LCD_WIDTH_OF_SCROLL_ARROW_AND_PADDING)
         {
         // nothing to do
         }

      virtual ~LCDCharacterDisplay()
         {
         // nothing to do
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

   private:

      CTextLcd lcd;
      const unsigned int numRows;
      const unsigned int numColumns;
      const unsigned int totalCharacterCount;
      const unsigned int numColumnsWhenInScrollingMode;

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

   };

#endif /* LCDCHARACTERDISPLAY_H_ */
