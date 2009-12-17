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
