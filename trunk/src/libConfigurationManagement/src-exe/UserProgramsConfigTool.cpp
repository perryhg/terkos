/*
 * Simple program enabling access to the UserProgramsConfigManager so that
 * the user program configuration can be read or written via the command line.
 *
 * To read the configuration, call the program with a "--read" argument.  To
 * write, call the program with no arguments and then supply the JSON code on
 * STDIN.  See the UserProgramsConfigManager class and user_programs_config.json
 * for details on the JSON format.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <UserProgramsConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1 && strcmp(argv[1], "--read") == 0)
      {
      UserProgramsConfigManager configManager;

      cout << configManager.getJSON() << endl;
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
         UserProgramsConfigManager configManager;

         if (!configManager.setJson(config))
            {
            return 1;
            }
         }
      }

   return 0;
   }
