/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WIRELESSNETWORKINGCONFIGMANAGER_H_
#define WIRELESSNETWORKINGCONFIGMANAGER_H_

#include <algorithm>
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

      const bool willStartOnBootup() const;

      bool setWillStartOnBootup(const bool willStart);

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string WILL_START_ON_BOOTUP_PROPERTY;
      static const string PROFILES_PROPERTY;
   };

#endif /* WIRELESSNETWORKINGCONFIGMANAGER_H_ */
