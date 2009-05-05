/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "HardwareVersionMenuItemAction.h"

const string HardwareVersionMenuItemAction::CLASS_NAME = "HardwareVersionMenuItemAction";

void HardwareVersionMenuItemAction::activate()
   {
   // TODO: get this version number from the proper place
   getCharacterDisplay()->setLine(0, "Hardware");
   getCharacterDisplay()->setLine(1, "version 1.0.0h");
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
         return new HardwareVersionMenuItemAction(deleteObject, (MenuItem*)argv[0], (MenuStatusManager*)argv[1], (CharacterDisplay*)argv[2], *((map<string, string>*)argv[3]));
         }

      return NULL;
      }
   }
