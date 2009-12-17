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

#include "ResetRootUserControlPanelPasswordMenuItemAction.h"

const string ResetRootUserControlPanelPasswordMenuItemAction::CLASS_NAME = "ResetRootUserControlPanelPasswordMenuItemAction";

bool ResetRootUserControlPanelPasswordMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return false;
   }

void ResetRootUserControlPanelPasswordMenuItemAction::executeOption1Action()
   {
   // reset the root user password
   RootUserPasswordManager::revertToDefault();
   }

void ResetRootUserControlPanelPasswordMenuItemAction::executeOption2Action()
   {
   // do nothing
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteResetRootUserControlPanelPasswordMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadResetRootUserControlPanelPasswordMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ResetRootUserControlPanelPasswordMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ResetRootUserControlPanelPasswordMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ResetRootUserControlPanelPasswordMenuItemAction(deleteResetRootUserControlPanelPasswordMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
