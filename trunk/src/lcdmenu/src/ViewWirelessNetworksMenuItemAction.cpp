/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ViewWirelessNetworksMenuItemAction.h"

const string ViewWirelessNetworksMenuItemAction::CLASS_NAME = "ViewWirelessNetworksMenuItemAction";

const string ViewWirelessNetworksMenuItemAction::STATUS_FAILURE_PROPERTY = "status.failure";
const string ViewWirelessNetworksMenuItemAction::STATUS_FAILURE_DEFAULT = "Failed to get   wireless status.";

const string ViewWirelessNetworksMenuItemAction::STATUS_UNPLUGGED_PROPERTY = "status.unplugged";
const string ViewWirelessNetworksMenuItemAction::STATUS_UNPLUGGED_DEFAULT = "Wireless adapteris unplugged.";

const string ViewWirelessNetworksMenuItemAction::STATUS_DISABLED_PROPERTY = "status.disabled";
const string ViewWirelessNetworksMenuItemAction::STATUS_DISABLED_DEFAULT = "Wireless is     turned off.";

const string ViewWirelessNetworksMenuItemAction::STATUS_NO_NETWORKS_FOUND_PROPERTY = "status.no-networks-found";
const string ViewWirelessNetworksMenuItemAction::STATUS_NO_NETWORKS_FOUND_DEFAULT = "No wireless     networks found.";

const string ViewWirelessNetworksMenuItemAction::SCANNING_LABEL_PROPERTY = "scanning.label";
const string ViewWirelessNetworksMenuItemAction::SCANNING_LABEL_DEFAULT = "Scanning...";

const string ViewWirelessNetworksMenuItemAction::NETWORK_LABEL_PROPERTY = "network.label";
const string ViewWirelessNetworksMenuItemAction::NETWORK_LABEL_DEFAULT = "Network";

const string ViewWirelessNetworksMenuItemAction::JOIN_NETWORK_PROMPT_PROPERTY = "join-network.prompt";
const string ViewWirelessNetworksMenuItemAction::JOIN_NETWORK_PROMPT_DEFAULT = "Join?";

const string ViewWirelessNetworksMenuItemAction::YES_OPTION_PROPERTY = "yes.option";
const string ViewWirelessNetworksMenuItemAction::YES_OPTION_DEFAULT = "Yes";

const string ViewWirelessNetworksMenuItemAction::NO_OPTION_PROPERTY = "no.option";
const string ViewWirelessNetworksMenuItemAction::NO_OPTION_DEFAULT = "No";

const string ViewWirelessNetworksMenuItemAction::CONNECTING_TO_LABEL_PROPERTY = "connecting-to.label";
const string ViewWirelessNetworksMenuItemAction::CONNECTING_TO_LABEL_DEFAULT = "Connecting to";

const string ViewWirelessNetworksMenuItemAction::STATUS_CONNECTION_SUCESS_PROPERTY = "status.connection.success";
const string ViewWirelessNetworksMenuItemAction::STATUS_CONNECTION_SUCESS_DEFAULT = "Connection      successful.";

const string ViewWirelessNetworksMenuItemAction::STATUS_CONNECTION_FAILURE_PROPERTY = "status.connection.failure";
const string ViewWirelessNetworksMenuItemAction::STATUS_CONNECTION_FAILURE_DEFAULT = "Connection      failed.";

void ViewWirelessNetworksMenuItemAction::activate()
   {
   WirelessStatusCheckingMenuItemAction::activate();
   currentMenuItemAction->activate();
   }

void ViewWirelessNetworksMenuItemAction::start()
   {
   currentMenuItemAction->start();
   }

void ViewWirelessNetworksMenuItemAction::stop()
   {
   currentMenuItemAction->stop();
   }

void ViewWirelessNetworksMenuItemAction::upEvent()
   {
   currentMenuItemAction->upEvent();
   }

void ViewWirelessNetworksMenuItemAction::downEvent()
   {
   currentMenuItemAction->downEvent();
   }

void ViewWirelessNetworksMenuItemAction::rightEvent()
   {
   currentMenuItemAction->rightEvent();
   }

void ViewWirelessNetworksMenuItemAction::leftEvent()
   {
   currentMenuItemAction->leftEvent();
   }

