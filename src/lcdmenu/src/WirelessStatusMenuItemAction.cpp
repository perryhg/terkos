/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessStatusMenuItemAction.h"

const string WirelessStatusMenuItemAction::CLASS_NAME = "WirelessStatusMenuItemAction";

const string WirelessStatusMenuItemAction::STATUS_FAILURE_PROPERTY_ACTION_PROMPT = "status.failure";
const string WirelessStatusMenuItemAction::STATUS_FAILURE_DEFAULT_ACTION_PROMPT = "Failed to get   wireless status.";

const string WirelessStatusMenuItemAction::STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT = "status.unplugged";
const string WirelessStatusMenuItemAction::STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT = "Wireless adapteris unplugged.";

const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_PROMPT = "wireless-disabled.action.prompt";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_CANCEL = "wireless-disabled.action.cancel";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION1 = "wireless-disabled.choice.option1";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_CHOICE_OPTION2 = "wireless-disabled.choice.option2";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION1 = "wireless-disabled.action.option1";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_PROPERTY_ACTION_CHOSE_OPTION2 = "wireless-disabled.action.option2";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_PROMPT = "Wireless is off.";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_CANCEL = "Cancelled -- No settings changed";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_LABEL_OPTION1 = "Start   ";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_LABEL_OPTION2 = "OK";
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION1 = "You chose Start."; // TODO
const string WirelessStatusMenuItemAction::WIRELESS_DISABLED_DEFAULT_ACTION_CHOSE_OPTION2 = "Nothing to say here."; // TODO

const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_PROMPT = "wireless-enabled.action.prompt";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_CANCEL = "wireless-enabled.action.cancel";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION1 = "wireless-enabled.choice.option1";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_CHOICE_OPTION2 = "wireless-enabled.choice.option2";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2 = "wireless-enabled.action.option2";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_PROPERTY_OPTION1_FAILURE = "wireless-enabled.option1.failure";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_PROMPT = "Wireless is on.";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_CANCEL = "Cancelled -- No settings changed";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_LABEL_OPTION1 = "Info  ";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_LABEL_OPTION2 = "Stop";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2 = "Wireless is now turned off.";
const string WirelessStatusMenuItemAction::WIRELESS_ENABLED_DEFAULT_OPTION1_FAILURE = "Could not obtainwireless details";

void WirelessStatusMenuItemAction::activate()
   {
   WirelessStatusCheckingMenuItemAction::activate();
   currentMenuItemAction->activate();
   }

void WirelessStatusMenuItemAction::start()
   {
   currentMenuItemAction->start();
   }

void WirelessStatusMenuItemAction::stop()
   {
   currentMenuItemAction->stop();
   }

void WirelessStatusMenuItemAction::upEvent()
   {
   currentMenuItemAction->upEvent();
   }

void WirelessStatusMenuItemAction::downEvent()
   {
   currentMenuItemAction->downEvent();
   }

void WirelessStatusMenuItemAction::rightEvent()
   {
   currentMenuItemAction->rightEvent();
   }

void WirelessStatusMenuItemAction::leftEvent()
   {
   currentMenuItemAction->leftEvent();
   }

// ---------------------------------------------------------------------------------------------------------------------

bool WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return true;
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::executeOption1Action()
   {
   // TODO
   cout << "WirelessDisabledMenuItemAction::executeOption1Action()" << endl;
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::executeOption2Action()
   {
   // TODO
   cout << "WirelessDisabledMenuItemAction::executeOption2Action()" << endl;
   }

// ---------------------------------------------------------------------------------------------------------------------

bool WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::shouldOption1BeSelectedUponActivation() const
   {
   return true;
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::executeOption1Action()
   {
   isOnChoiceScreen = false;
   Json::Value status = parentMenuItemAction->getWirelessNetworkingStatus();
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

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::executeOption2Action()
   {
   // TODO
   isOnChoiceScreen = false;

   // call the Perl script which disables wireless
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/disableWirelessNetworking.pl");

      // In this case, we don't really care about the returned JSON status.  Just display
      // a message to the user and then pop up to the parent
      is.close();
      }
   catch (...)
      {
      cerr << "WirelessEnabledMenuItemAction::executeOption2Action(): failed to call script to disable wireless networking." << endl;
      }

   getCharacterDisplay()->setText(getProperty(WIRELESS_ENABLED_PROPERTY_ACTION_CHOSE_OPTION2, WIRELESS_ENABLED_DEFAULT_ACTION_CHOSE_OPTION2));
   }
// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteWirelessStatusMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadWirelessStatusMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(WirelessStatusMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, WirelessStatusMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new WirelessStatusMenuItemAction(deleteWirelessStatusMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
