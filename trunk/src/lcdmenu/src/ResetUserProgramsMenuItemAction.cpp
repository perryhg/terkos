/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ResetUserProgramsMenuItemAction.h"

const string ResetUserProgramsMenuItemAction::CLASS_NAME = "ResetUserProgramsMenuItemAction";

bool ResetUserProgramsMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return false;
   }

void ResetUserProgramsMenuItemAction::executeOption1Action()
   {
   UserProgramsConfigManager manager;

   manager.revertToDefault();
   }

void ResetUserProgramsMenuItemAction::executeOption2Action()
   {
   // do nothing
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteResetUserProgramsMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadResetUserProgramsMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ResetUserProgramsMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ResetUserProgramsMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ResetUserProgramsMenuItemAction(deleteResetUserProgramsMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
