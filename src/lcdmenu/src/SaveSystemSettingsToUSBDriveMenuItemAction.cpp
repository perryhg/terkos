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

#include "SaveSystemSettingsToUSBDriveMenuItemAction.h"

const string SaveSystemSettingsToUSBDriveMenuItemAction::CLASS_NAME = "SaveSystemSettingsToUSBDriveMenuItemAction";

const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_MOUNTING_USB_DRIVE_PROPERTY = "status.mounting-usb-drive";
const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_MOUNTING_USB_DRIVE_DEFAULT_VALUE = "Mounting the USBdrive...";

const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_WRITING_TO_FILE_PROPERTY = "status.writing-to-file";
const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_WRITING_TO_FILE_DEFAULT_VALUE = "Writing to file:";

const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_SUCCESSFULLY_WROTE_PREFS_PROPERTY = "status.prefs-file-written-successfully";
const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_SUCCESSFULLY_WROTE_PREFS_DEFAULT_VALUE = "Done writing.   Unmounting...";

const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_FAILURE_COULD_NOT_WRITE_FILE_PROPERTY = "status.could-not-write-file";
const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_FAILURE_COULD_NOT_WRITE_FILE_DEFAULT_VALUE = "Write failed.   Unmounting...";

const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_SUCCESS_PROPERTY = "status.usb-unmount-success";
const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_SUCCESS_DEFAULT_VALUE = "You may remove  the USB drive.";

const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_FAILURE_PROPERTY = "status.usb-unmount-failure";
const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_FAILURE_DEFAULT_VALUE = "Failed to un-   mount USB drive.";

const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_FAILURE_PROPERTY = "status.failure";
const string SaveSystemSettingsToUSBDriveMenuItemAction::STATUS_FAILURE_DEFAULT_VALUE = "USB drive is notinstalled.";

const string SaveSystemSettingsToUSBDriveMenuItemAction::CONFIG_FILE_BASE_NAME_PROPERTY = "config.file-base-name";
const string SaveSystemSettingsToUSBDriveMenuItemAction::CONFIG_FILE_BASE_NAME_DEFAULT_VALUE = "terkos";
const string SaveSystemSettingsToUSBDriveMenuItemAction::CONFIG_FILE_EXTENSION_PROPERTY = "config.file-extension";
const string SaveSystemSettingsToUSBDriveMenuItemAction::CONFIG_FILE_EXTENSION_DEFAULT_VALUE = ".cfg";

const int SaveSystemSettingsToUSBDriveMenuItemAction::SLEEP_TIME_MICROSECONDS = 3000000;

