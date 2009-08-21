/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ToggleLCDBacklightMenuItemAction.h"

const string ToggleLCDBacklightMenuItemAction::CLASS_NAME = "ToggleLCDBacklightMenuItemAction";

bool ToggleLCDBacklightMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return configManager.isBacklightEnabled();
   }

void ToggleLCDBacklightMenuItemAction::executeOption1Action()
   {
   setBacklightEnabled(true);
   }

void ToggleLCDBacklightMenuItemAction::executeOption2Action()
   {
   setBacklightEnabled(false);
   }

void ToggleLCDBacklightMenuItemAction::setBacklightEnabled(const bool isEnabled)
   {
   configManager.setBacklightEnabled(isEnabled);
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteToggleLCDBacklightMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadToggleLCDBacklightMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ToggleLCDBacklightMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ToggleLCDBacklightMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ToggleLCDBacklightMenuItemAction(deleteToggleLCDBacklightMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
