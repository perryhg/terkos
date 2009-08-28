/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WIRELESSNETWORKINGCONFIGMANAGER_H_
#define WIRELESSNETWORKINGCONFIGMANAGER_H_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <pstream.h>
#include <json/json.h>
#include "ConfigFile.h"
#include "WpaSupplicantConf.h"
#include "EtcNetworkInterfacesConf.h"

using namespace std;
using namespace redi;

class WirelessNetworkingConfigManager : public ConfigFile
   {
   public:

      static Json::Value parseWirelessNetworkingStatusJSONStream(redi::ipstream& is);
      static const bool parseJSONAndReturnWhetherWirelessNetworkingIsEnabled(const Json::Value& json);

      WirelessNetworkingConfigManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~WirelessNetworkingConfigManager()
         {
         // nothing to do
         }

      virtual const bool revertToDefault();

      const bool willStartOnBootup() const;

      bool setWillStartOnBootup(const bool willStart);

      const bool setJson(Json::Value& config);

      /**
       * Reads in the config and applies it to /etc/network/interfaces and /etc/network/wpa_supplicant.conf as
       * a way of ensuring that the user's saved configuration is correctly reflected in the system's config
       * files.
       */
      virtual void applyConfiguration();

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;

      static const string WILL_START_ON_BOOTUP_PROPERTY;
      static const string PROFILES_PROPERTY;
      static const string SSID_PROPERTY;
      static const string IS_ENCRYPTED_PROPERTY;

      const bool addNetworkProfile(const string& ssid);

   };

#endif /* WIRELESSNETWORKINGCONFIGMANAGER_H_ */
