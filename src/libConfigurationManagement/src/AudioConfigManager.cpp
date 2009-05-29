/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "AudioConfigManager.h"

const unsigned int AudioConfigManager::MIN_VOLUME = 0;
const unsigned int AudioConfigManager::MAX_VOLUME = 10;
const string AudioConfigManager::CONFIG_FILENAME = "audio_config.json";
const string AudioConfigManager::DEFAULT_CONFIG_FILENAME = "audio_config.default.json";
const string AudioConfigManager::VOLUME_PROPERTY = "volume.level";
const string AudioConfigManager::ARE_ALERTS_ENABLED_PROPERTY = "alerts.enabled";

const unsigned int AudioConfigManager::getVolumeLevel() const
   {
   return getUnsignedIntValue(VOLUME_PROPERTY);
   }

bool AudioConfigManager::setVolumeLevel(unsigned int volume)
   {
   return setUnsignedIntValue(VOLUME_PROPERTY, std::min(volume, MAX_VOLUME));
   }

const bool AudioConfigManager::areAlertsEnabled() const
   {
   return getBooleanValue(ARE_ALERTS_ENABLED_PROPERTY);
   }

bool AudioConfigManager::setAlertsEnabled(const bool isEnabled)
   {
   return setBooleanValue(ARE_ALERTS_ENABLED_PROPERTY, isEnabled);
   }
