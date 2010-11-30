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

#ifndef ADHOCWIRELESSSTATUSMENUITEMACTION_H_
#define ADHOCWIRELESSSTATUSMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pstream.h>
#include <json/json.h>
#include <CharacterDisplayMenuItemAction.h>
#include <TwoOptionMenuItemAction.h>
#include <NoOpMenuItemAction.h>
#include <StringUtilities.h>
#include "WirelessStatusCheckingMenuItemAction.h"

using namespace std;
using namespace redi;

class AdHocWirelessStatusMenuItemAction : public WirelessStatusCheckingMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string STATUS_FAILURE_PROPERTY_ACTION_PROMPT;
      static const string STATUS_FAILURE_DEFAULT_ACTION_PROMPT;

      static const string STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT;
      static const string STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT;

      static const string WIRELESS_DISABLED_PROPERTY_ACTION_PROMPT;
      static const string WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION1;
      static const string WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION2;
      static const string WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1;
      static const string WIRELESS_DISABLED_PROPERTY_OPTION1_FAILURE;
      static const string WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1_BEFORE;

      static const string WIRELESS_DISABLED_DEFAULT_ACTION_PROMPT;
      static const string WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_DISABLED_DEFAULT_LABEL_OPTION1;
      static const string WIRELESS_DISABLED_DEFAULT_LABEL_OPTION2;
      static const string WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1;
      static const string WIRELESS_DISABLED_DEFAULT_OPTION1_FAILURE;
      static const string WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1_BEFORE;

      static const string WIRELESS_ENABLED_PROPERTY_ACTION_PROMPT;
      static const string WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION1;
      static const string WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION2;
      static const string WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2;
      static const string WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE;
      static const string WIRELESS_ENABLED_PROPERTY_OPTION2_FAILURE;
      static const string WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2_BEFORE;

      static const string WIRELESS_ENABLED_DEFAULT_ACTION_PROMPT;
      static const string WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_CANCEL;
      static const string WIRELESS_ENABLED_DEFAULT_LABEL_OPTION1;
      static const string WIRELESS_ENABLED_DEFAULT_LABEL_OPTION2;
      static const string WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2;
      static const string WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE;
      static const string WIRELESS_ENABLED_DEFAULT_OPTION2_FAILURE;
      static const string WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2_BEFORE;

      AdHocWirelessStatusMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                   CharacterDisplay* characterDisplay, map<string, string>& properties) :
         WirelessStatusCheckingMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties, true, WirelessStatusCheckingMenuItemAction::WIRELESS_MODE_AD_HOC)
         {
         map<string, string> wirelessDisabledProperties;
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_PROMPT]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_PROMPT, WIRELESS_DISABLED_DEFAULT_ACTION_PROMPT);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_CANCEL]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_CANCEL, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_CANCEL);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION1]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION1, WIRELESS_DISABLED_DEFAULT_LABEL_OPTION1);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION2]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION2, WIRELESS_DISABLED_DEFAULT_LABEL_OPTION2);
         wirelessDisabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_OPTION1]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1);
         wirelessDisabledProperties[WIRELESS_DISABLED_PROPERTY_OPTION1_FAILURE]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_OPTION1_FAILURE, WIRELESS_DISABLED_DEFAULT_OPTION1_FAILURE);
         wirelessDisabledProperties[WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1_BEFORE]
                  = getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1_BEFORE, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1_BEFORE);

         map<string, string> wirelessEnabledProperties;
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_PROMPT]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_PROMPT, WIRELESS_ENABLED_DEFAULT_ACTION_PROMPT);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_CANCEL]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_CANCEL, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_CANCEL);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION1]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION1, WIRELESS_ENABLED_DEFAULT_LABEL_OPTION1);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_CHOICE_OPTION2]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION2, WIRELESS_ENABLED_DEFAULT_LABEL_OPTION2);
         wirelessEnabledProperties[TwoOptionMenuItemAction::PROPERTY_ACTION_CHOSE_OPTION2]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2);
         wirelessEnabledProperties[WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE, WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE);
         wirelessEnabledProperties[WIRELESS_ENABLED_PROPERTY_OPTION2_FAILURE]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_OPTION2_FAILURE, WIRELESS_ENABLED_DEFAULT_OPTION2_FAILURE);
         wirelessEnabledProperties[WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2_BEFORE]
                  = getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2_BEFORE, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2_BEFORE);

         wirelessFailureMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_FAILURE_PROPERTY_ACTION_PROMPT, STATUS_FAILURE_DEFAULT_ACTION_PROMPT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessUnpluggedMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT, STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessDisabledMenuItemAction
                  = new WirelessDisabledMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay, wirelessDisabledProperties);
         wirelessEnabledMenuItemAction
                  = new WirelessEnabledMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay, wirelessEnabledProperties);
         }

      virtual ~AdHocWirelessStatusMenuItemAction()
         {
         currentMenuItemAction = NULL;
         delete wirelessFailureMenuItemAction;
         delete wirelessUnpluggedMenuItemAction;
         delete wirelessDisabledMenuItemAction;
         delete wirelessEnabledMenuItemAction;
         }

      void activate();
      void start();
      void stop();
      void upEvent();
      void downEvent();
      void rightEvent();
      void leftEvent();

   protected:

      virtual void handleWirelessEnabled()
         {
         currentMenuItemAction = wirelessEnabledMenuItemAction;
         }

      virtual void handleWirelessDisabled()
         {
         currentMenuItemAction = wirelessDisabledMenuItemAction;
         }

      virtual void handleWirelessUnplugged()
         {
         currentMenuItemAction = wirelessUnpluggedMenuItemAction;
         }

      virtual void handleWirelessStatusFailure()
         {
         currentMenuItemAction = wirelessFailureMenuItemAction;
         }

   private:

      MenuItemAction* currentMenuItemAction;
      MenuItemAction* wirelessFailureMenuItemAction;
      MenuItemAction* wirelessUnpluggedMenuItemAction;
      MenuItemAction* wirelessDisabledMenuItemAction;
      MenuItemAction* wirelessEnabledMenuItemAction;

      friend class WirelessEnabledMenuItemAction;
      friend class WirelessDisabledMenuItemAction;

      class WirelessDisabledMenuItemAction : public TwoOptionMenuItemAction
         {
         public:

            WirelessDisabledMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                           CharacterDisplay* characterDisplay, map<string, string>& properties) :
               TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
               {
               // nothing to do
               }

            virtual ~WirelessDisabledMenuItemAction()
               {
               // nothing to do
               }

            void activate()
               {
               TwoOptionMenuItemAction::activate();
               isOnChoiceScreen = true;
               }

            void start()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::start();
                  }
               else
                  {
                  activate();
                  }
               }

            void stop()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::stop();
                  }
               else
                  {
                  activate();
                  }
               }

            void upEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::upEvent();
                  }
               }

            void downEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::downEvent();
                  }
               }

            void rightEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::rightEvent();
                  }
               }

            void leftEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::leftEvent();
                  }
               }

         protected:

            bool shouldOption1BeSelectedUponActivation() const;
            void executeOption1Action();
            void executeOption2Action();

            virtual bool shouldDisplayTextThenExitToParentWhenAction1IsComplete()
               {
               return false;
               }

            virtual bool shouldDisplayTextThenExitToParentWhenAction2IsComplete()
               {
               return true;
               }

            virtual int getMillisecondsToSleepBeforeExitingToParentAfterAction2IsComplete()
               {
               return 0;
               }

            virtual int getMillisecondsToSleepBeforeExitingToParentAfterCancel()
               {
               return 0;
               }

         private:

            bool isOnChoiceScreen;

            void sleepThenPopUpToParentMenuItem(const int millisecondsToSleep)
               {
               usleep(1000 * millisecondsToSleep);
               CharacterDisplayMenuItemAction::stop();
               }

         };

      class WirelessEnabledMenuItemAction : public TwoOptionMenuItemAction
         {
         public:

            WirelessEnabledMenuItemAction(void(*delObj)(void*), MenuItem* menuItem,
                                          MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay, map<string, string>& properties) :
               TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
               {
               // nothing to do
               }

            virtual ~WirelessEnabledMenuItemAction()
               {
               // nothing to do
               }

            void activate()
               {
               TwoOptionMenuItemAction::activate();
               isOnChoiceScreen = true;
               }

            void start()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::start();
                  }
               else
                  {
                  activate();
                  }
               }

            void stop()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::stop();
                  }
               else
                  {
                  activate();
                  }
               }

            void upEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::upEvent();
                  }
               }

            void downEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::downEvent();
                  }
               }

            void rightEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::rightEvent();
                  }
               }

            void leftEvent()
               {
               if (isOnChoiceScreen)
                  {
                  TwoOptionMenuItemAction::leftEvent();
                  }
               }

         protected:

            bool shouldOption1BeSelectedUponActivation() const;
            void executeOption1Action();
            void executeOption2Action();

            virtual bool shouldDisplayTextThenExitToParentWhenAction1IsComplete()
               {
               return false;
               }

            virtual bool shouldDisplayTextThenExitToParentWhenAction2IsComplete()
               {
               return false;
               }

            virtual int getMillisecondsToSleepBeforeExitingToParentAfterCancel()
               {
               return 0;
               }

         private:

            bool isOnChoiceScreen;
            
            void sleepThenPopUpToParentMenuItem(const int millisecondsToSleep)
               {
               usleep(1000 * millisecondsToSleep);
               CharacterDisplayMenuItemAction::stop();
               }

         };
   };

#endif /* ADHOCWIRELESSSTATUSMENUITEMACTION_H_ */
