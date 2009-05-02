/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef DEFAULTMENUSTATUSMANAGER_H_
#define DEFAULTMENUSTATUSMANAGER_H_

#include <pthread.h>
#include "MenuStatusManager.h"
#include "MenuItem.h"

class DefaultMenuStatusManager : public MenuStatusManager
   {
   public:

      DefaultMenuStatusManager() :
         MenuStatusManager()
         {
         activeMenuItem = NULL;
         }
      virtual ~DefaultMenuStatusManager()
         {
         // nothing to do
         }

      MenuItem* getActiveMenuItem() const;
      void setActiveMenuItem(MenuItem* menuItem);
      void handleStartEvent();
      void handleStopEvent();
      void handleUpEvent();
      void handleRightEvent();
      void handleDownEvent();
      void handleLeftEvent();

   private:

      MenuItem* activeMenuItem;
   };

#endif /* DEFAULTMENUSTATUSMANAGER_H_ */
