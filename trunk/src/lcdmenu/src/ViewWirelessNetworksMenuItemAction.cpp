/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ViewWirelessNetworksMenuItemAction.h"

const string ViewWirelessNetworksMenuItemAction::CLASS_NAME = "ViewWirelessNetworksMenuItemAction";

void ViewWirelessNetworksMenuItemAction::activate()
   {
   getCharacterDisplay()->setText("Scanning...");

   // reset the current servo id
   currentAccessPointIndex = 0;

   // call the Perl script which returns iwlist results as JSON and the parse the JSON
   try
      {
      // try to load the file
      redi::ipstream is("scripts/getWirelessAccessPointsAsJSON.pl");
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(is, accessPoints, true);
      is.close();
      if (!parsingSuccessful)
         {
         // report to the user the failure and their locations in the document.
         cerr << "ViewWirelessNetworksMenuItemAction(): failed to parse iwlist JSON:" << endl << reader.getFormatedErrorMessages();
         accessPoints = Json::Value::null;
         }
      }
   catch (...)
      {
      cerr << "ViewWirelessNetworksMenuItemAction(): failed to parse iwlist JSON" << endl;
      accessPoints = Json::Value::null;
      }

   // TODO: sort list of access points by signal strength, filter out encrypted networks
   
   getCharacterDisplay()->clear();

   displayAccessPointDetails();
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
      if (currentAccessPointIndex == 0)
         {
         currentAccessPointIndex = getNumberOfAccessPoints() - 1;
         }
      else
         {
         currentAccessPointIndex--;
         }

      displayAccessPointDetails();
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
      if (currentAccessPointIndex >= getNumberOfAccessPoints() - 1)
         {
         currentAccessPointIndex = 0;
         }
      else
         {
         currentAccessPointIndex++;
         }

      displayAccessPointDetails();
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

void ViewWirelessNetworksMenuItemAction::displayAccessPointDetails()
   {
   Json::Value ssid = accessPoints["access-points"][currentAccessPointIndex]["ssid"];

   string networkLine = "Network: ";
   networkLine += StringUtilities::convertIntToString(currentAccessPointIndex + 1, 2, '0');
   networkLine += "/";
   networkLine += StringUtilities::convertIntToString(getNumberOfAccessPoints(), 2, '0');
   getCharacterDisplay()->setTextWithScrollArrows(networkLine + ssid.asString());
   }

const unsigned int ViewWirelessNetworksMenuItemAction::getNumberOfAccessPoints() const
   {
   if (accessPoints != Json::Value::null)
      {
      return accessPoints["access-points"].size();
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
