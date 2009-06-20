/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessStatusCheckingMenuItemAction.h"

void WirelessStatusCheckingMenuItemAction::activate()
   {
   cout << "WirelessStatusCheckingMenuItemAction::activate()" << endl;

   // call the Perl script which returns the current wireless status
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/getWirelessNetworkingStatusAsJSON.pl");

      // parse the stream
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(is, wirelessNetworkingStatusJson, true);
      is.close();

      if (!parsingSuccessful)
         {
         // report to the user the failure and their locations in the document.
         cerr << "WirelessStatusCheckingMenuItemAction(): failed to parse wireless networking status JSON:" << endl
                  << reader.getFormatedErrorMessages();
         wirelessNetworkingStatusJson = Json::Value::null;
         }
      }
   catch (...)
      {
      cerr << "WirelessStatusCheckingMenuItemAction(): failed to parse wireless networking status JSON" << endl;
      wirelessNetworkingStatusJson = Json::Value::null;
      }

   if (wirelessNetworkingStatusJson != Json::Value::null)
      {
      Json::Value isInstalled = wirelessNetworkingStatusJson["wireless-networking-status"]["is-installed"];
      if (isInstalled != Json::Value::null)
         {
         if (isInstalled.asBool())
            {
            Json::Value wirelessInterface = wirelessNetworkingStatusJson["wireless-networking-status"]["wireless-interface"];
            if (wirelessInterface != Json::Value::null)
               {
               Json::Value isEnabled = wirelessInterface["is-enabled"];
               if (isEnabled != Json::Value::null && isEnabled.asBool())
                  {
                  handleWirelessEnabled();
                  }
               else
                  {
                  handleWirelessDisabled();
                  }
               }
            else
               {
               handleWirelessStatusFailure();
               }
            }
         else
            {
            handleWirelessUnplugged();
            }
         }
      else
         {
         handleWirelessStatusFailure();
         }
      }
   else
      {
      handleWirelessStatusFailure();
      }
   }

Json::Value WirelessStatusCheckingMenuItemAction::getWirelessNetworkingStatus()
   {
   return wirelessNetworkingStatusJson;
   }
