/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ToggleAudioAlertsMenuItemAction.h"

const string ToggleAudioAlertsMenuItemAction::CLASS_NAME = "ToggleAudioAlertsMenuItemAction";

bool ToggleAudioAlertsMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return config.areAlertsEnabled();
   }

void ToggleAudioAlertsMenuItemAction::executeOption1Action()
   {
   setAlertsEnabled(true);
   }

void ToggleAudioAlertsMenuItemAction::executeOption2Action()
   {
   setAlertsEnabled(false);
   }

void ToggleAudioAlertsMenuItemAction::setAlertsEnabled(const bool isEnabled)
   {
   config.setAlertsEnabled(isEnabled);
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
      int numCharsToCompare = std::min(strlen(name), strlen(ToggleAudioAlertsMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ToggleAudioAlertsMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ToggleAudioAlertsMenuItemAction(deleteObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
