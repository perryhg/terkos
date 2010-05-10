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

#include "LoadSystemSettingsFromUSBDriveMenuItemAction.h"

const string LoadSystemSettingsFromUSBDriveMenuItemAction::CLASS_NAME = "LoadSystemSettingsFromUSBDriveMenuItemAction";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_MOUNTING_USB_DRIVE_PROPERTY = "status.mounting-usb-drive";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_MOUNTING_USB_DRIVE_DEFAULT_VALUE = "Mounting the USBdrive...";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_UNMOUNTING_USB_DRIVE_PROPERTY = "status.unmounting-usb-drive";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_UNMOUNTING_USB_DRIVE_DEFAULT_VALUE = "Unmounting driveplease wait...";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_SUCCESS_PROPERTY = "status.usb-unmount-success";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_SUCCESS_DEFAULT_VALUE = "You may remove  the USB drive.";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_FAILURE_PROPERTY = "status.usb-unmount-failure";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_USB_UNMOUNT_FAILURE_DEFAULT_VALUE = "Failed to un-   mount USB drive.";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_LOAD_COMPLETED = "status.load-completed";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_LOAD_COMPLETED_DEFAULT_VALUE = "Prefs loaded    successfully.";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_LOAD_FAILED = "status.load-failed";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_LOAD_FAILED_DEFAULT_VALUE = "Failed to load  preferences.";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_LOAD_CANCELLED = "status.load-cancelled";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_LOAD_CANCELLED_DEFAULT_VALUE = "Cancelled -- No settings changed";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_FAILURE_PROPERTY = "status.failure";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::STATUS_FAILURE_DEFAULT_VALUE = "USB drive is notinstalled.";

const string LoadSystemSettingsFromUSBDriveMenuItemAction::FILE_PROPERTY = "label.file";
const string LoadSystemSettingsFromUSBDriveMenuItemAction::FILE_DEFAULT_VALUE = "File";

const int LoadSystemSettingsFromUSBDriveMenuItemAction::SLEEP_TIME_MICROSECONDS = 1500000;

