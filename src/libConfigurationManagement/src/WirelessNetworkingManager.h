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

#ifndef WIRELESSNETWORKINGMANAGER_H_
#define WIRELESSNETWORKINGMANAGER_H_

#include <iostream>
#include <pstream.h>
#include <json/json.h>
#include "WirelessNetworkingConfigManager.h"

using namespace std;
using namespace redi;

class WirelessNetworkingManager
   {
   public:

      /**
       * Enables wireless networking and returns true if enabling was successful, false otherwise.
       */
      static const bool enableWirelessNetworking();

      /**
       * Enables ad-hoc wireless networking and returns true if enabling was successful, false otherwise.
       */
      static const bool enableAdHocWirelessNetworking();

      /**
       * Disables wireless networking and returns true if disabling was successful, false otherwise.
       */
      static const bool disableWirelessNetworking();

      static Json::Value getWirelessNetworkingStatus();

   private:

      WirelessNetworkingManager()
         {
         // nothing to do
         }

      virtual ~WirelessNetworkingManager()
         {
         // nothing to do
         }

   };

#endif /* WIRELESSNETWORKINGMANAGER_H_ */
