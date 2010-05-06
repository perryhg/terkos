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
 * Program enabling access to the various config managers so that
 * the TerkOS configuration can be read or written via the command line.
 *
 * To read the configuration, call the program with a "--json" argument.  To
 * write, call the program with no arguments and then supply the JSON code on
 * STDIN.  The format is simply a single root node named "terk-os" which
 * contains each of the various configs.  E.g.:
 *
 *    {
 *       "terk-os" : {
 *          "audio" : {
 *             ...
 *          },
 *          "user-programs" : {
 *                ...
 *          },
 *          "wireless-networking" : {
 *                ...
 *          }
 *       }
 *    }
 *
 * The program will return JSON describing the result of the update.  E.g.:
 *
 *    {
 *       "message" : "configuration updated",
 *       "ok" : true,
 *       "updated-components" : [ "audio", "user-programs", "wireless-networking" ]
 *    }
 *
 * If the update failed, the "OK" element will be false and the message element will contain
 * a description of the problem.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <TerkOSConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      if (strcmp(argv[1], "--json") == 0)
         {
         TerkOSConfigManager configManager;

         cout << configManager.getJSON() << endl;
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
         TerkOSConfigManager configManager;

         Json::Value output = configManager.setJSON(config);
         cout << output << endl;

         if (output != Json::Value::null)
            {
            return 1;
            }
         }
      else
         {
         // create the root element of the response
         Json::Value outputRootElement;

         // build the updatedComponenetsElement
         Json::Value updatedComponenetsElement(Json::arrayValue);

         outputRootElement["ok"] = false;
         outputRootElement["message"] = "invalid JSON";
         outputRootElement["updated-components"] = updatedComponenetsElement;

         cout << outputRootElement << endl;
         }
      }

   return 0;
   }
