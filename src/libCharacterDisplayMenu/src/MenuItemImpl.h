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
