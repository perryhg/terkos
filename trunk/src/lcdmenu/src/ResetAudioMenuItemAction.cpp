/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ResetAudioMenuItemAction.h"

const string ResetAudioMenuItemAction::CLASS_NAME = "ResetAudioMenuItemAction";

bool ResetAudioMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return false;
   }

void ResetAudioMenuItemAction::executeOption1Action()
   {
   AudioConfigManager manager;

   manager.revertToDefault();
   }

void ResetAudioMenuItemAction::executeOption2Action()
   {
   // do nothing
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteResetAudioMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadResetAudioMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ResetAudioMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ResetAudioMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ResetAudioMenuItemAction(deleteResetAudioMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
