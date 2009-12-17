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

#include "DefaultMenuStatusManager.h"

MenuItem* DefaultMenuStatusManager::getActiveMenuItem() const
   {
   return activeMenuItem;
   }

void DefaultMenuStatusManager::setActiveMenuItem(MenuItem* newMenuItem)
   {
   if (activeMenuItem != NULL)
      {
      activeMenuItem->getMenuItemAction()->deactivate();
      }
   activeMenuItem = newMenuItem;
   if (newMenuItem != NULL)
      {
      newMenuItem->getMenuItemAction()->activate();
      }
   }

void DefaultMenuStatusManager::handleStartEvent()
   {
   if (activeMenuItem != NULL)
      {
      activeMenuItem->getMenuItemAction()->start();
      }
   }

void DefaultMenuStatusManager::handleStopEvent()
   {
   if (activeMenuItem != NULL)
      {
      activeMenuItem->getMenuItemAction()->stop();
      }
   }

void DefaultMenuStatusManager::handleUpEvent()
   {
   if (activeMenuItem != NULL)
      {
      activeMenuItem->getMenuItemAction()->upEvent();
      }
   }

void DefaultMenuStatusManager::handleRightEvent()
   {
   if (activeMenuItem != NULL)
      {
      activeMenuItem->getMenuItemAction()->rightEvent();
      }
   }

void DefaultMenuStatusManager::handleDownEvent()
   {
   if (activeMenuItem != NULL)
      {
      activeMenuItem->getMenuItemAction()->downEvent();
      }
   }

void DefaultMenuStatusManager::handleLeftEvent()
   {
   if (activeMenuItem != NULL)
      {
      activeMenuItem->getMenuItemAction()->leftEvent();
      }
   }
