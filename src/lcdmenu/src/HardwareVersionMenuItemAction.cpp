/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "HardwareVersionMenuItemAction.h"

const string HardwareVersionMenuItemAction::CLASS_NAME = "HardwareVersionMenuItemAction";
const string HardwareVersionMenuItemAction::DEFAULT_LABEL = "Hardware version";
const string HardwareVersionMenuItemAction::PROPERTY_LABEL = "label";

void HardwareVersionMenuItemAction::activate()
   {
   // TODO: user properties instead of hardcoded english
   getCharacterDisplay()->setLine(0, getProperty(PROPERTY_LABEL, DEFAULT_LABEL));
   getCharacterDisplay()->setLine(1, "1.0.0h");
   // TODO: use this once CQwerkHardware is working
   // getCharacterDisplay()->setLine(1, "" + hardware.GetVersion());
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(HardwareVersionMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, HardwareVersionMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new HardwareVersionMenuItemAction(deleteObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
