/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "PowerInfoManager.h"

const unsigned int PowerInfoManager::NUM_BREAKERS = 5;

const int PowerInfoManager::getMainBatteryVoltage()
   {
   return getHardwareProperty(QHW_PROP_MAIN_BATT_VOLTAGE);
   }

const int PowerInfoManager::getBackupBatteryVoltage()
   {
   return getHardwareProperty(QHW_PROP_BACKUP_BATT_VOLTAGE);
   }

const int PowerInfoManager::getIO5VVoltage()
   {
   return getHardwareProperty(QHW_PROP_IO_5V_VOLTAGE);
   }

const int PowerInfoManager::getMain5VVoltage()
   {
   return getHardwareProperty(QHW_PROP_MAIN_5V_VOLTAGE);
   }

const int PowerInfoManager::getMainBatteryState()
   {
   return getHardwareProperty(QHW_PROP_MAIN_BATT_STATE);
   }

const int PowerInfoManager::getBackupBatteryState()
   {
   return getHardwareProperty(QHW_PROP_BACKUP_BATT_STATE);
   }

const int PowerInfoManager::getIO5VState()
   {
   return getHardwareProperty(QHW_PROP_IO_5V_STATE);
   }

const bool PowerInfoManager::getBreakerState(const unsigned int breakerId)
   {
   if (breakerId < NUM_BREAKERS)
      {
      const int breakersState = getHardwareProperty(QHW_PROP_BREAKER_STATE);
      if (breakersState >= 0)
         {
         return (breakersState >> breakerId & 1) == 1;
         }
      }
   return false;
   }

Json::Value PowerInfoManager::getJSON()
   {
   Json::Value mainBatteryElement;
   mainBatteryElement["voltage"] = getMainBatteryVoltage();
   mainBatteryElement["state"] = getMainBatteryState();

   Json::Value backupBatteryElement;
   backupBatteryElement["voltage"] = getBackupBatteryVoltage();
   backupBatteryElement["state"] = getBackupBatteryState();

   Json::Value io5VElement;
   io5VElement["voltage"] = getIO5VVoltage();
   io5VElement["state"] = getIO5VState();

   Json::Value main5VElement;
   main5VElement["voltage"] = getMain5VVoltage();

   Json::Value breakersElement;
   for (unsigned int i=0; i<NUM_BREAKERS; i++)
      {
      breakersElement[i] = getBreakerState(i);
      }

   Json::Value powerInfoElement;
   powerInfoElement["main-battery"] = mainBatteryElement;
   powerInfoElement["backup-battery"] = backupBatteryElement;
   powerInfoElement["IO-5V"] = io5VElement;
   powerInfoElement["main-5V"] = main5VElement;
   powerInfoElement["breakers-state"] = breakersElement;

   Json::Value rootElement;
   rootElement["power-info"] = powerInfoElement;

   return rootElement;
   }

int PowerInfoManager::getHardwareProperty(const int property)
   {
   long val = 0;
   if (hardware.GetProperty(property, &val) == PROP_OK)
      {
      return (int)val;
      }
   else
      {
      // TODO: add some logging here
      }

   return -1;
   }
