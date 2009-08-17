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
