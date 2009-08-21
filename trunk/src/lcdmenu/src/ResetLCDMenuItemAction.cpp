/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ResetLCDMenuItemAction.h"

const string ResetLCDMenuItemAction::CLASS_NAME = "ResetLCDMenuItemAction";

bool ResetLCDMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return false;
   }

void ResetLCDMenuItemAction::executeOption1Action()
   {
   LCDConfigManager manager;

   manager.revertToDefault();
   }

void ResetLCDMenuItemAction::executeOption2Action()
   {
   // do nothing
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteResetLCDMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadResetLCDMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ResetLCDMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ResetLCDMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ResetLCDMenuItemAction(deleteResetLCDMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
