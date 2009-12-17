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

#include "TwoOptionMenuItemAction.h"

const string TwoOptionMenuItemAction::DEFAULT_ACTION_PROMPT = "Choose Yes or No";
const string TwoOptionMenuItemAction::DEFAULT_ACTION_CHOSE_OPTION1 = "You chose yes.";
const string TwoOptionMenuItemAction::DEFAULT_ACTION_CHOSE_OPTION2 = "You chose no.";
const string TwoOptionMenuItemAction::DEFAULT_ACTION_CHOSE_CANCEL = "Cancelled -- No settings changed";
const string TwoOptionMenuItemAction::DEFAULT_LABEL_OPTION1 = "Yes  ";
const string TwoOptionMenuItemAction::DEFAULT_LABEL_OPTION2 = "No";

const string TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION1 = "choice.option1";
const string TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION2 = "choice.option2";
const string TwoOptionMenuItemAction::PROPERTY_ACTION_PROMPT = "action.prompt";
const string TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_OPTION1 = "action.option1";
const string TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_OPTION2 = "action.option2";
const string TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_CANCEL = "action.cancel";

const int TwoOptionMenuItemAction::DEFAULT_MILLISECONDS_TO_SLEEP = 1500;

void TwoOptionMenuItemAction::activate()
   {
   userChoseOption1 = shouldOption1BeSelectedUponActivation();
   getCharacterDisplay()->setLine(0, getPromptText());
   getCharacterDisplay()->setLine(1, generateOptionChoiceLine());
   }

void TwoOptionMenuItemAction::start()
   {
   string text;
   if (userChoseOption1)
      {
      executeOption1Action();
      if (shouldDisplayTextThenExitToParentWhenAction1IsComplete())
         {
         text = getUserChoseOption1Text();
         getCharacterDisplay()->setText(text);
         sleepThenPopUpToParentMenuItem(getMillisecondsToSleepBeforeExitingToParentAfterAction1IsComplete());
         }
      }
   else
      {
      executeOption2Action();
      if (shouldDisplayTextThenExitToParentWhenAction2IsComplete())
         {
         text = getUserChoseOption2Text();
         getCharacterDisplay()->setText(text);
         sleepThenPopUpToParentMenuItem(getMillisecondsToSleepBeforeExitingToParentAfterAction2IsComplete());
         }
      }
   }

void TwoOptionMenuItemAction::stop()
   {
   getCharacterDisplay()->setText(getUserChoseCancelText());
   sleepThenPopUpToParentMenuItem(getMillisecondsToSleepBeforeExitingToParentAfterCancel());
   }

void TwoOptionMenuItemAction::rightEvent()
   {
   leftEvent();
   }

void TwoOptionMenuItemAction::leftEvent()
   {
   userChoseOption1 = !userChoseOption1;
   getCharacterDisplay()->setLine(1, generateOptionChoiceLine());
   }

void TwoOptionMenuItemAction::upEvent()
   {
   CharacterDisplayMenuItemAction::upEvent();
   }

void TwoOptionMenuItemAction::downEvent()
   {
   CharacterDisplayMenuItemAction::downEvent();
   }

void TwoOptionMenuItemAction::sleepThenPopUpToParentMenuItem(const int millisecondsToSleep)
   {
   usleep(1000 * millisecondsToSleep);
   CharacterDisplayMenuItemAction::stop();
   }
