/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessNetworkingConfigManager.h"

const string WirelessNetworkingConfigManager::CONFIG_FILENAME = "wireless_networking_config.json";
const string WirelessNetworkingConfigManager::DEFAULT_CONFIG_FILENAME = "wireless_networking_config.default.json";
const string WirelessNetworkingConfigManager::WILL_START_ON_BOOTUP_PROPERTY = "will-start-on-bootup";
const string WirelessNetworkingConfigManager::PROFILES_PROPERTY = "profiles";

const bool WirelessNetworkingConfigManager::willStartOnBootup() const
   {
   return getBooleanValue(WILL_START_ON_BOOTUP_PROPERTY);
   }

bool WirelessNetworkingConfigManager::setWillStartOnBootup(const bool willStart)
   {
   return setBooleanValue(WILL_START_ON_BOOTUP_PROPERTY, willStart);
   }
