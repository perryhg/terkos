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
   configManager.revertToDefault();
   }

void ResetAudioMenuItemAction::executeOption2Action()
   {
   // do nothing
   }

void ResetAudioMenuItemAction::setAlertsEnabled(const bool isEnabled)
   {
   configManager.setAlertsEnabled(isEnabled);
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
      int numCharsToCompare = std::min(strlen(name), strlen(ResetAudioMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ResetAudioMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ResetAudioMenuItemAction(deleteObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
