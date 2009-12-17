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
      virtual const unsigned int getColumnsWhenInScrollingMode() const = 0;
      virtual void setText(const string& text) = 0;;
      virtual void setText(const string& text, const bool willClearFirst) = 0;;
      virtual void setTextWithScrollArrows(const string& text) = 0;;
      virtual void setLine(const unsigned int lineNumber, const string& text) = 0;;
      virtual void setLine(const unsigned int lineNumber, const string& text, const bool willClearLineFirst) = 0;;
      virtual void setCharacter(const unsigned int row, const unsigned int col, const char character) = 0;;
      virtual void clear() = 0;;
      virtual void clearLine(const unsigned int lineNumber) = 0;;
};

#endif /* CHARACTERDISPLAY_H_ */
