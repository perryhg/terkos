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

#include "SetUserProgramToRunOnBootMenuItemAction.h"

const string SetUserProgramToRunOnBootMenuItemAction::CLASS_NAME = "SetUserProgramToRunOnBootMenuItemAction";

const string SetUserProgramToRunOnBootMenuItemAction::DEFAULT_LABEL_PROGRAM = "Program";
const string SetUserProgramToRunOnBootMenuItemAction::DEFAULT_LABEL_NO_PROGRAM = "< NONE >";
const string SetUserProgramToRunOnBootMenuItemAction::DEFAULT_LABEL_NO_PROGRAM_WILL_RUN_ON_BOOT = "No user program will run on boot";
const string SetUserProgramToRunOnBootMenuItemAction::DEFAULT_LABEL_ACTION_PERFORMED = "will run on boot";
const string SetUserProgramToRunOnBootMenuItemAction::DEFAULT_LABEL_ACTION_CANCELLED = "Cancelled -- No settings changed";
const string SetUserProgramToRunOnBootMenuItemAction::DEFAULT_LABEL_ACTION_FAILED = "Failed -- No    settings changed";

const string SetUserProgramToRunOnBootMenuItemAction::PROPERTY_PROGRAM = "label.program";
const string SetUserProgramToRunOnBootMenuItemAction::PROPERTY_NO_PROGRAM = "label.no-program";
const string SetUserProgramToRunOnBootMenuItemAction::PROPERTY_NO_PROGRAM_WILL_RUN_ON_BOOT = "label.no-program-will-run-on-boot";
const string SetUserProgramToRunOnBootMenuItemAction::PROPERTY_ACTION_PERFORMED = "action.performed";
const string SetUserProgramToRunOnBootMenuItemAction::PROPERTY_ACTION_CANCELLED = "action.cancelled";
const string SetUserProgramToRunOnBootMenuItemAction::PROPERTY_ACTION_FAILED = "action.failed";

void SetUserProgramToRunOnBootMenuItemAction::activate()
   {
   // get the program names from the config manager
   userProgramNames = configManager.getUserProgramNames();

   // insert the option for running nothing on boot
   userProgramNames.insert(userProgramNames.begin(), getProperty(PROPERTY_NO_PROGRAM, DEFAULT_LABEL_NO_PROGRAM));

   // get the name currently-selected program from the config manager
   string currentProgramName = configManager.getProgramToRunOnBoot();

   // now try to find the currently-selected program in the set of program names so we can determine the correct initial index
   selectedUserProgramIndex = 0;
   if (currentProgramName.compare("") != 0)
      {
      for (unsigned int i = 1; i < userProgramNames.size(); i++)
         {
         if (currentProgramName.compare(userProgramNames[i]) == 0)
            {
            selectedUserProgramIndex = i;
            break;
            }
         }
      }

   displaySelectedUserProgram();
   }

void SetUserProgramToRunOnBootMenuItemAction::start()
   {
   if (configManager.setProgramToRunOnBoot(getSelectedProgramName()))
      {
      displayActionPerformedText();
      }
   else
      {
      displayActionFailedText();
      }
   sleepThenPopUpToParentMenuItem();
   }

void SetUserProgramToRunOnBootMenuItemAction::stop()
   {
   displayActionCancelledText();
   sleepThenPopUpToParentMenuItem();
   }

void SetUserProgramToRunOnBootMenuItemAction::upEvent()
   {
   // decrement the current servo ID, wrapping around if necessary
   if (selectedUserProgramIndex == 0)
      {
      selectedUserProgramIndex = getNumberOfUserPrograms() - 1;
      }
   else
      {
      selectedUserProgramIndex--;
      }

   displaySelectedUserProgram();
   }

void SetUserProgramToRunOnBootMenuItemAction::downEvent()
   {
   // increment the current servo ID, wrapping around if necessary
   if (selectedUserProgramIndex >= getNumberOfUserPrograms() - 1)
      {
      selectedUserProgramIndex = 0;
      }
   else
      {
      selectedUserProgramIndex++;
      }

   displaySelectedUserProgram();
   }

void SetUserProgramToRunOnBootMenuItemAction::rightEvent()
   {
   downEvent();
   }

void SetUserProgramToRunOnBootMenuItemAction::leftEvent()
   {
   upEvent();
   }

void SetUserProgramToRunOnBootMenuItemAction::displayActionPerformedText()
   {
   if (configManager.getProgramToRunOnBoot().compare("") == 0)
      {
      getCharacterDisplay()->setText(getProperty(PROPERTY_NO_PROGRAM_WILL_RUN_ON_BOOT, DEFAULT_LABEL_NO_PROGRAM_WILL_RUN_ON_BOOT));
      }
   else
      {
      getCharacterDisplay()->setLine(0, configManager.getProgramToRunOnBoot());
      getCharacterDisplay()->setLine(1, getProperty(PROPERTY_ACTION_PERFORMED, DEFAULT_LABEL_ACTION_PERFORMED));
      }
   }

void SetUserProgramToRunOnBootMenuItemAction::displayActionCancelledText()
   {
   getCharacterDisplay()->setText(getProperty(PROPERTY_ACTION_CANCELLED, DEFAULT_LABEL_ACTION_CANCELLED));
   }

void SetUserProgramToRunOnBootMenuItemAction::displayActionFailedText()
   {
   getCharacterDisplay()->setText(getProperty(PROPERTY_ACTION_FAILED, DEFAULT_LABEL_ACTION_FAILED));
   }

void SetUserProgramToRunOnBootMenuItemAction::displaySelectedUserProgram()
   {
   string s =  getProperty(PROPERTY_PROGRAM, DEFAULT_LABEL_PROGRAM) + ": ";
   s += StringUtilities::convertIntToString(selectedUserProgramIndex + 1, 2, '0');
   s += "/";
   s += StringUtilities::convertIntToString(getNumberOfUserPrograms(), 2, '0');
   getCharacterDisplay()->setTextWithScrollArrows(s + getSelectedProgramName());
   }

void SetUserProgramToRunOnBootMenuItemAction::sleepThenPopUpToParentMenuItem()
   {
   usleep(1500000);
   CharacterDisplayMenuItemAction::stop();
   }

// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteSetUserProgramToRunOnBootMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadSetUserProgramToRunOnBootMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(SetUserProgramToRunOnBootMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, SetUserProgramToRunOnBootMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new SetUserProgramToRunOnBootMenuItemAction(deleteSetUserProgramToRunOnBootMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
