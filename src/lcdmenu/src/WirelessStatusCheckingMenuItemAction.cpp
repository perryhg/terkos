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

#include "WirelessStatusCheckingMenuItemAction.h"

const string WirelessStatusCheckingMenuItemAction::WIRELESS_MODE_MANAGED = "Managed";
const string WirelessStatusCheckingMenuItemAction::WIRELESS_MODE_AD_HOC = "Ad-Hoc";

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
                  Json::Value accessPoint = wirelessInterface["access-point"];
                  if (accessPoint != Json::Value::null)
                     {
                     if (shouldVerifyMode)
                        {
                        Json::Value mode = accessPoint["mode"];
                        if (mode != Json::Value::null && expectedMode.compare(mode.asString()) == 0)
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
                        handleWirelessEnabled();
                        }
                     }
                  else
                     {
                     handleWirelessDisabled();
                     }
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

