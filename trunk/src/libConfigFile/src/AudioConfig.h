/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef AUDIOCONFIG_H_
#define AUDIOCONFIG_H_

#include <algorithm>
#include "ConfigFile.h"

using namespace std;

class AudioConfig : public ConfigFile
   {
   public:

      static const unsigned int MAX_VOLUME;

      AudioConfig() :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME)
         {
         // nothing to do
         }

      virtual ~AudioConfig()
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

#endif /* AUDIOCONFIG_H_ */
