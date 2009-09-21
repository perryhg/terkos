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
   try
      {
      CTextLcd &lcd = CTextLcd::GetRef();
      lcd.SetBacklight(isEnabled);
      CTextLcd::Release();
      }
   catch (...)
      {
      // TODO: add logging
      cerr << "LCDConfigManager::setBacklightEnabled(): failed to get CTextLcd reference required to set backlight state." << endl;
      }

   return setBooleanValue(IS_BACKLIGHT_ENABLED, isEnabled);
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
