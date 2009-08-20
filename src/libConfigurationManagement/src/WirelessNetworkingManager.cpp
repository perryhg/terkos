/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessNetworkingManager.h"

const bool WirelessNetworkingManager::enableWirelessNetworking()
   {
   // call the Perl script which enables wireless
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/enableWirelessNetworking.pl");

      // parse the stream to get the status
      Json::Value wirelessNetworkingStatus = WirelessNetworkingConfigManager::parseWirelessNetworkingStatusJSONStream(is);

      is.close();

      // check the status to see whether the enabling worked 
      return WirelessNetworkingConfigManager::parseJSONAndReturnWhetherWirelessNetworkingIsEnabled(wirelessNetworkingStatus);
      }
   catch (...)
      {
      cerr << "WirelessNetworkingManager::enableWirelessNetworking(): failed to call script to enable wireless networking." << endl;
      }

   return false;
   }

const bool WirelessNetworkingManager::disableWirelessNetworking()
   {
   // call the Perl script which disables wireless
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/disableWirelessNetworking.pl");

      // parse the stream to get the status
      Json::Value wirelessNetworkingStatus = WirelessNetworkingConfigManager::parseWirelessNetworkingStatusJSONStream(is);

      is.close();

      // check the status to see whether the disabling worked (we return
      // true if disabling worked, so take the not of isEnabled)
      return !WirelessNetworkingConfigManager::parseJSONAndReturnWhetherWirelessNetworkingIsEnabled(wirelessNetworkingStatus);
      }
   catch (...)
      {
      cerr << "WirelessNetworkingManager::disableWirelessNetworking(): failed to call script to disable wireless networking." << endl;
      }

   return false;
   }

Json::Value WirelessNetworkingManager::getWirelessNetworkingStatus()
   {
   // call the Perl script which returns the current wireless status
   redi::ipstream is("perl -I/opt/scripts /opt/scripts/getWirelessNetworkingStatusAsJSON.pl");
   Json::Value wirelessNetworkingStatusJson = WirelessNetworkingConfigManager::parseWirelessNetworkingStatusJSONStream(is);
   is.close();

   return wirelessNetworkingStatusJson;
   }