// ---------------------------------------------------------------------------------------------------------------------

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::activate()
   {
   if (isListMode)
      {
      getCharacterDisplay()->setText(getProperty(SCANNING_LABEL_PROPERTY, SCANNING_LABEL_DEFAULT));

      // reset the current servo id
      currentWirelessNetworkIndex = 0;

      // call the Perl script which returns iwlist results as JSON and the parse the JSON
      try
         {
         // execute the script and return the results as a stream (for now, only return unencrypted networks)
         redi::ipstream is("perl -I/opt/scripts /opt/scripts/getWirelessNetworksAsJSON.pl --include-encrypted=false");

         // parse the stream
         Json::Reader reader;
         bool parsingSuccessful = reader.parse(is, wirelessNetworks, true);
         is.close();

         if (!parsingSuccessful)
            {
            // report to the user the failure and their locations in the document.
            cerr << "ViewWirelessNetworksMenuItemAction(): failed to parse wireless networks JSON:" << endl << reader.getFormatedErrorMessages();
            wirelessNetworks = Json::Value::null;
            }
         }
      catch (...)
         {
         cerr << "ViewWirelessNetworksMenuItemAction(): failed to parse wireless networks JSON" << endl;
         wirelessNetworks = Json::Value::null;
         }

      getCharacterDisplay()->clear();

      displayWirelessNetworkDetails();
      }
   else
      {
      cout << "join mode::activate()" << endl;
      willJoinNetwork = true;
      getCharacterDisplay()->setLine(0, generateJoinNetworkChoiceLine());
      getCharacterDisplay()->setLine(1, getCurrentNetworkSSID());
      }
   }

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::start()
   {
   if (isListMode)
      {
      if (getNumberOfWirelessNetworks() > 0)
         {
         cout << "will go into detail mode for network #" << currentWirelessNetworkIndex << endl;
         switchToJoinNetworkMode();
         }
      else
         {
         stop();
         }
      }
   else
      {
      // todo
      cout << "join mode::start()" << endl;
      if (willJoinNetwork)
         {
         getCharacterDisplay()->setLine(0,getProperty(CONNECTING_TO_LABEL_PROPERTY, CONNECTING_TO_LABEL_DEFAULT));
         getCharacterDisplay()->setLine(1,getCurrentNetworkSSID() + "...");
         }
      else
         {
         stop();
         }
      }
   }

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::stop()
   {
   if (isListMode)
      {
      CharacterDisplayMenuItemAction::stop();
      }
   else
      {
      cout << "join mode::stop(): will go back into list mode" << endl;
      switchToListModeMode();
      }
   }

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::upEvent()
   {
   if (isListMode)
      {
      // decrement the current servo ID, wrapping around if necessary
      if (currentWirelessNetworkIndex == 0)
         {
         currentWirelessNetworkIndex = getNumberOfWirelessNetworks() - 1;
         }
      else
         {
         currentWirelessNetworkIndex--;
         }

      displayWirelessNetworkDetails();
      }
   else
      {
      cout << "join mode::upEvent()" << endl;
      CharacterDisplayMenuItemAction::upEvent();
      }
   }

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::downEvent()
   {
   if (isListMode)
      {
      // increment the current servo ID, wrapping around if necessary
      if (currentWirelessNetworkIndex >= getNumberOfWirelessNetworks() - 1)
         {
         currentWirelessNetworkIndex = 0;
         }
      else
         {
         currentWirelessNetworkIndex++;
         }

      displayWirelessNetworkDetails();
      }
   else
      {
      cout << "join mode::downEvent()" << endl;
      CharacterDisplayMenuItemAction::downEvent();
      }
   }

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::rightEvent()
   {
   if (isListMode)
      {
      downEvent();
      }
   else
      {
      cout << "join mode::rightEvent()" << endl;
      leftEvent();
      }
   }

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::leftEvent()
   {
   if (isListMode)
      {
      upEvent();
      }
   else
      {
      cout << "join mode::leftEvent()" << endl;
      willJoinNetwork = !willJoinNetwork;
      getCharacterDisplay()->setLine(0, generateJoinNetworkChoiceLine());
      }
   }

void ViewWirelessNetworksMenuItemAction::WirelessEnabledMenuItemAction::displayWirelessNetworkDetails()
   {
   if (getNumberOfWirelessNetworks() > 0)
      {
      string networkLine = getProperty(NETWORK_LABEL_PROPERTY, NETWORK_LABEL_DEFAULT) + ": ";
      networkLine += StringUtilities::convertIntToString(currentWirelessNetworkIndex + 1, 2, '0');
      networkLine += "/";
      networkLine += StringUtilities::convertIntToString(getNumberOfWirelessNetworks(), 2, '0');
      getCharacterDisplay()->setTextWithScrollArrows(networkLine + getCurrentNetworkSSID());
      }
   else
      {
      getCharacterDisplay()->setText(getProperty(STATUS_NO_NETWORKS_FOUND_PROPERTY, STATUS_NO_NETWORKS_FOUND_DEFAULT));
      }
   }

// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteViewWirelessNetworksMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadViewWirelessNetworksMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(ViewWirelessNetworksMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ViewWirelessNetworksMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ViewWirelessNetworksMenuItemAction(deleteViewWirelessNetworksMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
