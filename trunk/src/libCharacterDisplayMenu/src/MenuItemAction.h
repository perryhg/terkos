/*
 * MenuItemAction is a pure abstract base class, defining an interface for menu item actions.
 *
 * Author: Chris Bartley (bartley@cmu.edu)
 */

#ifndef MENUITEMACTION_H_
#define MENUITEMACTION_H_

class MenuItemAction
   {
   public:

      virtual ~MenuItemAction()
         {
         }

      virtual void activate() = 0;
      virtual void deactivate() = 0;
      virtual void start() = 0;
      virtual void stop() = 0;
      virtual void upEvent() = 0;
      virtual void rightEvent() = 0;
      virtual void downEvent() = 0;
      virtual void leftEvent() = 0;
   };

#endif /* MENUITEMACTION_H_ */