void LoadSystemSettingsFromUSBDriveMenuItemAction::activate()
   {
   getCharacterDisplay()->setText(getProperty(STATUS_MOUNTING_USB_DRIVE_PROPERTY, STATUS_MOUNTING_USB_DRIVE_DEFAULT_VALUE));

   // sleep to give the user time to read the status message
   usleep(SLEEP_TIME_MICROSECONDS);

   FilesystemManager filesystemManager;
   bool isMounted = filesystemManager.mountFilesystem(FilesystemManager::USB_DRIVE_FILESYSTEM_NAME, FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME);

   if (isMounted)
      {
      // read the USB drive
      filenames = FileUtilities::getFilenamesInDirectory(FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME);
      selectedFilenameIndex = 0;
      displaySelectedFlename();
      }
   else
      {
      cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::activate(): could not mount USB drive" << endl;
      getCharacterDisplay()->setText(getProperty(STATUS_FAILURE_PROPERTY, STATUS_FAILURE_DEFAULT_VALUE));
      sleepThenPopUpToParentMenuItem();
      }
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::start()
   {
   string configFilename = FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME + "/" + getSelectedFilename();

   Json::Value configJson = loadConfigFile(configFilename);

   if (configJson != Json::Value::null)
      {
      TerkOSConfigManager configManager;

      Json::Value response = configManager.setJSON(configJson);

      if (response != Json::Value::null)
         {
         Json::Value okElement = response["ok"];
         if (okElement != Json::Value::null)
            {
            if (okElement.asBool())
               {
               cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::start(): successfully loaded preferences from file [" << configFilename << "]" << endl;
               getCharacterDisplay()->setText(getProperty(STATUS_LOAD_COMPLETED, STATUS_LOAD_COMPLETED_DEFAULT_VALUE));
               }
            else
               {
               cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::start(): failed to load preferences from file [" << configFilename << "].  The TerkOSConfigManager rejected it.  Response was: " << response << endl;
               getCharacterDisplay()->setText(getProperty(STATUS_LOAD_FAILED, STATUS_LOAD_FAILED_DEFAULT_VALUE));
               }
            }
         else
            {
            cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::start(): failed to load preferences from file [" << configFilename << "].  The TerkOSConfigManager returned a unexpected response: " << response << endl;
            getCharacterDisplay()->setText(getProperty(STATUS_LOAD_FAILED, STATUS_LOAD_FAILED_DEFAULT_VALUE));
            }
         }
      else
         {
         cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::start(): failed to load preferences from file [" << configFilename << "].  The TerkOSConfigManager rejected it, returning a null response." << endl;
         getCharacterDisplay()->setText(getProperty(STATUS_LOAD_FAILED, STATUS_LOAD_FAILED_DEFAULT_VALUE));
         }
      }
   else
      {
      cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::start(): failed to load preferences from file [" << configFilename << "].  File does not appear to be JSON." << endl;
      getCharacterDisplay()->setText(getProperty(STATUS_LOAD_FAILED, STATUS_LOAD_FAILED_DEFAULT_VALUE));
      }

   // sleep to give the user time to read the status message
   usleep(SLEEP_TIME_MICROSECONDS);

   unmountUSBDrive();

   sleepThenPopUpToParentMenuItem();
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::stop()
   {
   getCharacterDisplay()->setText(getProperty(STATUS_LOAD_CANCELLED, STATUS_LOAD_CANCELLED_DEFAULT_VALUE));

   // sleep to give the user time to read the status message
   usleep(SLEEP_TIME_MICROSECONDS);

   unmountUSBDrive();

   sleepThenPopUpToParentMenuItem();
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::upEvent()
   {
   // decrement the current servo ID, wrapping around if necessary
   if (selectedFilenameIndex == 0)
      {
      selectedFilenameIndex = getNumberOfFilenames() - 1;
      }
   else
      {
      selectedFilenameIndex--;
      }

   displaySelectedFlename();
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::downEvent()
   {
   // increment the current servo ID, wrapping around if necessary
   if (selectedFilenameIndex >= getNumberOfFilenames() - 1)
      {
      selectedFilenameIndex = 0;
      }
   else
      {
      selectedFilenameIndex++;
      }

   displaySelectedFlename();
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::rightEvent()
   {
   downEvent();
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::leftEvent()
   {
   upEvent();
   }

Json::Value LoadSystemSettingsFromUSBDriveMenuItemAction::loadConfigFile(const string absoluteFilePath)
   {
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("cat " + absoluteFilePath);

      // parse the stream
      Json::Reader reader;
      Json::Value json;
      bool parsingSuccessful = reader.parse(is, json, true);
      is.close();

      if (parsingSuccessful)
         {
         return json;
         }
      else
         {
         cerr << "LoadSystemSettingsFromUSBDriveMenuItemAction::loadConfigFile(): failed to read/parse JSON:" << endl << reader.getFormatedErrorMessages();
         }
      }
   catch (...)
      {
      cerr << "LoadSystemSettingsFromUSBDriveMenuItemAction::loadConfigFile(): failed to read/parse JSON" << endl;
      }

   return Json::Value::null;
   }

const bool LoadSystemSettingsFromUSBDriveMenuItemAction::unmountUSBDrive()
   {
   getCharacterDisplay()->setText(getProperty(STATUS_UNMOUNTING_USB_DRIVE_PROPERTY, STATUS_UNMOUNTING_USB_DRIVE_DEFAULT_VALUE));

   // unmount the USB drive
   FilesystemManager filesystemManager;
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
      cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::start(): successfully unmounted USB drive" << endl;
      getCharacterDisplay()->setText(getProperty(STATUS_USB_UNMOUNT_SUCCESS_PROPERTY, STATUS_USB_UNMOUNT_SUCCESS_DEFAULT_VALUE));
      }
   else
      {
      cout << "LoadSystemSettingsFromUSBDriveMenuItemAction::start(): failed to unmount USB drive" << endl;
      getCharacterDisplay()->setText(getProperty(STATUS_USB_UNMOUNT_FAILURE_PROPERTY, STATUS_USB_UNMOUNT_FAILURE_DEFAULT_VALUE));
      }

   return unmountStatus;
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::displaySelectedFlename()
   {
   string s1 = getProperty(FILE_PROPERTY, FILE_DEFAULT_VALUE) + ": ";
   string s2 = StringUtilities::convertIntToString(selectedFilenameIndex + 1);
   s2 += "/";
   s2 += StringUtilities::convertIntToString(getNumberOfFilenames());

   string line1 = StringUtilities::middlePadOrTrimRight(s1, s2, getCharacterDisplay()->getColumnsWhenInScrollingMode());
   getCharacterDisplay()->setTextWithScrollArrows(line1 + getSelectedFilename());
   }

void LoadSystemSettingsFromUSBDriveMenuItemAction::sleepThenPopUpToParentMenuItem()
   {
   usleep(SLEEP_TIME_MICROSECONDS);
   CharacterDisplayMenuItemAction::stop();
   }

// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteLoadSystemSettingsFromUSBDriveMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadLoadSystemSettingsFromUSBDriveMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(LoadSystemSettingsFromUSBDriveMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, LoadSystemSettingsFromUSBDriveMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new LoadSystemSettingsFromUSBDriveMenuItemAction(deleteLoadSystemSettingsFromUSBDriveMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
