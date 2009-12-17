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
