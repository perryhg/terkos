/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ResetWebControlPanelPasswordMenuItemAction.h"

const string ResetWebControlPanelPasswordMenuItemAction::CLASS_NAME = "ResetWebControlPanelPasswordMenuItemAction";

bool ResetWebControlPanelPasswordMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return false;
   }

void ResetWebControlPanelPasswordMenuItemAction::executeOption1Action()
   {
   WebControlPanelPasswordManager manager;

   // revert the password to the default
   manager.revertToDefault();

   // restart the web server
   manager.restartWebServer();
   }

void ResetWebControlPanelPasswordMenuItemAction::executeOption2Action()
   {
   // do nothing
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteResetWebControlPanelPasswordMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadResetWebControlPanelPasswordMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ResetWebControlPanelPasswordMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ResetWebControlPanelPasswordMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ResetWebControlPanelPasswordMenuItemAction(deleteResetWebControlPanelPasswordMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
