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
 * Menu is a pure abstract base class, defining an interface for menus.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef MENU_H_
#define MENU_H_

#include <string>
#include "MenuItem.h"

using namespace std;

class Menu
   {
   public:

      virtual ~Menu()
         {
         }

      virtual MenuItem* getDefaultMenuItem() const = 0;
      virtual const bool hasWelcomeText() const = 0;
      virtual const string getWelcomeText() const = 0;
   };

#endif /* MENU_H_ */
