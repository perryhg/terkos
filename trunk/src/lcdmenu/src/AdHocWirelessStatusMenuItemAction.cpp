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

#include "AdHocWirelessStatusMenuItemAction.h"

const string AdHocWirelessStatusMenuItemAction::CLASS_NAME = "AdHocWirelessStatusMenuItemAction";

const string AdHocWirelessStatusMenuItemAction::STATUS_FAILURE_PROPERTY_ACTION_PROMPT = "status.failure";
const string AdHocWirelessStatusMenuItemAction::STATUS_FAILURE_DEFAULT_ACTION_PROMPT = "Failed to get   ad-hoc status.";

const string AdHocWirelessStatusMenuItemAction::STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT = "status.unplugged";
const string AdHocWirelessStatusMenuItemAction::STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT = "Wireless adapteris unplugged.";

const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_PROMPT = "wireless-disabled.action.prompt";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_CANCEL = "wireless-disabled.action.cancel";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION1 = "wireless-disabled.choice.option1";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION2 = "wireless-disabled.choice.option2";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1 = "wireless-disabled.action.option1";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_OPTION1_FAILURE = "wireless-disabled.option1.failure";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1_BEFORE = "wireless-disabled.action.option1.before";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_PROMPT = "Ad-hoc is off.";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_CANCEL = "Cancelled -- No settings changed";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_LABEL_OPTION1 = "Start   ";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_LABEL_OPTION2 = "OK";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1 = "Ad-hoc network  is now on.";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_OPTION1_FAILURE = "Failed to start ad-hoc network.";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1_BEFORE = "Starting ad-hoc wifi network...";

const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_PROMPT = "wireless-enabled.action.prompt";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_CANCEL = "wireless-enabled.action.cancel";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION1 = "wireless-enabled.choice.option1";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION2 = "wireless-enabled.choice.option2";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2 = "wireless-enabled.action.option2";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE = "wireless-enabled.option1.failure";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_OPTION2_FAILURE = "wireless-enabled.option2.failure";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2_BEFORE = "wireless-enabled.action.option2.before";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_PROMPT = "Ad-hoc is on.";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_CANCEL = "Cancelled -- No settings changed";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_LABEL_OPTION1 = "Info  ";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_LABEL_OPTION2 = "Stop";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2 = "Ad-hoc network  is now off.";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE = "Could not obtainad-hoc details";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_OPTION2_FAILURE = "Failed to stop  ad-hoc network.";
const string AdHocWirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2_BEFORE = "Stopping ad-hoc wifi network...";

void AdHocWirelessStatusMenuItemAction::activate()
   {
   WirelessStatusCheckingMenuItemAction::activate();
   currentMenuItemAction->activate();
   }

void AdHocWirelessStatusMenuItemAction::start()
   {
   currentMenuItemAction->start();
   }

void AdHocWirelessStatusMenuItemAction::stop()
   {
   currentMenuItemAction->stop();
   }

void AdHocWirelessStatusMenuItemAction::upEvent()
   {
   currentMenuItemAction->upEvent();
   }

void AdHocWirelessStatusMenuItemAction::downEvent()
   {
   currentMenuItemAction->downEvent();
   }

void AdHocWirelessStatusMenuItemAction::rightEvent()
   {
   currentMenuItemAction->rightEvent();
   }

void AdHocWirelessStatusMenuItemAction::leftEvent()
   {
   currentMenuItemAction->leftEvent();
   }

// ---------------------------------------------------------------------------------------------------------------------

bool AdHocWirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return true;
   }

void AdHocWirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::executeOption1Action()
   {
   isOnChoiceScreen = false;

   getCharacterDisplay()->setText(getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1_BEFORE, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1_BEFORE));

   // enable ad-hoc wireless and display the appropriate status upon completion
   if (WirelessNetworkingManager::enableAdHocWirelessNetworking())
      {
      getCharacterDisplay()->setText(getProperty(WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1, WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1));
      }
   else
      {
      getCharacterDisplay()->setText(getProperty(WIRELESS_DISABLED_PROPERTY_OPTION1_FAILURE, WIRELESS_DISABLED_DEFAULT_OPTION1_FAILURE));
      }

   sleepThenPopUpToParentMenuItem(TwoOptionMenuItemAction::DEFAULT_MILLISECONDS_TO_SLEEP);
   }

void AdHocWirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::executeOption2Action()
   {
   isOnChoiceScreen = false;
   }

// ---------------------------------------------------------------------------------------------------------------------

bool AdHocWirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return true;
   }

void AdHocWirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::executeOption1Action()
   {
   isOnChoiceScreen = false;
   Json::Value status = WirelessNetworkingManager::getWirelessNetworkingStatus();
   Json::Value accessPointJson = status["wireless-networking-status"]["wireless-interface"]["access-point"];
   if (accessPointJson != Json::Value::null)
      {
      getCharacterDisplay()->setLine(0, accessPointJson["ssid"].asString());
      getCharacterDisplay()->setLine(1, accessPointJson["ip-address"].asString());
      }
   else
      {
      getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE, WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE));
      }
   }

void AdHocWirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::executeOption2Action()
   {
   getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2_BEFORE, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2_BEFORE));

   isOnChoiceScreen = false;

   // disable ad-hoc wireless and display the appropriate status upon completion
   if (WirelessNetworkingManager::disableWirelessNetworking())
      {
      getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2));
      }
   else
      {
      getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_OPTION2_FAILURE, WIRELESS_ENABLED_DEFAULT_OPTION2_FAILURE));
      }

   sleepThenPopUpToParentMenuItem(TwoOptionMenuItemAction::DEFAULT_MILLISECONDS_TO_SLEEP);
   }
// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteAdHocWirelessStatusMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadAdHocWirelessStatusMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(AdHocWirelessStatusMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, AdHocWirelessStatusMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new AdHocWirelessStatusMenuItemAction(deleteAdHocWirelessStatusMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
