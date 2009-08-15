/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "PowerStatusMenuItemAction.h"

const string PowerStatusMenuItemAction::CLASS_NAME = "PowerStatusMenuItemAction";

const unsigned int PowerStatusMenuItemAction::NUM_NONBREAKER_COMPONENTS = 7;
const unsigned int PowerStatusMenuItemAction::NUM_COMPONENTS = NUM_NONBREAKER_COMPONENTS + PowerInfoManager::NUM_BREAKERS;

const string PowerStatusMenuItemAction::VOLTAGE_PROPERTY        = "voltage.label";
const string PowerStatusMenuItemAction::STATE_PROPERTY         = "state.label";
const string PowerStatusMenuItemAction::MAIN_BATTERY_PROPERTY   = "main-battery.label";
const string PowerStatusMenuItemAction::BACKUP_BATTERY_PROPERTY = "backup-battery.label";
const string PowerStatusMenuItemAction::IO_5_VOLT_PROPERTY      = "io-5-volt.label";
const string PowerStatusMenuItemAction::MAIN_5_VOLT_PROPERTY    = "main-5-volt.label";
const string PowerStatusMenuItemAction::BREAKER_PROPERTY        = "breaker.label";
const string PowerStatusMenuItemAction::BREAKER_OK_PROPERTY     = "breaker-ok.label";
const string PowerStatusMenuItemAction::BREAKER_FAULT_PROPERTY  = "breaker-fault.label";

const string PowerStatusMenuItemAction::VOLTAGE_DEFAULT_LABEL        = "Voltage";
const string PowerStatusMenuItemAction::STATE_DEFAULT_LABEL          = "State";
const string PowerStatusMenuItemAction::MAIN_BATTERY_DEFAULT_LABEL   = "Main Battery";
const string PowerStatusMenuItemAction::BACKUP_BATTERY_DEFAULT_LABEL = "Backup Battery";
const string PowerStatusMenuItemAction::IO_5_VOLT_DEFAULT_LABEL      = "I/O 5 Volt";
const string PowerStatusMenuItemAction::MAIN_5_VOLT_DEFAULT_LABEL    = "Main 5 Volt";
const string PowerStatusMenuItemAction::BREAKER_DEFAULT_LABEL        = "Breaker";
const string PowerStatusMenuItemAction::BREAKER_OK_DEFAULT_LABEL     = "OK";
const string PowerStatusMenuItemAction::BREAKER_FAULT_DEFAULT_LABEL  = "Fault";

void PowerStatusMenuItemAction::activate()
   {
   // reset the current component id
   currentComponentId = 0;

   displayComponentDetails();
   }

void PowerStatusMenuItemAction::start()
   {
   // hitting OK will refresh the current value
   displayComponentDetails();
   }

void PowerStatusMenuItemAction::stop()
   {
   CharacterDisplayMenuItemAction::stop();
   }

void PowerStatusMenuItemAction::upEvent()
   {
   // decrement the current component ID, wrapping around if necessary
   if (currentComponentId == 0)
      {
      currentComponentId = NUM_COMPONENTS - 1;
      }
   else
      {
      currentComponentId--;
      }

   displayComponentDetails();
   }

void PowerStatusMenuItemAction::downEvent()
   {
   // increment the current component ID, wrapping around if necessary
   if (currentComponentId >= NUM_COMPONENTS - 1)
      {
      currentComponentId = 0;
      }
   else
      {
      currentComponentId++;
      }

   displayComponentDetails();
   }

void PowerStatusMenuItemAction::rightEvent()
   {
   downEvent();
   }

void PowerStatusMenuItemAction::leftEvent()
   {
   upEvent();
   }

