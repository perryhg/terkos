//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ToggleWirelessStartupUponBootMenuItemAction.h"

const string ToggleWirelessStartupUponBootMenuItemAction::CLASS_NAME = "ToggleWirelessStartupUponBootMenuItemAction";

bool ToggleWirelessStartupUponBootMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return configManager.willStartOnBootup();
   }

void ToggleWirelessStartupUponBootMenuItemAction::executeOption1Action()
   {
   setWillStartOnBootup(true);
   }

void ToggleWirelessStartupUponBootMenuItemAction::executeOption2Action()
   {
   setWillStartOnBootup(false);
   }

void ToggleWirelessStartupUponBootMenuItemAction::setWillStartOnBootup(const bool willStartOnBootup)
   {
   configManager.setWillStartOnBootup(willStartOnBootup);
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
