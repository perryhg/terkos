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
