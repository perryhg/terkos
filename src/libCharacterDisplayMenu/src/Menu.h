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
