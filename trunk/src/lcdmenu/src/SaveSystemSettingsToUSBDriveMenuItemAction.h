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

#ifndef SAVESYSTEMSETTINGSTOUSBDRIVEMENUITEMACTION_H_
#define SAVESYSTEMSETTINGSTOUSBDRIVEMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <FilesystemManager.h>
#include <TerkOSConfigManager.h>
#include <StringUtilities.h>
#include <FileUtilities.h>
#include "NoOpMenuItemAction.h"
#include "CharacterDisplayMenuItemAction.h"

using namespace std;

class SaveSystemSettingsToUSBDriveMenuItemAction : public NoOpMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string STATUS_MOUNTING_USB_DRIVE_PROPERTY;
      static const string STATUS_MOUNTING_USB_DRIVE_DEFAULT_VALUE;

      static const string STATUS_WRITING_TO_FILE_PROPERTY;
      static const string STATUS_WRITING_TO_FILE_DEFAULT_VALUE;

      static const string STATUS_SUCCESSFULLY_WROTE_PREFS_PROPERTY;
      static const string STATUS_SUCCESSFULLY_WROTE_PREFS_DEFAULT_VALUE;

      static const string STATUS_FAILURE_COULD_NOT_WRITE_FILE_PROPERTY;
      static const string STATUS_FAILURE_COULD_NOT_WRITE_FILE_DEFAULT_VALUE;

      static const string STATUS_USB_UNMOUNT_SUCCESS_PROPERTY;
      static const string STATUS_USB_UNMOUNT_SUCCESS_DEFAULT_VALUE;

      static const string STATUS_USB_UNMOUNT_FAILURE_PROPERTY;
      static const string STATUS_USB_UNMOUNT_FAILURE_DEFAULT_VALUE;

      static const string STATUS_FAILURE_PROPERTY;
      static const string STATUS_FAILURE_DEFAULT_VALUE;

      static const string CONFIG_FILE_BASE_NAME_PROPERTY;
      static const string CONFIG_FILE_BASE_NAME_DEFAULT_VALUE;
      static const string CONFIG_FILE_EXTENSION_PROPERTY;
      static const string CONFIG_FILE_EXTENSION_DEFAULT_VALUE;

      SaveSystemSettingsToUSBDriveMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                   CharacterDisplay* characterDisplay, map<string, string>& properties) :
         NoOpMenuItemAction("", delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         configDirectory = FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME + "/";
         configBaseFilename = getProperty(CONFIG_FILE_BASE_NAME_PROPERTY, CONFIG_FILE_BASE_NAME_DEFAULT_VALUE);
         configFilenameExtension = getProperty(CONFIG_FILE_EXTENSION_PROPERTY, CONFIG_FILE_EXTENSION_DEFAULT_VALUE);
         }

      virtual ~SaveSystemSettingsToUSBDriveMenuItemAction()
         {
         // nothing to do
         }

      void activate();

   private:

      static const int SLEEP_TIME_MICROSECONDS;

      string configDirectory;
      string configBaseFilename;
      string configFilenameExtension;
      const bool writeConfig(Json::Value config, const string configFilename);
      void sleepThenPopUpToParentMenuItem();
   };

#endif /* SAVESYSTEMSETTINGSTOUSBDRIVEMENUITEMACTION_H_ */
