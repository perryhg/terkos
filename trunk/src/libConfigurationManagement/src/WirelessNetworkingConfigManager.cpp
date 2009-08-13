/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessNetworkingConfigManager.h"

const string WirelessNetworkingConfigManager::CONFIG_FILENAME = "wireless_networking_config.json";
const string WirelessNetworkingConfigManager::DEFAULT_CONFIG_FILENAME = "wireless_networking_config.default.json";

const string WirelessNetworkingConfigManager::NETWORK_INTERFACES_CONF_PATH = "/etc/network/interfaces";
const string WirelessNetworkingConfigManager::DEFAULT_NETWORK_INTERFACES_CONF_PATH = "/etc/network/interfaces.default";
const string WirelessNetworkingConfigManager::WPA_SUPPLICANT_CONF_PATH = "/etc/network/wpa_supplicant.conf";
const string WirelessNetworkingConfigManager::DEFAULT_WPA_SUPPLICANT_CONF_PATH = "/etc/network/wpa_supplicant.default.conf";

const string WirelessNetworkingConfigManager::BEGIN_WLAN0_AUTOSTART = "# BEGIN_WLAN0_AUTOSTART";
const string WirelessNetworkingConfigManager::END_WLAN0_AUTOSTART = "# END_WLAN0_AUTOSTART";

const string WirelessNetworkingConfigManager::WILL_START_ON_BOOTUP_PROPERTY = "wireless-networking.will-start-on-bootup";
const string WirelessNetworkingConfigManager::PROFILES_PROPERTY = "wireless-networking.profiles";
const string WirelessNetworkingConfigManager::SSID_PROPERTY = "ssid";
const string WirelessNetworkingConfigManager::IS_ENCRYPTED_PROPERTY = "is-encrypted";

void WirelessNetworkingConfigManager::revertToDefault()
   {
   // call superclass version to revert the JSON
   ConfigFile::revertToDefault();

   // revert wpa_supplicant.conf
   copyFile(DEFAULT_WPA_SUPPLICANT_CONF_PATH, WPA_SUPPLICANT_CONF_PATH);

   // revert interfaces
   copyFile(DEFAULT_NETWORK_INTERFACES_CONF_PATH, NETWORK_INTERFACES_CONF_PATH);
   }

void WirelessNetworkingConfigManager::copyFile(const string& source, const string& destination)
   {
   string copyCommand = "cp " + source + " " + destination;
   system(copyCommand.c_str());
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
      // read in the existing conf line-by-line until we find the BEGIN_WLAN0_AUTOSTART line
      ostringstream theFile(ostringstream::out);
      bool foundAutoStartMarker = false;
      ifstream ifs(NETWORK_INTERFACES_CONF_PATH.c_str(), ifstream::in);
      string line;
      while (ifs.good())
         {
         getline(ifs, line);
         theFile << line << endl;
         if (line == BEGIN_WLAN0_AUTOSTART)
            {
            foundAutoStartMarker = true;
            break;
            }
         }

      if (foundAutoStartMarker)
         {
         // now write the auto start line appropriately (with or without the comment character)
         if (!willStart)
            {
            theFile << "#";
            }
         theFile << "auto wlan0" << endl;

         // read and discard any lines until we find the END_WLAN0_AUTOSTART line
         while (ifs.good())
            {
            getline(ifs, line);

            if (line == END_WLAN0_AUTOSTART)
               {
               theFile << line << endl;
               break;
               }
            }
         }

      // now just read and copy the rest of the file
      while (ifs.good())
         {
         getline(ifs, line);
         theFile << line << endl;
         }

      // close the input
      ifs.close();

      // open the file for writing and write our new version
      ofstream ofs(NETWORK_INTERFACES_CONF_PATH.c_str());

      // trim the file to get rid of extra line breaks at the end
      ofs << StringUtilities::trim(theFile.str()) << endl;

      // close the interfaces file
      ofs.close();
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

         // open the output stream to write to the wpa_supplicant.conf file
         ofstream wpa_supplicant_conf(WPA_SUPPLICANT_CONF_PATH.c_str());

         // write the header stuff
         wpa_supplicant_conf << "ctrl_interface=/var/run/wpa_supplicant" << endl;
         wpa_supplicant_conf << "ctrl_interface_group=0" << endl;

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
                     wpa_supplicant_conf << endl;
                     wpa_supplicant_conf << "network={" << endl;
                     wpa_supplicant_conf << "        ssid=\"" << ssid << "\"" << endl;
                     wpa_supplicant_conf << "        key_mgmt=NONE" << endl;
                     wpa_supplicant_conf << "        priority=" << priority << endl;
                     wpa_supplicant_conf << "}" << endl;
                     }
                  }
               }

            priority--;
            }

         // write the catch-all network which will cause it to connect to any network
         wpa_supplicant_conf << endl;
         wpa_supplicant_conf << "network={" << endl;
         wpa_supplicant_conf << "        key_mgmt=NONE" << endl;
         wpa_supplicant_conf << "        priority=0" << endl;
         wpa_supplicant_conf << "}" << endl;

         // close the wpa_supplicant.conf file
         wpa_supplicant_conf.close();

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
