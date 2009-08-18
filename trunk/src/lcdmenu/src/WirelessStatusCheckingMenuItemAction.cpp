/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessStatusCheckingMenuItemAction.h"

void WirelessStatusCheckingMenuItemAction::activate()
   {
   cout << "WirelessStatusCheckingMenuItemAction::activate()" << endl;

   Json::Value wirelessNetworkingStatusJson = getWirelessNetworkingStatus();

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
   // call the Perl script which returns the current wireless status
   redi::ipstream is("perl -I/opt/scripts /opt/scripts/getWirelessNetworkingStatusAsJSON.pl");
   Json::Value wirelessNetworkingStatusJson = parseJSONStream(is);
   is.close();

   return wirelessNetworkingStatusJson;
   }

const bool WirelessStatusCheckingMenuItemAction::parseJSONAndReturnWhetherWirelessNetworkingIsEnabled(const Json::Value& wirelessNetworkingStatusJson)
   {
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
               return (isEnabled != Json::Value::null && isEnabled.asBool());
               }
            }
         }
      }

   return false;
   }

Json::Value WirelessStatusCheckingMenuItemAction::parseJSONStream(redi::ipstream& is)
   {
   Json::Value wirelessNetworkingStatusJson = Json::Value::null;

   // call the Perl script which returns the current wireless status
   try
      {
      // parse the stream
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(is, wirelessNetworkingStatusJson, true);

      if (!parsingSuccessful)
         {
         // report to the user the failure and their locations in the document.
         cerr << "WirelessStatusCheckingMenuItemAction::getWirelessNetworkingStatus(): failed to parse wireless networking status JSON:" << endl
                  << reader.getFormatedErrorMessages();
         wirelessNetworkingStatusJson = Json::Value::null;
         }
      }
   catch (...)
      {
      cerr << "WirelessStatusCheckingMenuItemAction::getWirelessNetworkingStatus(): failed to parse wireless networking status JSON" << endl;
      wirelessNetworkingStatusJson = Json::Value::null;
      }

   return wirelessNetworkingStatusJson;
   }

