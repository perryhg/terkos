/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ViewWirelessNetworksMenuItemAction.h"

const string ViewWirelessNetworksMenuItemAction::CLASS_NAME = "ViewWirelessNetworksMenuItemAction";

void ViewWirelessNetworksMenuItemAction::activate()
   {
   getCharacterDisplay()->setText("Scanning...");

   // reset the current servo id
   currentWirelessNetworkIndex = 0;

   // call the Perl script which returns iwlist results as JSON and the parse the JSON
   try
      {
      // execute the script and return the results as a stream (for now, only return unencrypted networks)
      redi::ipstream is("perl -Iscripts scripts/getWirelessNetworksAsJSON.pl --include-encrypted=false");

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

   // TODO: add error handling here if no networks were found (display something to the user.)

   getCharacterDisplay()->clear();

   displayWirelessNetworkDetails();
   }

void ViewWirelessNetworksMenuItemAction::start()
   {
   if (isDetailViewMode)
      {
      // TODO
      }
   else
      {
      // TODO
      }
   }

void ViewWirelessNetworksMenuItemAction::stop()
   {
   if (isDetailViewMode)
      {
      // TODO
      }
   else
      {
      CharacterDisplayMenuItemAction::stop();
      }
   }

void ViewWirelessNetworksMenuItemAction::upEvent()
   {
   if (isDetailViewMode)
      {
      // TODO
      }
   else
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
   }

void ViewWirelessNetworksMenuItemAction::downEvent()
   {
   if (isDetailViewMode)
      {
      // TODO
      }
   else
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
   }

void ViewWirelessNetworksMenuItemAction::rightEvent()
   {
   if (isDetailViewMode)
      {
      // TODO
      }
   else
      {
      downEvent();
      }
   }

void ViewWirelessNetworksMenuItemAction::leftEvent()
   {
   if (isDetailViewMode)
      {
      // TODO
      }
   else
      {
      upEvent();
      }
   }

void ViewWirelessNetworksMenuItemAction::displayWirelessNetworkDetails()
   {
   Json::Value ssid = wirelessNetworks["wireless-networks"][currentWirelessNetworkIndex]["ssid"];

   string networkLine = "Network: ";
   networkLine += StringUtilities::convertIntToString(currentWirelessNetworkIndex + 1, 2, '0');
   networkLine += "/";
   networkLine += StringUtilities::convertIntToString(getNumberOfWirelessNetworks(), 2, '0');
   getCharacterDisplay()->setTextWithScrollArrows(networkLine + ssid.asString());
   }

const unsigned int ViewWirelessNetworksMenuItemAction::getNumberOfWirelessNetworks() const
   {
   if (wirelessNetworks != Json::Value::null)
      {
      return wirelessNetworks["wireless-networks"].size();
      }
   return 0;
   }

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
