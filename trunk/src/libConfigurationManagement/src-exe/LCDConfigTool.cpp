/*
 * Simple program enabling access to the LCDConfigManager so that
 * the LCD configuration can be set via the command line.
 *
 * The configuration is set by giving this program JSON code on STDIN.  See the
 * LCDConfigManager class and lcd_config.json for details on the JSON format.
 *
 * To reset the configuration, call the program with a "--reset" argument.  The program
 * will print "1" if the reset was successful and "0" otherwise.
 *
 * To apply the configuration to the system, call the program with a "--apply-config"
 * argument.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <LCDConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      if (strcmp(argv[1], "--reset") == 0)
         {
         LCDConfigManager configManager;
         
         cout << configManager.revertToDefault() << endl;
         }
      else if (strcmp(argv[1], "--apply-config") == 0)
         {
         LCDConfigManager configManager;

         configManager.applyConfiguration();
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
         LCDConfigManager configManager;

         if (!configManager.setJson(config))
            {
            return 1;
            }
         }
      }

   return 0;
   }