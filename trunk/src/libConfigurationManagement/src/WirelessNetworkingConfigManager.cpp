/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessNetworkingConfigManager.h"

const string WirelessNetworkingConfigManager::CONFIG_FILENAME = "wireless_networking_config.json";
const string WirelessNetworkingConfigManager::DEFAULT_CONFIG_FILENAME = "wireless_networking_config.default.json";
const string WirelessNetworkingConfigManager::IS_ENABLED_PROPERTY = "is-enabled";
const string WirelessNetworkingConfigManager::WILL_CONNECT_USING_SPECIFIC_PROFILE_PROPERTY = "will-connect-using-specific-profile";
const string WirelessNetworkingConfigManager::CONNECTION_ATTEMPTS_BEFORE_SWITCHING_TO_BEST_AVAILABLE_PROPERTY = "connection-attempts-before-switching-to-best-available";
const string WirelessNetworkingConfigManager::PREFERRED_PROFILE_ID_PROPERTY = "preferred-profile-id";
const string WirelessNetworkingConfigManager::PROFILES_PROPERTY = "profiles";

const bool WirelessNetworkingConfigManager::isEnabled() const
   {
   return getBooleanValue(IS_ENABLED_PROPERTY);
   }

bool WirelessNetworkingConfigManager::setEnabled(const bool isEnabled)
   {
   return setBooleanValue(IS_ENABLED_PROPERTY, isEnabled);
   }

const bool WirelessNetworkingConfigManager::willConnectUsingSpecificProfile() const
   {
   return getBooleanValue(WILL_CONNECT_USING_SPECIFIC_PROFILE_PROPERTY);
   }

bool WirelessNetworkingConfigManager::setWillConnectUsingSpecificProfile(const bool willConnectUsingSpecificProfile)
   {
   return setBooleanValue(WILL_CONNECT_USING_SPECIFIC_PROFILE_PROPERTY, willConnectUsingSpecificProfile);
   }
