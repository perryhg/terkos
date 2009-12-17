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
