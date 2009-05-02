/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef DEFAULTMENUITEMACTION_H_
#define DEFAULTMENUITEMACTION_H_

#include "AbstractMenuItemAction.h"

class DefaultMenuItemAction : public AbstractMenuItemAction
   {
   public:

      DefaultMenuItemAction(MenuItem* menuItem, MenuStatusManager* menuStatusManager) :
         AbstractMenuItemAction(menuItem, menuStatusManager)
         {
         // nothing to do
         }

      virtual ~DefaultMenuItemAction()
         {
         // nothing to do
         }

      virtual void start();

      virtual void stop();

      virtual void upEvent();

      virtual void downEvent();
};

#endif /* DEFAULTMENUITEMACTION_H_ */
