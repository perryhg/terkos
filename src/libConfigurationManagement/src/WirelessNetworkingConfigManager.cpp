/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessNetworkingConfigManager.h"

const string WirelessNetworkingConfigManager::CONFIG_FILENAME = "wireless_networking_config.json";
const string WirelessNetworkingConfigManager::DEFAULT_CONFIG_FILENAME = "wireless_networking_config.default.json";

const string WirelessNetworkingConfigManager::WILL_START_ON_BOOTUP_PROPERTY = "wireless-networking.will-start-on-bootup";
const string WirelessNetworkingConfigManager::PROFILES_PROPERTY = "wireless-networking.profiles";
const string WirelessNetworkingConfigManager::SSID_PROPERTY = "ssid";
const string WirelessNetworkingConfigManager::IS_ENCRYPTED_PROPERTY = "is-encrypted";

Json::Value WirelessNetworkingConfigManager::parseWirelessNetworkingStatusJSONStream(redi::ipstream& is)
   {
   Json::Value wirelessNetworkingStatusJson = Json::Value::null;

   try
      {
      // parse the stream
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(is, wirelessNetworkingStatusJson, true);

      if (!parsingSuccessful)
         {
         // report to the user the failure and their locations in the document.
         cerr << "WirelessNetworkingConfigManager::parseWirelessNetworkingStatusJSONStream(): failed to parse wireless networking status JSON:" << endl
                  << reader.getFormatedErrorMessages();
         wirelessNetworkingStatusJson = Json::Value::null;
         }
      }
   catch (...)
      {
      cerr << "WirelessNetworkingConfigManager::parseWirelessNetworkingStatusJSONStream(): failed to parse wireless networking status JSON" << endl;
      wirelessNetworkingStatusJson = Json::Value::null;
      }

   return wirelessNetworkingStatusJson;
   }

const bool WirelessNetworkingConfigManager::parseJSONAndReturnWhetherWirelessNetworkingIsEnabled(const Json::Value& wirelessNetworkingStatusJson)
   {
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
               return (isEnabled != Json::Value::null && isEnabled.asBool());
               }
            }
         }
      }

   return false;
   }

const bool WirelessNetworkingConfigManager::revertToDefault()
   {
   // call superclass version to revert the JSON
   bool success = ConfigFile::revertToDefault();

   // revert wpa_supplicant.conf
   WpaSupplicantConf::revertToDefault();

   // revert interfaces
   EtcNetworkInterfacesConf::revertToDefault();

   return success;
   }

const bool WirelessNetworkingConfigManager::willStartOnBootup() const
   {
   return getBooleanValue(WILL_START_ON_BOOTUP_PROPERTY);
   }

bool WirelessNetworkingConfigManager::setWillStartOnBootup(const bool willStart)
   {
   bool success = setBooleanValue(WILL_START_ON_BOOTUP_PROPERTY, willStart);

   // update the interfaces conf
   if (success)
      {
      EtcNetworkInterfacesConf etcNetworkInterfacesConf(willStart);
      etcNetworkInterfacesConf.save();
      }

   return success;
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
         // revert JSON to default
         ConfigFile::revertToDefault();

         // set the WILL_START_ON_BOOTUP_PROPERTY
         setWillStartOnBootup(willStartOnBootupProperty->asBool());

         // create the WpaSupplicantConf file and make sure it's configred to include the catch-all network
         WpaSupplicantConf wpaSupplicantConf;
         wpaSupplicantConf.setWillIncludeCatchAllNetwork(true);

         // set the profiles
         int priority = (*profilesProperty).size();
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
                  string ssid = ssidProperty->asString();
                  bool success = addNetworkProfile(ssid);
                  if (success)
                     {
                     wpaSupplicantConf.addNetwork(ssid, priority);
                     }
                  }
               }

            priority--;
            }

         // save the WpaSupplicantConf file
         wpaSupplicantConf.save();

         return true;
         }
      }
   return false;
   }

void WirelessNetworkingConfigManager::applyConfiguration()
   {
   // load the configuration from disk
   Json::Value config;
   load(config);

   // call setJson which ensures that the config prefs are applied to the appropriate system files
   setJson(config);
   }

const bool WirelessNetworkingConfigManager::addNetworkProfile(const string& ssid)
   {
   Json::Value networkProfile;
   networkProfile[SSID_PROPERTY] = ssid;
   networkProfile[IS_ENCRYPTED_PROPERTY] = false;
   return appendObjectToArray(PROFILES_PROPERTY, networkProfile);
   }
