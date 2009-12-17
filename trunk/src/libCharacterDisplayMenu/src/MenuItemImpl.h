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

#ifndef MENUITEMIMPL_H_
#define MENUITEMIMPL_H_

#include <vector>

using namespace std;

#include "MenuItem.h"

class MenuItemImpl : public MenuItem
   {
   public:

      MenuItemImpl(const string& text) :
         text(text)
         {
         parent = NULL;
         previousSibling = NULL;
         nextSibling = NULL;
         action = NULL;
         }

      MenuItemImpl(const string& text, vector<MenuItemImpl*> theChildren) :
         text(text)
         {
         this->children = theChildren;
         parent = NULL;
         previousSibling = NULL;
         nextSibling = NULL;
         action = NULL;
         }

      ~MenuItemImpl()
         {
         // remove links to parent and siblings
         parent = NULL;
         previousSibling = NULL;
         nextSibling = NULL;

         // delete the action, if any
         if (action != NULL)
            {
            // TODO: I think I need to delete using action->deleteSelf() or somesuch.
            //delete action;
            }

         // TODO: delete each of the children (I think I can just call children.clear())
         }

      string getText() const
         {
         return text;
         }

      MenuItem* getParent() const
         {
         return parent;
         }

      MenuItem* getFirstChild() const
         {
         if (!children.empty())
            {
            return children[0];
            }

         return NULL;
         }

      MenuItem* getPreviousSibling() const
         {
         return previousSibling;
         }

      MenuItem* getNextSibling() const
         {
         return nextSibling;
         }

      bool hasSiblings() const
         {
         return (previousSibling != this && nextSibling != this);
         }

      bool hasChildren() const
         {
         return !children.empty();
         }

      bool isRoot() const
         {
         return parent == NULL;
         }

      MenuItemAction* getMenuItemAction() const
         {
         return action;
         }

   private:
      friend class CharacterDisplayMenu;

      string text;
      vector<MenuItemImpl*> children;
      MenuItem* parent;
      MenuItem* previousSibling;
      MenuItem* nextSibling;
      MenuItemAction* action;

      void setParent(MenuItem* parent)
         {
         this->parent = parent;
         }

      void setPreviousSibling(MenuItem* sibling)
         {
         previousSibling = sibling;
         }

      void setNextSibling(MenuItem* sibling)
         {
         nextSibling = sibling;
         }

      void setMenuItemAction(MenuItemAction* action)
         {
         this->action = action;
         }
   };

#endif /* MENUITEMIMPL_H_ */