void SaveSystemSettingsToUSBDriveMenuItemAction::activate()
   {
   getCharacterDisplay()->setText(getProperty(STATUS_MOUNTING_USB_DRIVE_PROPERTY, STATUS_MOUNTING_USB_DRIVE_DEFAULT_VALUE));

   // sleep to give the user time to read the status message
   usleep(1500000);

   FilesystemManager filesystemManager;
   bool isMounted = filesystemManager.mountFilesystem(FilesystemManager::USB_DRIVE_FILESYSTEM_NAME, FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME);

   if (isMounted)
      {
      // get the prefs
      TerkOSConfigManager configManager;
      Json::Value config = configManager.getJSON();

      // try to find a unique filename
      bool foundUniqueFilename = false;
      string configFilename = configBaseFilename + configFilenameExtension;
      string configAbsPath = configDirectory + configFilename;
      if (FileUtilities::fileExists(configAbsPath))
         {
         for (unsigned int i = 0; i < 1000; i++)
            {
            configFilename = configBaseFilename + StringUtilities::convertIntToString(i, 2, '0') + configFilenameExtension;
            configAbsPath = configDirectory + configFilename;
            if (!FileUtilities::fileExists(configAbsPath))
               {
               foundUniqueFilename = true;
               break;
               }
            }
         }
      else
         {
         foundUniqueFilename = true;
         }

      if (foundUniqueFilename)
         {
         getCharacterDisplay()->setLine(0, getProperty(STATUS_WRITING_TO_FILE_PROPERTY, STATUS_WRITING_TO_FILE_DEFAULT_VALUE));
         getCharacterDisplay()->setLine(1, configFilename);
         
         // write the config
         const bool wasWrittenSuccessfully = writeConfig(config, configAbsPath);

         // sleep to give the user time to read the status message
         usleep(SLEEP_TIME_MICROSECONDS);

         if (wasWrittenSuccessfully)
            {
            cout << "SaveSystemSettingsToUSBDriveMenuItemAction::writeConfig(): Done writing to config file [" << configAbsPath << "]" << endl;
            getCharacterDisplay()->setText(getProperty(STATUS_SUCCESSFULLY_WROTE_PREFS_PROPERTY, STATUS_SUCCESSFULLY_WROTE_PREFS_DEFAULT_VALUE));
            }
         else
            {
            cout << "SaveSystemSettingsToUSBDriveMenuItemAction::writeConfig(): could not open file for writing!" << endl;
            getCharacterDisplay()->setText(getProperty(STATUS_FAILURE_COULD_NOT_WRITE_FILE_PROPERTY, STATUS_FAILURE_COULD_NOT_WRITE_FILE_DEFAULT_VALUE));
            }

         // sleep to give the user time to read the status message
         usleep(SLEEP_TIME_MICROSECONDS);

         // unmount the USB drive
         const bool unmountStatus = filesystemManager.unmountFilesystem(FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME);

         // repeatedly check the mount point to see if it's still in use, checking once every 500 ms, for up to 10 seconds.
         for (unsigned int i = 0; i < 20; i++)
            {
            if (filesystemManager.isMountPointInUse(FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME))
               {
               usleep(500000);
               }
            else
               {
               break;
               }
            }
            
         // sleep for an extra second, just for safety
         usleep(1000000);

         if (unmountStatus)
            {
            cout << "SaveSystemSettingsToUSBDriveMenuItemAction::activate(): successfully unmounted USB drive" << endl;
            getCharacterDisplay()->setText(getProperty(STATUS_USB_UNMOUNT_SUCCESS_PROPERTY, STATUS_USB_UNMOUNT_SUCCESS_DEFAULT_VALUE));
            }
         else
            {
            cout << "SaveSystemSettingsToUSBDriveMenuItemAction::activate(): failed to unmount USB drive" << endl;
            getCharacterDisplay()->setText(getProperty(STATUS_USB_UNMOUNT_FAILURE_PROPERTY, STATUS_USB_UNMOUNT_FAILURE_DEFAULT_VALUE));
            }
         }
      else
         {
         cout << "SaveSystemSettingsToUSBDriveMenuItemAction::activate(): could not find unique filename!" << endl;
         getCharacterDisplay()->setText(getProperty(STATUS_FAILURE_COULD_NOT_WRITE_FILE_PROPERTY, STATUS_FAILURE_COULD_NOT_WRITE_FILE_DEFAULT_VALUE));
         }
      }
   else
      {
      cout << "SaveSystemSettingsToUSBDriveMenuItemAction::activate(): could not mount USB drive" << endl;
      getCharacterDisplay()->setText(getProperty(STATUS_FAILURE_PROPERTY, STATUS_FAILURE_DEFAULT_VALUE));
      }
      
   sleepThenPopUpToParentMenuItem();
   }

const bool SaveSystemSettingsToUSBDriveMenuItemAction::writeConfig(Json::Value config, const string configAbsPath)
   {
   cout << "SaveSystemSettingsToUSBDriveMenuItemAction::writeConfig(): Trying to write config file [" << configAbsPath << "]" << endl;
   ofstream configFile(configAbsPath.c_str());

   if (configFile.is_open())
      {
      configFile << config << endl;
      configFile.close();
      return true;
      }
   return false;
   }

void SaveSystemSettingsToUSBDriveMenuItemAction::sleepThenPopUpToParentMenuItem()
   {
   usleep(SLEEP_TIME_MICROSECONDS);
   CharacterDisplayMenuItemAction::stop();
   }

// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteSaveSystemSettingsToUSBDriveMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadSaveSystemSettingsToUSBDriveMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(SaveSystemSettingsToUSBDriveMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, SaveSystemSettingsToUSBDriveMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new SaveSystemSettingsToUSBDriveMenuItemAction(deleteSaveSystemSettingsToUSBDriveMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
