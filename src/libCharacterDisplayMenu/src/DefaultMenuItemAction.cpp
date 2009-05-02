/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "DefaultMenuItemAction.h"

void DefaultMenuItemAction::start()
   {
   if (getMenuItem()->hasChildren())
      {
      getMenuStatusManager()->setActiveMenuItem(getMenuItem()->getFirstChild());
      }
   }

void DefaultMenuItemAction::stop()
   {
   MenuItem* parent = getMenuItem()->getParent();
   if (!parent->isRoot())
      {
      getMenuStatusManager()->setActiveMenuItem(parent);
      }
   }

void DefaultMenuItemAction::upEvent()
   {
   MenuItem* self = getMenuItem();
   MenuItem* sibling = self->getPreviousSibling();
   if (self != sibling) // TODO: will this work?
      {
      getMenuStatusManager()->setActiveMenuItem(sibling);
      }
   }

void DefaultMenuItemAction::downEvent()
   {
   MenuItem* self = getMenuItem();
   MenuItem* sibling = self->getNextSibling();
   if (self != sibling) // TODO: will this work?
      {
      getMenuStatusManager()->setActiveMenuItem(sibling);
      }
   }
