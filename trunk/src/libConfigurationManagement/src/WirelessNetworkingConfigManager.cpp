/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessNetworkingConfigManager.h"

const string WirelessNetworkingConfigManager::CONFIG_FILENAME = "wireless_networking_config.json";
const string WirelessNetworkingConfigManager::DEFAULT_CONFIG_FILENAME = "wireless_networking_config.default.json";
const string WirelessNetworkingConfigManager::WILL_START_ON_BOOTUP_PROPERTY = "will-start-on-bootup";
const string WirelessNetworkingConfigManager::PROFILES_PROPERTY = "profiles";
const string WirelessNetworkingConfigManager::SSID_PROPERTY = "ssid";
const string WirelessNetworkingConfigManager::IS_ENCRYPTED_PROPERTY = "is-encrypted";

const bool WirelessNetworkingConfigManager::willStartOnBootup() const
   {
   return getBooleanValue(WILL_START_ON_BOOTUP_PROPERTY);
   }

bool WirelessNetworkingConfigManager::setWillStartOnBootup(const bool willStart)
   {
   return setBooleanValue(WILL_START_ON_BOOTUP_PROPERTY, willStart);
   }

const bool WirelessNetworkingConfigManager::setJson(Json::Value& config)
   {
   if (config != Json::Value::null)
      {
      // now do some simple tests to verify it
      Json::Value* willStartOnBootupProperty = ConfigFile::findProperty(config, WILL_START_ON_BOOTUP_PROPERTY);
      Json::Value* profilesProperty = ConfigFile::findProperty(config, PROFILES_PROPERTY);
      if (willStartOnBootupProperty != NULL && profilesProperty != NULL)
         {
         // revert to default
         revertToDefault();

         // set the WILL_START_ON_BOOTUP_PROPERTY
         setWillStartOnBootup(willStartOnBootupProperty->asBool());

         // set the profiles
         for (unsigned int i = 0; i < (*profilesProperty).size(); i++)
            {
            // get the property
            Json::Value profileProperty = (*profilesProperty)[i];
            if (profileProperty != Json::Value::null)
               {
               // TODO: this (and the addNetworkProfile() method) will evolve over time as we add
               // support for encryption and such, but for now it's nothing more than specifying
               // the SSID
               Json::Value* ssidProperty = ConfigFile::findProperty(profileProperty, SSID_PROPERTY);
               if (ssidProperty != NULL && *ssidProperty != Json::Value::null)
                  {
                  addNetworkProfile(ssidProperty->asString());
                  }
               }
            }

         return true;
         }
      }
   return false;
   }

bool WirelessNetworkingConfigManager::addNetworkProfile(const string& ssid)
   {
   Json::Value networkProfile;
   networkProfile[SSID_PROPERTY] = ssid;
   networkProfile[IS_ENCRYPTED_PROPERTY] = false;
   return appendObjectToArray(PROFILES_PROPERTY, networkProfile);
   }
