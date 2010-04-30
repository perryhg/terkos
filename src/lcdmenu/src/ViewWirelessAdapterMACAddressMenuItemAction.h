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

#ifndef VIEWWIRELESSADAPTERMACADDRESSMENUITEMACTION_H_
#define VIEWWIRELESSADAPTERMACADDRESSMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pstream.h>
#include <json/json.h>
#include <NoOpMenuItemAction.h>
#include <WirelessNetworkingManager.h>
#include <StringUtilities.h>

using namespace std;
using namespace redi;

class ViewWirelessAdapterMACAddressMenuItemAction : public NoOpMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string STATUS_FAILURE_PROPERTY;
      static const string STATUS_FAILURE_DEFAULT_VALUE;

      static const string STATUS_UNPLUGGED_PROPERTY;
      static const string STATUS_UNPLUGGED_DEFAULT_VALUE;

      static const string WIFI_ADAPTER_MAC_ADDRESS_PROPERTY;
      static const string WIFI_ADAPTER_MAC_ADDRESS_DEFAULT_LABEL;

      ViewWirelessAdapterMACAddressMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                   CharacterDisplay* characterDisplay, map<string, string>& properties) :
         NoOpMenuItemAction("", delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ViewWirelessAdapterMACAddressMenuItemAction()
         {
         // nothing to do
         }

      void activate();

   };

#endif /* VIEWWIRELESSADAPTERMACADDRESSMENUITEMACTION_H_ */
