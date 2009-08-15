/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ResetWirelessNetworkingMenuItemAction.h"

const string ResetWirelessNetworkingMenuItemAction::CLASS_NAME = "ResetWirelessNetworkingMenuItemAction";

bool ResetWirelessNetworkingMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return false;
   }

void ResetWirelessNetworkingMenuItemAction::executeOption1Action()
   {
   WirelessNetworkingConfigManager manager;

   manager.revertToDefault();
   }

void ResetWirelessNetworkingMenuItemAction::executeOption2Action()
   {
   // do nothing
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteResetWirelessNetworkingMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadResetWirelessNetworkingMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ResetWirelessNetworkingMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ResetWirelessNetworkingMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ResetWirelessNetworkingMenuItemAction(deleteResetWirelessNetworkingMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
