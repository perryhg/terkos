/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef LCDCONFIGMANAGER_H_
#define LCDCONFIGMANAGER_H_

#include <algorithm>
#include "ConfigFile.h"
#include "textlcd.h"

using namespace std;

class LCDConfigManager : public ConfigFile
   {
   public:

      LCDConfigManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~LCDConfigManager()
         {
         // nothing to do
         }

      const bool isBacklightEnabled() const;

      bool setBacklightEnabled(const bool isEnabled);

      Json::Value getJSON();

      const bool setJson(Json::Value& config);

      virtual void applyConfiguration();

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string IS_BACKLIGHT_ENABLED;

   };

#endif /* LCDCONFIGMANAGER_H_ */
