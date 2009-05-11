/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "FirmwareVersionMenuItemAction.h"

const string FirmwareVersionMenuItemAction::CLASS_NAME = "FirmwareVersionMenuItemAction";
const string FirmwareVersionMenuItemAction::DEFAULT_LABEL = "Firmware version";
const string FirmwareVersionMenuItemAction::PROPERTY_LABEL = "label";

void FirmwareVersionMenuItemAction::activate()
   {
   // TODO: get this version number from the proper place
   getCharacterDisplay()->setLine(0, getProperty(PROPERTY_LABEL, DEFAULT_LABEL));
   getCharacterDisplay()->setLine(1, "1.0.0f");
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
      int numCharsToCompare = std::min(strlen(name), strlen(FirmwareVersionMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, FirmwareVersionMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new FirmwareVersionMenuItemAction(deleteObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
