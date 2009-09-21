/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "AudioConfigManager.h"

const unsigned int AudioConfigManager::MIN_VOLUME = 0;
const unsigned int AudioConfigManager::MAX_VOLUME = 10;
const string AudioConfigManager::CONFIG_FILENAME = "audio_config.json";
const string AudioConfigManager::DEFAULT_CONFIG_FILENAME = "audio_config.default.json";
const string AudioConfigManager::VOLUME_PROPERTY = "audio.volume.level";
const string AudioConfigManager::ARE_ALERTS_ENABLED_PROPERTY = "audio.alerts.enabled";

const unsigned int AudioConfigManager::getVolumeLevel() const
   {
   return getUnsignedIntValue(VOLUME_PROPERTY);
   }

bool AudioConfigManager::setVolumeLevel(unsigned int volume)
   {
   // TODO: also set the volume in libqwerk or somesuch
   return setUnsignedIntValue(VOLUME_PROPERTY, std::min(volume, MAX_VOLUME));
   }

const bool AudioConfigManager::areAlertsEnabled() const
   {
   return getBooleanValue(ARE_ALERTS_ENABLED_PROPERTY);
   }

bool AudioConfigManager::setAlertsEnabled(const bool isEnabled)
   {
   // TODO: also set the volume in libqwerk or somesuch (?)
   return setBooleanValue(ARE_ALERTS_ENABLED_PROPERTY, isEnabled);
   }

Json::Value AudioConfigManager::getJSON()
   {
   Json::Value config;
   if (!load(config))
      {
      cerr << "AudioConfigManager::getJSON(): failed to load config file, returning null JSON instead." << endl;
      return Json::Value::null;
      }

   return config;
   }

const bool AudioConfigManager::setJson(Json::Value& config)
   {
   if (config != Json::Value::null)
      {
      // now do some simple tests to verify it
      Json::Value* volumeProperty = ConfigFile::findProperty(config, VOLUME_PROPERTY);
      Json::Value* areAlertsEnabledProperty = ConfigFile::findProperty(config, ARE_ALERTS_ENABLED_PROPERTY);
      if (volumeProperty != NULL && areAlertsEnabledProperty != NULL)
         {
         // revert to default
         revertToDefault();

         // set the VOLUME_PROPERTY
         setVolumeLevel(volumeProperty->asUInt());

         // set the ARE_ALERTS_ENABLED_PROPERTY
         setAlertsEnabled(areAlertsEnabledProperty->asBool());

         return true;
         }
      }
   return false;
   }

void AudioConfigManager::applyConfiguration()
   {
   // load the configuration from disk
   Json::Value config;
   load(config);

   // call setJson which ensures that the config prefs are applied to the system
   setJson(config);
   }
