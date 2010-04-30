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

#include "LCDConfigManager.h"

const int LCDConfigManager::BACKLIGHT_TIMEOUT_VALUE_ALWAYS_ON = -1;
const int LCDConfigManager::BACKLIGHT_TIMEOUT_VALUE_ALWAYS_OFF = 0;
const string LCDConfigManager::CONFIG_FILENAME = "lcd_config.json";
const string LCDConfigManager::DEFAULT_CONFIG_FILENAME = "lcd_config.default.json";
const string LCDConfigManager::BACKLIGHT_TIMEOUT_SECONDS = "lcd.backlight.timeout-seconds";

const int LCDConfigManager::getBacklightTimeout() const
   {
   return getIntValue(BACKLIGHT_TIMEOUT_SECONDS);
   }

bool LCDConfigManager::setBacklightTimeout(const int timeoutInSeconds)
   {
   return setIntValue(BACKLIGHT_TIMEOUT_SECONDS, timeoutInSeconds);
   }

Json::Value LCDConfigManager::getJSON()
   {
   Json::Value config;
   if (!load(config))
      {
      cerr << "LCDConfigManager::getJSON(): failed to load config file, returning null JSON instead." << endl;
      return Json::Value::null;
      }

   return config;
   }

const bool LCDConfigManager::setJson(Json::Value& config)
   {
   if (config != Json::Value::null)
      {
      // now do some simple tests to verify it
      Json::Value* setBacklightTimeoutSecondsProperty = ConfigFile::findProperty(config, BACKLIGHT_TIMEOUT_SECONDS);
      if (setBacklightTimeoutSecondsProperty != NULL)
         {
         // revert to default
         revertToDefault();

         // set the BACKLIGHT_TIMEOUT_SECONDS
         return setBacklightTimeout(setBacklightTimeoutSecondsProperty->asInt());
         }
      }
   return false;
   }
