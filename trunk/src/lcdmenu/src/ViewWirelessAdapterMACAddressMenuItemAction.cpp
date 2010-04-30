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

#include "ViewWirelessAdapterMACAddressMenuItemAction.h"

const string ViewWirelessAdapterMACAddressMenuItemAction::CLASS_NAME = "ViewWirelessAdapterMACAddressMenuItemAction";

const string ViewWirelessAdapterMACAddressMenuItemAction::STATUS_FAILURE_PROPERTY = "status.failure";
const string ViewWirelessAdapterMACAddressMenuItemAction::STATUS_FAILURE_DEFAULT_VALUE = "Failed to probe wifi adapter.";

const string ViewWirelessAdapterMACAddressMenuItemAction::STATUS_UNPLUGGED_PROPERTY = "status.unplugged";
const string ViewWirelessAdapterMACAddressMenuItemAction::STATUS_UNPLUGGED_DEFAULT_VALUE = "Wireless adapteris unplugged.";

const string ViewWirelessAdapterMACAddressMenuItemAction::WIFI_ADAPTER_MAC_ADDRESS_PROPERTY = "wifi-mac-address.label";
const string ViewWirelessAdapterMACAddressMenuItemAction::WIFI_ADAPTER_MAC_ADDRESS_DEFAULT_LABEL = "Wifi MAC Address";

void ViewWirelessAdapterMACAddressMenuItemAction::activate()
   {
   cout << "ViewWirelessAdapterMACAddressMenuItemAction::activate()" << endl;

   Json::Value wirelessNetworkingStatusJson = WirelessNetworkingManager::getWirelessNetworkingStatus();

   if (wirelessNetworkingStatusJson != Json::Value::null)
      {
      Json::Value isInstalled = wirelessNetworkingStatusJson["wireless-networking-status"]["is-installed"];
      if (isInstalled != Json::Value::null)
         {
         if (isInstalled.asBool())
            {
            Json::Value wirelessInterface = wirelessNetworkingStatusJson["wireless-networking-status"]["wireless-interface"];
            if (wirelessInterface != Json::Value::null && wirelessInterface["mac-address"] != Json::Value::null)
               {
               // tokenize the MAC address, splitting on colons, so we can strip the colons out in order to diplay it on the LCD
               string macAddressWithColons = wirelessInterface["mac-address"].asString();
               string macAddress = "";
               vector<string> macAddressTokens;
               StringUtilities::tokenizeString(macAddressWithColons, macAddressTokens, ':');

               // join the tokens to create the stripped MAC address
               unsigned int i = 0;
               while (i < macAddressTokens.size())
                  {
                  macAddress.append(macAddressTokens[i++]);
                  }

               getCharacterDisplay()->setLine(0, getProperty(WIFI_ADAPTER_MAC_ADDRESS_PROPERTY, WIFI_ADAPTER_MAC_ADDRESS_DEFAULT_LABEL));
               getCharacterDisplay()->setLine(1, macAddress);
               }
            else
               {
               cout << "ViewWirelessAdapterMACAddressMenuItemAction::activate(): Wireless status failure" << endl;
               getCharacterDisplay()->setText(getProperty(STATUS_FAILURE_PROPERTY, STATUS_FAILURE_DEFAULT_VALUE));
               }
            }
         else
            {
            cout << "ViewWirelessAdapterMACAddressMenuItemAction::activate(): Wireless is unplugged" << endl;
            getCharacterDisplay()->setText(getProperty(STATUS_UNPLUGGED_PROPERTY, STATUS_UNPLUGGED_DEFAULT_VALUE));
            }
         }
      else
         {
         cout << "ViewWirelessAdapterMACAddressMenuItemAction::activate(): Wireless status failure" << endl;
         getCharacterDisplay()->setText(getProperty(STATUS_FAILURE_PROPERTY, STATUS_FAILURE_DEFAULT_VALUE));
         }
      }
   else
      {
      cout << "ViewWirelessAdapterMACAddressMenuItemAction::activate(): Wireless status failure" << endl;
      getCharacterDisplay()->setText(getProperty(STATUS_FAILURE_PROPERTY, STATUS_FAILURE_DEFAULT_VALUE));
      }
   }

// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteViewWirelessAdapterMACAddressMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadViewWirelessAdapterMACAddressMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ViewWirelessAdapterMACAddressMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ViewWirelessAdapterMACAddressMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ViewWirelessAdapterMACAddressMenuItemAction(deleteViewWirelessAdapterMACAddressMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
