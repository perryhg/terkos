/*
 * MenuItem is a pure abstract base class, defining an interface for menu items.
 *
 * Author: Chris Bartley (bartley@cmu.edu)
 */

#ifndef MENUITEM_H_
#define MENUITEM_H_

#include <string>
#include "MenuItemAction.h"

using namespace std;

class MenuItem
   {
   public:

      virtual ~MenuItem()
         {
         }

      virtual string getText() const = 0;
      virtual MenuItem* getParent() const = 0;
      virtual MenuItem* getFirstChild() const = 0;
      virtual MenuItem* getPreviousSibling() const = 0;
      virtual MenuItem* getNextSibling() const = 0;
      virtual bool hasSiblings() const = 0;
      virtual bool hasChildren() const = 0;
      virtual bool isRoot() const = 0;
      virtual MenuItemAction* getMenuItemAction() const = 0;
   };

#endif /* MENUITEM_H_ */
