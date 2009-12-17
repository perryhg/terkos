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
