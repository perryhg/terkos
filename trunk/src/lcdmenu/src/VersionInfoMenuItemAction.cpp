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

#include "VersionInfoMenuItemAction.h"

const string VersionInfoMenuItemAction::CLASS_NAME = "VersionInfoMenuItemAction";

const string VersionInfoMenuItemAction::UNKNOWN_VALUE = "???";

const string VersionInfoMenuItemAction::OPERATING_SYSTEM_VERSION_PROPERTY = "operating-system-version.label";
const string VersionInfoMenuItemAction::FIRMWARE_VERSION_PROPERTY = "firmware-version.label";
const string VersionInfoMenuItemAction::HARDWARE_VERSION_PROPERTY = "hardware-version.label";

const string VersionInfoMenuItemAction::OPERATING_SYSTEM_VERSION_DEFAULT_LABEL = "OS Version";
const string VersionInfoMenuItemAction::FIRMWARE_VERSION_DEFAULT_LABEL = "Firmware Ver";
const string VersionInfoMenuItemAction::HARDWARE_VERSION_DEFAULT_LABEL = "Hardware Ver";

void VersionInfoMenuItemAction::activate()
   {
   // reset the current component id
   currentScreenId = 0;

   start();
   }

void VersionInfoMenuItemAction::start()
   {
   displayCurrentScreen();
   }

void VersionInfoMenuItemAction::stop()
   {
   CharacterDisplayMenuItemAction::stop();
   }

void VersionInfoMenuItemAction::upEvent()
   {
   // decrement the current component ID, wrapping around if necessary
   if (currentScreenId == 0)
      {
      currentScreenId = screens.size() - 1;
      }
   else
      {
      currentScreenId--;
      }

   displayCurrentScreen();
   }

void VersionInfoMenuItemAction::downEvent()
   {
   // increment the current component ID, wrapping around if necessary
   if (currentScreenId >= screens.size() - 1)
      {
      currentScreenId = 0;
      }
   else
      {
      currentScreenId++;
      }

   displayCurrentScreen();
   }

void VersionInfoMenuItemAction::rightEvent()
   {
   downEvent();
   }

void VersionInfoMenuItemAction::leftEvent()
   {
   upEvent();
   }

void VersionInfoMenuItemAction::displayCurrentScreen()
   {
   getCharacterDisplay()->setTextWithScrollArrows(screens[currentScreenId]);
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteVersionInfoMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadVersionInfoMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(VersionInfoMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, VersionInfoMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new VersionInfoMenuItemAction(deleteVersionInfoMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
