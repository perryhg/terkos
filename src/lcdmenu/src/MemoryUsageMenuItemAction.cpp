/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "MemoryUsageMenuItemAction.h"

const string MemoryUsageMenuItemAction::CLASS_NAME = "MemoryUsageMenuItemAction";

const unsigned int MemoryUsageMenuItemAction::NUM_COMPONENTS = 3;
const string MemoryUsageMenuItemAction::UNKNOWN_VALUE = "???";

const string MemoryUsageMenuItemAction::TOTAL_MEMORY_PROPERTY = "total-memory.label";
const string MemoryUsageMenuItemAction::USED_MEMORY_PROPERTY  = "used-memory.label";
const string MemoryUsageMenuItemAction::FREE_MEMORY_PROPERTY  = "free-memory.label";

const string MemoryUsageMenuItemAction::TOTAL_MEMORY_DEFAULT_LABEL = "Total memory";
const string MemoryUsageMenuItemAction::USED_MEMORY_DEFAULT_LABEL  = "Used memory";
const string MemoryUsageMenuItemAction::FREE_MEMORY_DEFAULT_LABEL  = "Free memory";

void MemoryUsageMenuItemAction::activate()
   {
   // reset the current component id
   currentComponentId = 0;

   start();
   }

void MemoryUsageMenuItemAction::start()
   {
   // hitting OK will refresh the values
   loadValues();
   displayComponentDetails();
   }

void MemoryUsageMenuItemAction::stop()
   {
   CharacterDisplayMenuItemAction::stop();
   }

void MemoryUsageMenuItemAction::upEvent()
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

void MemoryUsageMenuItemAction::downEvent()
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

void MemoryUsageMenuItemAction::rightEvent()
   {
   downEvent();
   }

void MemoryUsageMenuItemAction::leftEvent()
   {
   upEvent();
   }

void MemoryUsageMenuItemAction::loadValues()
   {
   Json::Value json = memoryUsageManager.getJSON();
   if (json != Json::Value::null)
      {
      Json::Value totalMemoryJson = json["memory-usage"]["total"]["total"];
      Json::Value usedMemoryJson = json["memory-usage"]["total"]["used"];
      Json::Value freeMemoryJson = json["memory-usage"]["total"]["free"];

      totalMemory = StringUtilities::convertIntToString(totalMemoryJson.asInt());
      usedMemory = StringUtilities::convertIntToString(usedMemoryJson.asInt());
      freeMemory = StringUtilities::convertIntToString(freeMemoryJson.asInt());
      }
   else
      {
      totalMemory = UNKNOWN_VALUE;
      usedMemory = UNKNOWN_VALUE;
      freeMemory = UNKNOWN_VALUE;
      }
   }

void MemoryUsageMenuItemAction::displayComponentDetails()
   {
   switch (currentComponentId)
      {
      case 0:
         displayComponentDetailsWorkhorse(
            getProperty(TOTAL_MEMORY_PROPERTY, TOTAL_MEMORY_DEFAULT_LABEL),
            totalMemory
         );
         break;
      case 1:
         displayComponentDetailsWorkhorse(
            getProperty(USED_MEMORY_PROPERTY, USED_MEMORY_DEFAULT_LABEL),
            usedMemory
         );
         break;
      case 2:
         displayComponentDetailsWorkhorse(
            getProperty(FREE_MEMORY_PROPERTY, FREE_MEMORY_DEFAULT_LABEL),
            freeMemory
         );
         break;
      default:
         // nothing to do
         break;
      }
   }

void MemoryUsageMenuItemAction::displayComponentDetailsWorkhorse(const string& label, const string& value)
   {
   string cleanedLine1 = StringUtilities::rightPadOrTrimRight(label + ":", numColumnsWhenInScrollingMode);

   getCharacterDisplay()->setTextWithScrollArrows(cleanedLine1 + value + " K");
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteMemoryUsageMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadMemoryUsageMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(MemoryUsageMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, MemoryUsageMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new MemoryUsageMenuItemAction(deleteMemoryUsageMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
