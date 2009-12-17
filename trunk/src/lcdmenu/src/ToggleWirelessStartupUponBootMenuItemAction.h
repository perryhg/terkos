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

#ifndef TOGGLEWIRELESSSTARTUPUPONBOOTMENUITEMACTION_H_
#define TOGGLEWIRELESSSTARTUPUPONBOOTMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "WirelessNetworkingConfigManager.h"

class ToggleWirelessStartupUponBootMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ToggleWirelessStartupUponBootMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                      CharacterDisplay* characterDisplay, map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ToggleWirelessStartupUponBootMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   private:

      WirelessNetworkingConfigManager configManager;

      void setWillStartOnBootup(const bool willStartOnBootup);
   };

#endif /* TOGGLEWIRELESSSTARTUPUPONBOOTMENUITEMACTION_H_ */
