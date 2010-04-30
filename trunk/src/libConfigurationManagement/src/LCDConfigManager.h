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

#ifndef LCDCONFIGMANAGER_H_
#define LCDCONFIGMANAGER_H_

#include <algorithm>
#include "ConfigFile.h"

using namespace std;

class LCDConfigManager : public ConfigFile
   {
   public:

      static const int BACKLIGHT_TIMEOUT_VALUE_ALWAYS_ON;
      static const int BACKLIGHT_TIMEOUT_VALUE_ALWAYS_OFF;

      LCDConfigManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~LCDConfigManager()
         {
         // nothing to do
         }

      const int getBacklightTimeout() const;

      bool setBacklightTimeout(const int timeoutInSeconds);

      Json::Value getJSON();

      const bool setJson(Json::Value& config);

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string BACKLIGHT_TIMEOUT_SECONDS;

   };

#endif /* LCDCONFIGMANAGER_H_ */
