/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "OperatingSystemMenuItemAction.h"

const string OperatingSystemMenuItemAction::CLASS_NAME = "OperatingSystemMenuItemAction";

void OperatingSystemMenuItemAction::activate()
   {
   getCharacterDisplay()->setLine(0, versionInfoManager.getOperatingSystemName());
   getCharacterDisplay()->setLine(1, versionInfoManager.getOperatingSystemRelease());
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteOperatingSystemMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadOperatingSystemMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(OperatingSystemMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, OperatingSystemMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new OperatingSystemMenuItemAction(deleteOperatingSystemMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
