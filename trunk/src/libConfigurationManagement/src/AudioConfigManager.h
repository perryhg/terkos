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

#ifndef AUDIOCONFIGMANAGER_H_
#define AUDIOCONFIGMANAGER_H_

#include <algorithm>
#include "ConfigFile.h"
#include "qeaudio.h"

using namespace std;

class AudioConfigManager : public ConfigFile
   {
   public:

      static const unsigned char MIN_VOLUME;
      static const unsigned char MAX_VOLUME;

      AudioConfigManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~AudioConfigManager()
         {
         // nothing to do
         }

      const unsigned char getVolumeLevel() const;

      bool setVolumeLevel(unsigned char volume);

      const void playSampleSound(unsigned char volume);

      const bool areAlertsEnabled() const;

      bool setAlertsEnabled(const bool isEnabled);

      Json::Value getJSON();

      const bool setJson(Json::Value& config);

      virtual void applyConfiguration();

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string VOLUME_PROPERTY;
      static const string ARE_ALERTS_ENABLED_PROPERTY;
      static const string SAMPLE_AUDIO_FILE;

      const unsigned char convertVolume(unsigned char volume);

   };

#endif /* AUDIOCONFIGMANAGER_H_ */
