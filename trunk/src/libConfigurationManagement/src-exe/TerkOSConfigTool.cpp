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

#include <vector>
#include <json/json.h>
#include <AudioConfigManager.h>
#include <UserProgramsConfigManager.h>
#include <FirmwareVersionManager.h>
#include <LCDConfigManager.h>
#include <VersionInfoManager.h>
#include <WirelessNetworkingConfigManager.h>
#include <StringUtilities.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      if (strcmp(argv[1], "--json") == 0)
         {
         // create the JSON response
         Json::Value terkOsElement;

         AudioConfigManager audioConfigManager;
         Json::Value audioJson = audioConfigManager.getJSON();
         if (audioJson != Json::Value::null)
            {
            Json::Value* audioElement = ConfigFile::findProperty(audioJson, "audio");
            if (audioElement != NULL)
               {
               terkOsElement["audio"] = *audioElement;
               }
            }

         WirelessNetworkingConfigManager wirelessNetworkingConfigManager;
         Json::Value wirelessJson = wirelessNetworkingConfigManager.getJSON();
         if (wirelessJson != Json::Value::null)
            {
            Json::Value* wirelessElement = ConfigFile::findProperty(wirelessJson, "wireless-networking");
            if (wirelessElement != NULL)
               {
               terkOsElement["wireless-networking"] = *wirelessElement;
               }
            }

         UserProgramsConfigManager userProgramsConfigManager;
         Json::Value userProgramsJson = userProgramsConfigManager.getJSON();
         if (userProgramsJson != Json::Value::null)
            {
            Json::Value* userProgramsElement = ConfigFile::findProperty(userProgramsJson, "user-programs");
            if (userProgramsElement != NULL)
               {
               terkOsElement["user-programs"] = *userProgramsElement;
               }
            }

         LCDConfigManager lcdConfigManager;
         Json::Value lcdJson = lcdConfigManager.getJSON();
         if (lcdJson != Json::Value::null)
            {
            Json::Value* lcdElement = ConfigFile::findProperty(lcdJson, "lcd");
            if (lcdElement != NULL)
               {
               terkOsElement["lcd"] = *lcdElement;
               }
            }

         VersionInfoManager versionInfoManager;
         Json::Value versionInfoJson = versionInfoManager.getJSON();
         if (versionInfoJson != Json::Value::null)
            {
            Json::Value* versionInfoElement = ConfigFile::findProperty(versionInfoJson, "version-info");
            if (versionInfoElement != NULL)
               {
               terkOsElement["version-info"] = *versionInfoElement;
               }
            }

         Json::Value outputRootElement;
         outputRootElement["terk-os"] = terkOsElement;
         cout << outputRootElement << endl;
         }
      }
   else
      {
      // create the root element of the response
      Json::Value outputRootElement;

      // create the vector containing the names of the updated components
      vector<string> updatedComponenets;

      // build the updatedComponenetsElement
      Json::Value updatedComponenetsElement(Json::arrayValue);

      // parse the JSON from stdin
      Json::Value config;
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(cin, config, true);

      // if parsing succeeded, then call the config manager and give it the JSON
      if (parsingSuccessful)
         {
         Json::Value* terkOsConfig = ConfigFile::findProperty(config, "terk-os");
         if (terkOsConfig != NULL)
            {
            Json::Value* revisionNumberElement = ConfigFile::findProperty(config, "terk-os.version-info.firmware.version.revision");
            if (revisionNumberElement != NULL)
               {
               // get current revision
               FirmwareVersionManager firmwareVersionManager;
               string currentRevisionStr = firmwareVersionManager.getRevision();

               int currentRevision = INT_MAX;
               if (!StringUtilities::convertStringToNum<int>(currentRevisionStr, currentRevision))
                  {
                  // TODO: replace with real logging
                  cerr << "TerkOSConfigTool:  failed to parse current firmware revision [" << currentRevisionStr << "]" << endl;
                  }

               // get the revision from the given JSON
               string newRevisionStr = revisionNumberElement->asString();

               int newRevision = INT_MAX;
               if (!StringUtilities::convertStringToNum<int>(newRevisionStr, newRevision))
                  {
                  // TODO: replace with real logging
                  cerr << "TerkOSConfigTool:  failed to parse firmware revision of given JSON [" << newRevisionStr << "]" << endl;
                  }

               // make sure the current revision greater than or equal to the revision in the given JSON
               if (currentRevision >= newRevision)
                  {
                  AudioConfigManager audioConfigManager;
                  if (audioConfigManager.setJson(*terkOsConfig))
                     {
                     updatedComponenets.push_back("audio");
                     }

                  // TODO: add support for updating LCD
                  
                  UserProgramsConfigManager userProgramsConfigManager;
                  if (userProgramsConfigManager.setJson(*terkOsConfig))
                     {
                     updatedComponenets.push_back("user-programs");
                     }

                  WirelessNetworkingConfigManager wirelessNetworkingConfigManager;
                  if (wirelessNetworkingConfigManager.setJson(*terkOsConfig))
                     {
                     updatedComponenets.push_back("wireless-networking");
                     }

                  // add the updated components to the JSON element
                  for (unsigned int i = 0; i < updatedComponenets.size(); i++)
                     {
                     updatedComponenetsElement[i] = updatedComponenets[i];
                     }

                  outputRootElement["ok"] = true;
                  outputRootElement["message"] = "configuration updated";
                  }
               else
                  {
                  outputRootElement["ok"] = false;
                  outputRootElement["message"] = "unknown firmware revision";
                  }
               }
            else
               {
               outputRootElement["ok"] = false;
               outputRootElement["message"] = "required JSON element missing (terk-os.version-info.firmware.version.revision)";
               }
            }
         else
            {
            outputRootElement["ok"] = false;
            outputRootElement["message"] = "required JSON element missing (terk-os)";
            }
         }
      else
         {
         outputRootElement["ok"] = false;
         outputRootElement["message"] = "invalid JSON";
         }

      outputRootElement["updated-components"] = updatedComponenetsElement;

      cout << outputRootElement << endl;
      }

   return 0;
   }