void PowerStatusMenuItemAction::displayComponentDetails()
   {
   if (currentComponentId < NUM_NONBREAKER_COMPONENTS)
      {
      switch (currentComponentId)
         {
         case 0:
            displayComponentDetailsWorkhorse(
               getProperty(MAIN_BATTERY_PROPERTY, MAIN_BATTERY_DEFAULT_LABEL),
               getProperty(VOLTAGE_PROPERTY, VOLTAGE_DEFAULT_LABEL),
               StringUtilities::convertIntToString(powerInfoManager.getMainBatteryVoltage())
            );
            break;
         case 1:
            displayComponentDetailsWorkhorse(
               getProperty(MAIN_BATTERY_PROPERTY, MAIN_BATTERY_DEFAULT_LABEL),
               getProperty(STATE_PROPERTY, STATE_DEFAULT_LABEL),
               StringUtilities::convertIntToString(powerInfoManager.getMainBatteryState())
            );
            break;
         case 2:
            displayComponentDetailsWorkhorse(
               getProperty(BACKUP_BATTERY_PROPERTY, BACKUP_BATTERY_DEFAULT_LABEL),
               getProperty(VOLTAGE_PROPERTY, VOLTAGE_DEFAULT_LABEL),
               StringUtilities::convertIntToString(powerInfoManager.getBackupBatteryVoltage())
            );
            break;
         case 3:
            displayComponentDetailsWorkhorse(
               getProperty(BACKUP_BATTERY_PROPERTY, BACKUP_BATTERY_DEFAULT_LABEL),
               getProperty(STATE_PROPERTY, STATE_DEFAULT_LABEL),
               StringUtilities::convertIntToString(powerInfoManager.getBackupBatteryState())
            );
            break;
         case 4:
            displayComponentDetailsWorkhorse(
               getProperty(IO_5_VOLT_PROPERTY, IO_5_VOLT_DEFAULT_LABEL),
               getProperty(VOLTAGE_PROPERTY, VOLTAGE_DEFAULT_LABEL),
               StringUtilities::convertIntToString(powerInfoManager.getIO5VVoltage())
            );
            break;
         case 5:
            displayComponentDetailsWorkhorse(
               getProperty(IO_5_VOLT_PROPERTY, IO_5_VOLT_DEFAULT_LABEL),
               getProperty(STATE_PROPERTY, STATE_DEFAULT_LABEL),
               StringUtilities::convertIntToString(powerInfoManager.getIO5VState())
            );
            break;
         case 6:
            displayComponentDetailsWorkhorse(
               getProperty(MAIN_5_VOLT_PROPERTY, MAIN_5_VOLT_DEFAULT_LABEL),
               getProperty(VOLTAGE_PROPERTY, VOLTAGE_DEFAULT_LABEL),
               StringUtilities::convertIntToString(powerInfoManager.getMain5VVoltage())
            );
            break;
         default:
            // nothing to do
            break;
         }
      }
   else
      {
      unsigned int breakerId = currentComponentId - NUM_NONBREAKER_COMPONENTS;
      string line1 = middlePadOrTrimRight(
         getProperty(BREAKER_PROPERTY, BREAKER_DEFAULT_LABEL) + ": ",
         StringUtilities::convertIntToString(breakerId+1, 2, '0') + "/" + StringUtilities::convertIntToString(PowerInfoManager::NUM_BREAKERS, 2, '0'),
         numColumnsWhenInScrollingMode
      );
      string value = powerInfoManager.getBreakerState(breakerId) ? getProperty(BREAKER_FAULT_PROPERTY, BREAKER_FAULT_DEFAULT_LABEL) : getProperty(BREAKER_OK_PROPERTY, BREAKER_OK_DEFAULT_LABEL);
      displayComponentDetailsWorkhorse(
         line1,
         getProperty(STATE_PROPERTY, STATE_DEFAULT_LABEL),
         value
      );
      }
   }

void PowerStatusMenuItemAction::displayComponentDetailsWorkhorse(const string& line1, const string& line2, const string& value)
   {
   string cleanedLine1 = rightPadOrTrimRight(line1, numColumnsWhenInScrollingMode);
   string cleanedLine2 = middlePadOrTrimRight(line2 + ": ", value, numColumnsWhenInScrollingMode);

   getCharacterDisplay()->setTextWithScrollArrows(cleanedLine1 + cleanedLine2);
   }

const string PowerStatusMenuItemAction::rightPadOrTrimRight(const string& str, unsigned int desiredLength)
   {
   if (desiredLength > str.length())
      {
      string padding(desiredLength - str.length(), ' ');
      return str + padding;
      }
   else if (desiredLength < str.length())
      {
      return str.substr(0, desiredLength);
      }

   return str;
   }

const string PowerStatusMenuItemAction::middlePadOrTrimRight(const string& str1, const string& str2, unsigned int desiredLength)
   {
   unsigned int cattedLength = str1.length() + str2.length();
   if (desiredLength > cattedLength)
      {
      string padding(desiredLength - cattedLength, ' ');
      return str1 + padding + str2;
      }
   else if (desiredLength < cattedLength)
      {
      string cattedStr = str1 + str2;
      return cattedStr.substr(0, desiredLength);
      }

   return str1 + str2;
   }

// required definitions for dynamic loading
extern "C"
   {
   void deletePowerStatusMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadPowerStatusMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(PowerStatusMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, PowerStatusMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new PowerStatusMenuItemAction(deletePowerStatusMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
