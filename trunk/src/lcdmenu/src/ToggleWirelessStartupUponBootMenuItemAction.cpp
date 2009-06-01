/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ToggleWirelessStartupUponBootMenuItemAction.h"

const string ToggleWirelessStartupUponBootMenuItemAction::CLASS_NAME = "ToggleWirelessStartupUponBootMenuItemAction";

bool ToggleWirelessStartupUponBootMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return configManager.isEnabled();
   }

void ToggleWirelessStartupUponBootMenuItemAction::executeOption1Action()
   {
   setEnabled(true);
   }

void ToggleWirelessStartupUponBootMenuItemAction::executeOption2Action()
   {
   setEnabled(false);
   }

void ToggleWirelessStartupUponBootMenuItemAction::setEnabled(const bool isEnabled)
   {
   configManager.setEnabled(isEnabled);
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteToggleWirelessStartupUponBootMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadToggleWirelessStartupUponBootMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ToggleWirelessStartupUponBootMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ToggleWirelessStartupUponBootMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ToggleWirelessStartupUponBootMenuItemAction(deleteToggleWirelessStartupUponBootMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
