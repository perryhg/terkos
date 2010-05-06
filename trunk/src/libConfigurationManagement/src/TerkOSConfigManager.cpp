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

#include "TerkOSConfigManager.h"

Json::Value TerkOSConfigManager::getJSON()
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

   return outputRootElement;
   }

Json::Value TerkOSConfigManager::setJSON(Json::Value& config)
   {
   // create the root element of the response
   Json::Value outputRootElement;

   // build the updatedComponenetsElement
   Json::Value updatedComponenetsElement(Json::arrayValue);

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

         // make sure the current revision is greater than or equal to the revision in the given JSON
         if (currentRevision >= newRevision)
            {
            // create the vector containing the names of the updated components
            vector<string> updatedComponenets;

            AudioConfigManager audioConfigManager;
            if (audioConfigManager.setJson(*terkOsConfig))
               {
               updatedComponenets.push_back("audio");
               }

            LCDConfigManager lcdConfigManager;
            if (lcdConfigManager.setJson(*terkOsConfig))
               {
               updatedComponenets.push_back("lcd");
               }

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

   outputRootElement["updated-components"] = updatedComponenetsElement;

   return outputRootElement;
   }
