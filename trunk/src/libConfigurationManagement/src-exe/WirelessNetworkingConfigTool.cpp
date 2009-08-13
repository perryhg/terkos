/*
 * Simple program enabling access to the WirelessNetworkingConfigManager so that
 * the wireless networking configuration can be set via the command line.
 *
 * The configuration is set by giving this program JSON code on STDIN.  See the
 * WirelessNetworkingConfigManager class and wireless_networking_config.json for
 * details on the JSON format.
 *
 * To reset the configuration, call the program with a "--reset" argument.  The program
 * will print "1" if the reset was successful and "0" otherwise.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <WirelessNetworkingConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      if (strcmp(argv[1], "--reset") == 0)
         {
         WirelessNetworkingConfigManager configManager;
         
         cout << configManager.revertToDefault() << endl;
         }
      }
   else
      {
      // parse the JSON from stdin
      Json::Value config;
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(cin, config, true);

      // if parsing succeeded, then call the config manager and give it the JSON
      if (parsingSuccessful)
         {
         WirelessNetworkingConfigManager configManager;

         if (!configManager.setJson(config))
            {
            return 1;
            }
         }
      }

   return 0;
   }
