/*
 * Simple program enabling access to the UserProgramsConfigManager so that
 * the user program configuration can be read or written via the command line.
 *
 * To read the configuration, call the program with a "--json" argument.  To
 * write, call the program with no arguments and then supply the JSON code on
 * STDIN.  See the UserProgramsConfigManager class and user_programs_config.json
 * for details on the JSON format.
 *
 * To get the absolute path to the program to run on boot, call the proram with
 * a "--program" argument.  If no program is specified to run on boot, this
 * program will print an empty line.
 *
 * To reset the configuration, call the program with a "--reset" argument.  The program
 * will print "1" if the reset was successful and "0" otherwise.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <UserProgramsConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      if (strcmp(argv[1], "--json") == 0)
         {
         UserProgramsConfigManager configManager;

         cout << configManager.getJSON() << endl;
         }
      else if (strcmp(argv[1], "--program") == 0)
         {
         UserProgramsConfigManager configManager;

         cout << configManager.getAbsolutePathToProgramToRunOnBoot() << endl;
         }
      else if (strcmp(argv[1], "--reset") == 0)
         {
         UserProgramsConfigManager configManager;

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
         UserProgramsConfigManager configManager;

         if (!configManager.setJson(config))
            {
            return 1;
            }
         }
      }

   return 0;
   }
