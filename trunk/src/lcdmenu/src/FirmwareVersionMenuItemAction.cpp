/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "FirmwareVersionMenuItemAction.h"

const string FirmwareVersionMenuItemAction::CLASS_NAME = "FirmwareVersionMenuItemAction";
const string FirmwareVersionMenuItemAction::DEFAULT_LABEL = "Firmware version";
const string FirmwareVersionMenuItemAction::PROPERTY_LABEL = "label";

void FirmwareVersionMenuItemAction::activate()
   {
   getCharacterDisplay()->setLine(0, getProperty(PROPERTY_LABEL, DEFAULT_LABEL));
   getCharacterDisplay()->setLine(1, versionInfoManager.getFirmwareVersion());
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteFirmwareVersionMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadFirmwareVersionMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(FirmwareVersionMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, FirmwareVersionMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new FirmwareVersionMenuItemAction(deleteFirmwareVersionMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
