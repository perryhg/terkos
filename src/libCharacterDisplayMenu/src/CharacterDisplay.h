/*
 * CharacterDisplay is a pure abstract base class, defining an interface for character displays.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef CHARACTERDISPLAY_H_
#define CHARACTERDISPLAY_H_

#include <string>

using namespace std;

class CharacterDisplay
   {
   public:

      virtual ~CharacterDisplay()
         {
         }

      virtual const unsigned int getRows() const = 0;
      virtual const unsigned int getColumns() const = 0;
      virtual void setText(const string& text) = 0;;
      virtual void setText(const string& text, const bool willClearFirst) = 0;;
      virtual void setTextWithScrollArrows(const string& text) = 0;;
      virtual void setLine(const unsigned int lineNumber, const string& text) = 0;;
      virtual void setLine(const unsigned int lineNumber, const string& text, const bool willClearLineFirst) = 0;;
      virtual void setCharacter(const unsigned int row, const unsigned int col, const char character) = 0;;
      virtual void setCharacter(const unsigned int row, const unsigned int col, const string& character) = 0;;
      virtual void clear() = 0;;
      virtual void clearLine(const unsigned int lineNumber) = 0;;
};

#endif /* CHARACTERDISPLAY_H_ */
