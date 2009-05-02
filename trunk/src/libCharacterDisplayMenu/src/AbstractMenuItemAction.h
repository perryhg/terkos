/*
 * AbstractMenuItemAction is an adapter class for MenuItemActions. The methods in this
 * class which implement MenuItemAction are empty. This class exists as a convenience for creating
 * MenuItemActions.
 *
 * Extend this class to create a MenuItemAction and override the methods for the events of interest. (If you
 * implement the MenuItemAction interface, you have to define all of the methods in it. This class
 * defines no-op methods for them all, so you only have to define methods for events you care about.)
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef ABSTRACTMENUITEMACTION_H_
#define ABSTRACTMENUITEMACTION_H_

#include "MenuItem.h"
#include "MenuItemAction.h"
#include "MenuStatusManager.h"

class AbstractMenuItemAction : public MenuItemAction
   {
   public:

      AbstractMenuItemAction(MenuItem* menuItem, MenuStatusManager* menuStatusManager) :
         menuItem(menuItem), menuStatusManager(menuStatusManager)
         {
         }

      virtual ~AbstractMenuItemAction()
         {
         // no need to do anything since the objects weren't created here
         }

      virtual void activate()
         {
         }

      virtual void deactivate()
         {
         }

      virtual void start()
         {
         }

      virtual void stop()
         {
         }

      virtual void upEvent()
         {
         }

      virtual void rightEvent()
         {
         }

      virtual void downEvent()
         {
         }

      virtual void leftEvent()
         {
         }

   protected:

      MenuItem* getMenuItem()
         {
         return menuItem;
         }

      MenuStatusManager* getMenuStatusManager()
         {
         return menuStatusManager;
         }

   private:

      MenuItem* menuItem;
      MenuStatusManager* menuStatusManager;
   };

#endif /* ABSTRACTMENUITEMACTION_H_ */
