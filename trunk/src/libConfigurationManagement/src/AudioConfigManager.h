/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef AUDIOCONFIGMANAGER_H_
#define AUDIOCONFIGMANAGER_H_

#include <algorithm>
#include "ConfigFile.h"

using namespace std;

class AudioConfigManager : public ConfigFile
   {
   public:

      static const unsigned int MIN_VOLUME;
      static const unsigned int MAX_VOLUME;

      AudioConfigManager() :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME)
         {
         // nothing to do
         }

      virtual ~AudioConfigManager()
         {
         // nothing to do
         }

      const unsigned int getVolumeLevel() const;

      bool setVolumeLevel(unsigned int volume);

      const bool areAlertsEnabled() const;

      bool setAlertsEnabled(const bool isEnabled);

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string VOLUME_PROPERTY;
      static const string ARE_ALERTS_ENABLED_PROPERTY;

   };

#endif /* AUDIOCONFIGMANAGER_H_ */
