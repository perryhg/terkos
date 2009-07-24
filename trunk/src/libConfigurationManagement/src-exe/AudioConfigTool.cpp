/*
 * Simple program enabling access to the AudioConfigManager so that
 * the audio configuration can be set via the command line.
 *
 * The configuration is set by giving this program JSON code on STDIN.  See the
 * AudioConfigManager class and audio_config.json for details on the JSON format.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <AudioConfigManager.h>

using namespace std;

int main(int argc, char** argv)
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

   return 0;
   }
