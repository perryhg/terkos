/*
 * Menu is a pure abstract base class, defining an interface for menus.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef MENU_H_
#define MENU_H_

#include "MenuItem.h"

class Menu
   {
   public:

      virtual ~Menu()
         {
         }

      virtual MenuItem* getDefaultMenuItem() const = 0;
   };

#endif /* MENU_H_ */
