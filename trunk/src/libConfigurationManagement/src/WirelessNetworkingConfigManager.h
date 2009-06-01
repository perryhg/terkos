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

      WirelessNetworkingConfigManager() :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME)
         {
         // nothing to do
         }

      virtual ~WirelessNetworkingConfigManager()
         {
         // nothing to do
         }

      const bool isEnabled() const;

      bool setEnabled(const bool isEnabled);

      const bool willConnectUsingSpecificProfile() const;

      bool setWillConnectUsingSpecificProfile(const bool willConnectUsingSpecificProfile);

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string IS_ENABLED_PROPERTY;
      static const string WILL_CONNECT_USING_SPECIFIC_PROFILE_PROPERTY;
      static const string CONNECTION_ATTEMPTS_BEFORE_SWITCHING_TO_BEST_AVAILABLE_PROPERTY;
      static const string PREFERRED_PROFILE_ID_PROPERTY;
      static const string PROFILES_PROPERTY;
   };

#endif /* WIRELESSNETWORKINGCONFIGMANAGER_H_ */
