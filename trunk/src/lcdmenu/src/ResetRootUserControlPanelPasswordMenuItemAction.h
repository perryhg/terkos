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

#ifndef RESETROOTUSERCONTROLPANELPASSWORDMENUITEMACTION_H_
#define RESETROOTUSERCONTROLPANELPASSWORDMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "RootUserPasswordManager.h"

class ResetRootUserControlPanelPasswordMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetRootUserControlPanelPasswordMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetRootUserControlPanelPasswordMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   };

#endif /* RESETROOTUSERCONTROLPANELPASSWORDMENUITEMACTION_H_ */
