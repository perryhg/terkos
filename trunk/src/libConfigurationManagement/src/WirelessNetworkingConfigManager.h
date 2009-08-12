/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WIRELESSNETWORKINGCONFIGMANAGER_H_
#define WIRELESSNETWORKINGCONFIGMANAGER_H_

#include <algorithm>
#include <json/json.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ConfigFile.h"

using namespace std;

class WirelessNetworkingConfigManager : public ConfigFile
   {
   public:

      WirelessNetworkingConfigManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~WirelessNetworkingConfigManager()
         {
         // nothing to do
         }

      virtual void revertToDefault();

      const bool willStartOnBootup() const;

      bool setWillStartOnBootup(const bool willStart);

      const bool setJson(Json::Value& config);

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;

      static const string NETWORK_INTERFACES_CONF_PATH;
      static const string DEFAULT_NETWORK_INTERFACES_CONF_PATH;
      static const string WPA_SUPPLICANT_CONF_PATH;
      static const string DEFAULT_WPA_SUPPLICANT_CONF_PATH;

      static const string BEGIN_WLAN0_AUTOSTART;
      static const string END_WLAN0_AUTOSTART;

      static const string WILL_START_ON_BOOTUP_PROPERTY;
      static const string PROFILES_PROPERTY;
      static const string SSID_PROPERTY;
      static const string IS_ENCRYPTED_PROPERTY;

      bool addNetworkProfile(const string& ssid);

      void copyFile(const string& source, const string& destination);
   };

#endif /* WIRELESSNETWORKINGCONFIGMANAGER_H_ */
