/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "LCDConfigManager.h"

const string LCDConfigManager::CONFIG_FILENAME = "lcd_config.json";
const string LCDConfigManager::DEFAULT_CONFIG_FILENAME = "lcd_config.default.json";
const string LCDConfigManager::IS_BACKLIGHT_ENABLED = "lcd.backlight.enabled";

const bool LCDConfigManager::isBacklightEnabled() const
   {
   return getBooleanValue(IS_BACKLIGHT_ENABLED);
   }

bool LCDConfigManager::setBacklightEnabled(const bool isEnabled)
   {
   lcd.SetBacklight(isEnabled);
   return setBooleanValue(IS_BACKLIGHT_ENABLED, isEnabled);
   }

const bool LCDConfigManager::setJson(Json::Value& config)
   {
   if (config != Json::Value::null)
      {
      // now do some simple tests to verify it
      Json::Value* isBacklightEnabledProperty = ConfigFile::findProperty(config, IS_BACKLIGHT_ENABLED);
      if (isBacklightEnabledProperty != NULL)
         {
         // revert to default
         revertToDefault();

         // set the IS_BACKLIGHT_ENABLED
         return setBacklightEnabled(isBacklightEnabledProperty->asBool());
         }
      }
   return false;
   }

void LCDConfigManager::applyConfiguration()
   {
   // load the configuration from disk
   Json::Value config;
   load(config);

   // call setJson which ensures that the config prefs are applied to the system
   setJson(config);
   }
