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
