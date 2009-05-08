/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "AudioConfig.h"

const unsigned int AudioConfig::MIN_VOLUME = 0;
const unsigned int AudioConfig::MAX_VOLUME = 10;
const string AudioConfig::CONFIG_FILENAME = "audio.cfg";
const string AudioConfig::DEFAULT_CONFIG_FILENAME = "audio.default.cfg";
const string AudioConfig::VOLUME_PROPERTY = "volume.level";
const string AudioConfig::ARE_ALERTS_ENABLED_PROPERTY = "alerts.enabled";

const unsigned int AudioConfig::getVolumeLevel() const
   {
   return getUnsignedIntValue(VOLUME_PROPERTY);
   }

bool AudioConfig::setVolumeLevel(unsigned int volume)
   {
   return setUnsignedIntValue(VOLUME_PROPERTY, std::min(volume, MAX_VOLUME));
   }

const bool AudioConfig::areAlertsEnabled() const
   {
   return getBooleanValue(ARE_ALERTS_ENABLED_PROPERTY);
   }

bool AudioConfig::setAlertsEnabled(const bool isEnabled)
   {
   return setBooleanValue(ARE_ALERTS_ENABLED_PROPERTY, isEnabled);
   }
