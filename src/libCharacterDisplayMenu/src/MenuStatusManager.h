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
 * MenuStatusManager is a pure abstract base class, defining an interface for menu status managers.
 *
 * Author: Chris Bartley (bartley@cmu.edu)
 */

#ifndef MENUSTATUSMANAGER_H_
#define MENUSTATUSMANAGER_H_

#include "MenuItem.h"

class MenuStatusManager
   {
   public:

      virtual ~MenuStatusManager()
         {
         }

      virtual MenuItem* getActiveMenuItem() const = 0;
      virtual void setActiveMenuItem(MenuItem* menuItem) = 0;
      virtual void handleStartEvent() = 0;
      virtual void handleStopEvent() = 0;
      virtual void handleUpEvent() = 0;
      virtual void handleRightEvent() = 0;
      virtual void handleDownEvent() = 0;
      virtual void handleLeftEvent() = 0;
   };

#endif /* MENUSTATUSMANAGER_H_ */
