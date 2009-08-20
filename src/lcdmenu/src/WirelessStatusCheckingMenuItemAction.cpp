/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessStatusCheckingMenuItemAction.h"

void WirelessStatusCheckingMenuItemAction::activate()
   {
   cout << "WirelessStatusCheckingMenuItemAction::activate()" << endl;

   Json::Value wirelessNetworkingStatusJson = WirelessNetworkingManager::getWirelessNetworkingStatus();

   if (wirelessNetworkingStatusJson != Json::Value::null)
      {
      Json::Value isInstalled = wirelessNetworkingStatusJson["wireless-networking-status"]["is-installed"];
      if (isInstalled != Json::Value::null)
         {
         if (isInstalled.asBool())
            {
            Json::Value wirelessInterface = wirelessNetworkingStatusJson["wireless-networking-status"]["wireless-interface"];
            if (wirelessInterface != Json::Value::null)
               {
               Json::Value isEnabled = wirelessInterface["is-enabled"];
               if (isEnabled != Json::Value::null && isEnabled.asBool())
                  {
                  handleWirelessEnabled();
                  }
               else
                  {
                  handleWirelessDisabled();
                  }
               }
            else
               {
               handleWirelessStatusFailure();
               }
            }
         else
            {
            handleWirelessUnplugged();
            }
         }
      else
         {
         handleWirelessStatusFailure();
         }
      }
   else
      {
      handleWirelessStatusFailure();
      }
   }

