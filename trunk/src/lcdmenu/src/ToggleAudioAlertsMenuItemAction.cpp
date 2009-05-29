/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ToggleAudioAlertsMenuItemAction.h"

const string ToggleAudioAlertsMenuItemAction::CLASS_NAME = "ToggleAudioAlertsMenuItemAction";

bool ToggleAudioAlertsMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return configManager.areAlertsEnabled();
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
   configManager.setAlertsEnabled(isEnabled);
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteToggleAudioAlertsMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadToggleAudioAlertsMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ToggleAudioAlertsMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ToggleAudioAlertsMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ToggleAudioAlertsMenuItemAction(deleteToggleAudioAlertsMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
