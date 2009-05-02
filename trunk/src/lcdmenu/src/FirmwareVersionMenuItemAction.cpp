/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "FirmwareVersionMenuItemAction.h"

const string FirmwareVersionMenuItemAction::CLASS_NAME = "FirmwareVersionMenuItemAction";

void FirmwareVersionMenuItemAction::activate()
   {
   getCharacterDisplay()->setLine(0, "Firmware");
   getCharacterDisplay()->setLine(1, "version 1.0.0f");
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
         // TODO: Remove this debug info
         printf("FirmwareVersionMenuItemAction.loadObject()\n");
         printf("argv[0]=%p\n",argv[0]);
         printf("argv[1]=%p\n",argv[1]);
         printf("argv[2]=%p\n",argv[2]);
         printf("argv[3]=%p\n",argv[3]);

         map<string, string> properties;
         return new FirmwareVersionMenuItemAction(deleteObject, (MenuItem*)argv[0], (MenuStatusManager*)argv[1], (CharacterDisplay*)argv[2], *((map<string, string>*)argv[3]));
         }

      return NULL;
      }
   }
