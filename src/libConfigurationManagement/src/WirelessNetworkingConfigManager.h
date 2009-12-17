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

      Json::Value getJSON();

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
