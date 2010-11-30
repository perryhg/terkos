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

#ifndef WIRELESSSTATUSCHECKINGMENUITEMACTION_H_
#define WIRELESSSTATUSCHECKINGMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pstream.h>
#include <json/json.h>
#include <CharacterDisplayMenuItemAction.h>
#include <WirelessNetworkingManager.h>
#include <StringUtilities.h>

using namespace std;
using namespace redi;

class WirelessStatusCheckingMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string WIRELESS_MODE_MANAGED;
      static const string WIRELESS_MODE_AD_HOC;

      WirelessStatusCheckingMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                           CharacterDisplay* characterDisplay, map<string, string>& properties, const bool willVerifyMode = false, const string expectedWirelessMode = WIRELESS_MODE_MANAGED) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         shouldVerifyMode = willVerifyMode;
         expectedMode = expectedWirelessMode;
         }

      virtual ~WirelessStatusCheckingMenuItemAction()
         {
         // nothing to do
         }

      void activate();

   protected:

      virtual void handleWirelessEnabled() = 0;
      virtual void handleWirelessDisabled() = 0;
      virtual void handleWirelessUnplugged() = 0;
      virtual void handleWirelessStatusFailure() = 0;

   private:

      bool   shouldVerifyMode;
      string expectedMode;
   };

#endif /* WIRELESSSTATUSCHECKINGMENUITEMACTION_H_ */
