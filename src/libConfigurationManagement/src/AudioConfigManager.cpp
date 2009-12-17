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

#include "AudioConfigManager.h"

const unsigned char AudioConfigManager::MIN_VOLUME = 0;
const unsigned char AudioConfigManager::MAX_VOLUME = 10;
const string AudioConfigManager::CONFIG_FILENAME = "audio_config.json";
const string AudioConfigManager::DEFAULT_CONFIG_FILENAME = "audio_config.default.json";
const string AudioConfigManager::VOLUME_PROPERTY = "audio.volume.level";
const string AudioConfigManager::ARE_ALERTS_ENABLED_PROPERTY = "audio.alerts.enabled";
const string AudioConfigManager::SAMPLE_AUDIO_FILE = "/opt/media/audio/pop.wav";

const unsigned char AudioConfigManager::getVolumeLevel() const
   {
   return (char)getUnsignedIntValue(VOLUME_PROPERTY);
   }

bool AudioConfigManager::setVolumeLevel(unsigned char volume)
   {
   const unsigned char boundedVolume =  std::min(volume, MAX_VOLUME);

   try
      {
      CQEAudioController &audio = CQEAudioController::GetRef();
      audio.SetVolume(convertVolume(boundedVolume));
      CQEAudioController::Release();
      }
   catch (...)
      {
      // TODO: add logging
      cerr << "AudioConfigManager::setVolumeLevel(): failed to get CQEAudioController reference required to set volume." << endl;
      }

   return setUnsignedIntValue(VOLUME_PROPERTY, boundedVolume);
   }

const void AudioConfigManager::playSampleSound(unsigned char volume)
   {
   const unsigned char currentVolume = getVolumeLevel();

   try
      {
      CQEAudioController &audio = CQEAudioController::GetRef();
      audio.SetVolume(convertVolume(volume));
      audio.PlayClip(SAMPLE_AUDIO_FILE.c_str());
      audio.SetVolume(convertVolume(currentVolume));
      CQEAudioController::Release();
      }
   catch (...)
      {
      // TODO: add logging
      cerr << "AudioConfigManager::playSampleSound(): failed to get CQEAudioController reference required to play the sample sound." << endl;
      }
   }

const bool AudioConfigManager::areAlertsEnabled() const
   {
   return getBooleanValue(ARE_ALERTS_ENABLED_PROPERTY);
   }

bool AudioConfigManager::setAlertsEnabled(const bool isEnabled)
   {
   // TODO: toggle audio alerts in the (currently nonexistent) alerts service
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
         setVolumeLevel((char)volumeProperty->asUInt());

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

const unsigned char AudioConfigManager::convertVolume(unsigned char volume)
   {
   // map the volume from 0-10 to 0-127
   return (char)(std::min(volume, MAX_VOLUME) * 12.7);
   }
