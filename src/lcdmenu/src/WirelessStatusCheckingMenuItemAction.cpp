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
      redi::ipstream is("perl -Iscripts scripts/getWirelessNetworkingStatusAsJSON.pl");

      // parse the stream
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(is, wirelessNetworkingStatus, true);
      is.close();

      if (!parsingSuccessful)
         {
         // report to the user the failure and their locations in the document.
         cerr << "WirelessStatusCheckingMenuItemAction(): failed to parse wireless networking status JSON:" << endl
                  << reader.getFormatedErrorMessages();
         wirelessNetworkingStatus = Json::Value::null;
         }
      }
   catch (...)
      {
      cerr << "WirelessStatusCheckingMenuItemAction(): failed to parse wireless networking status JSON" << endl;
      wirelessNetworkingStatus = Json::Value::null;
      }

   if (wirelessNetworkingStatus != Json::Value::null)
      {
      Json::Value isInstalled = wirelessNetworkingStatus["wireless-networking-status"]["is-installed"];
      if (isInstalled != Json::Value::null)
         {
         if (isInstalled.asBool())
            {
            Json::Value wirelessInterface = wirelessNetworkingStatus["wireless-networking-status"]["wireless-interface"];
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
