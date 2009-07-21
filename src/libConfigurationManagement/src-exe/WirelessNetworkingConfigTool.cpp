/*
 * Simple program enabling access to the WirelessNetworkingConfigManager so that
 * the wireless networking configuration can be set via the command line.
 *
 * The configuration is set by giving this program JSON code on STDIN.  See the
 * WirelessNetworkingConfigManager class and wireless_networking_config.json for
 * details on the JSON format.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <iostream>
#include <WirelessNetworkingConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   // parse the JSON from stdin
   Json::Value config;
   Json::Reader reader;
   bool parsingSuccessful = reader.parse(cin, config, true);
   if (parsingSuccessful)
      {
      WirelessNetworkingConfigManager configManager;
      configManager.setJson(config);
      }

   return 0;
   }
