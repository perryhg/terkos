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
