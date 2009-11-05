/*
 * Simple program enabling access to the AudioConfigManager so that
 * the audio configuration can be set via the command line.
 *
 * The configuration is set by giving this program JSON code on STDIN.  See the
 * AudioConfigManager class and audio_config.json for details on the JSON format.
 *
 * To reset the configuration, call the program with a "--reset" argument.  The program
 * will print "1" if the reset was successful and "0" otherwise.
 *
 * To apply the configuration to the system, call the program with a "--apply-config"
 * argument.
 *
 * To play a sample sound (e.g. to test the volume level), call the program
 * with a "--play-sample" argument followed by a integer in the range [0,10]
 * which specifies the volume.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <AudioConfigManager.h>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      if (strcmp(argv[1], "--reset") == 0)
         {
         AudioConfigManager configManager;
         
         cout << configManager.revertToDefault() << endl;
         }
      else if (strcmp(argv[1], "--apply-config") == 0)
         {
         AudioConfigManager configManager;

         configManager.applyConfiguration();
         }
      else if (argc > 2 && strcmp(argv[1], "--play-sample") == 0)
         {
         int volume = atoi(argv[2]);

         AudioConfigManager configManager;
         
         configManager.playSampleSound((unsigned char)volume);
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
         AudioConfigManager configManager;

         if (!configManager.setJson(config))
            {
            return 1;
            }
         }
      }

   return 0;
   }
