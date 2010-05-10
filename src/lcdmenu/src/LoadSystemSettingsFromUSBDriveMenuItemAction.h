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

#ifndef LOADSYSTEMSETTINGSFROMUSBDRIVEMENUITEMACTION_H_
#define LOADSYSTEMSETTINGSFROMUSBDRIVEMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <pstream.h>
#include <FilesystemManager.h>
#include <TerkOSConfigManager.h>
#include <StringUtilities.h>
#include <FileUtilities.h>
#include "CharacterDisplayMenuItemAction.h"

using namespace std;
using namespace redi;

class LoadSystemSettingsFromUSBDriveMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string STATUS_MOUNTING_USB_DRIVE_PROPERTY;
      static const string STATUS_MOUNTING_USB_DRIVE_DEFAULT_VALUE;

      static const string STATUS_UNMOUNTING_USB_DRIVE_PROPERTY;
      static const string STATUS_UNMOUNTING_USB_DRIVE_DEFAULT_VALUE;

      static const string STATUS_USB_UNMOUNT_SUCCESS_PROPERTY;
      static const string STATUS_USB_UNMOUNT_SUCCESS_DEFAULT_VALUE;

      static const string STATUS_USB_UNMOUNT_FAILURE_PROPERTY;
      static const string STATUS_USB_UNMOUNT_FAILURE_DEFAULT_VALUE;

      static const string STATUS_LOAD_COMPLETED;
      static const string STATUS_LOAD_COMPLETED_DEFAULT_VALUE;

      static const string STATUS_LOAD_FAILED;
      static const string STATUS_LOAD_FAILED_DEFAULT_VALUE;

      static const string STATUS_LOAD_CANCELLED;
      static const string STATUS_LOAD_CANCELLED_DEFAULT_VALUE;

      static const string STATUS_FAILURE_PROPERTY;
      static const string STATUS_FAILURE_DEFAULT_VALUE;

      static const string FILE_PROPERTY;
      static const string FILE_DEFAULT_VALUE;

      LoadSystemSettingsFromUSBDriveMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                   CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~LoadSystemSettingsFromUSBDriveMenuItemAction()
         {
         // nothing to do
         }

      void activate();
      void start();
      void stop();
      void upEvent();
      void downEvent();
      void rightEvent();
      void leftEvent();

   private:

      static const int SLEEP_TIME_MICROSECONDS;

      vector<string> filenames;
      unsigned int selectedFilenameIndex;

      Json::Value loadConfigFile(const string absoluteFilePath);
      const bool unmountUSBDrive();
      void displaySelectedFlename();
      void sleepThenPopUpToParentMenuItem();

      const unsigned int getNumberOfFilenames() const
         {
         return filenames.size();
         }

      const string getSelectedFilename() const
         {
         return filenames[selectedFilenameIndex];
         }
   };

#endif /* LOADSYSTEMSETTINGSFROMUSBDRIVEMENUITEMACTION_H_ */
