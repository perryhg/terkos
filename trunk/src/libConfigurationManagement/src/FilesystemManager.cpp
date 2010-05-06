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

#include "FilesystemManager.h"

const string FilesystemManager::USB_DRIVE_FILESYSTEM_NAME = "/dev/sda1";
const string FilesystemManager::USB_DRIVE_MOUNT_POINT_NAME = "/media/sda1";

const bool FilesystemManager::isMountPointInUse(const string mountPoint)
   {
   return getMountPointJSON(mountPoint) != Json::Value::null;
   }

const bool FilesystemManager::isFilesystemMounted(const string filesystem, const string mountPoint)
   {
   Json::Value mountPointJSON = getMountPointJSON(mountPoint);
   if (mountPointJSON != Json::Value::null)
      {
      if (mountPointJSON["filesystem"] != Json::Value::null &&
          filesystem.compare(mountPointJSON["filesystem"].asString()) == 0)
         {
         return true;
         }
      }
   else
      {
      cout << "FilesystemManager::isFilesystemMounted(): JSON attribute mount-points['"<< mountPoint <<"'] is null, returning false" << endl;
      }
   return false;
   }

const bool FilesystemManager::mountFilesystem(const string filesystem, const string mountPoint)
   {
   if (isMountPointInUse(mountPoint))
      {
      return true;
      }

   // call mount command
   string command = "/bin/mount -t auto " + filesystem + " " + mountPoint + " && echo 1";
   redi::ipstream is(command);
   string line;
   getline(is, line);
   return (line == "1");
   }

const bool FilesystemManager::unmountFilesystem(const string mountPoint)
   {
   if (!isMountPointInUse(mountPoint))
      {
      return true;
      }

   // call unmount command
   string command = "/bin/umount " + mountPoint + " && echo 1";
   redi::ipstream is(command);
   string line;
   getline(is, line);
   return (line == "1");
   }

Json::Value FilesystemManager::getMountPointJSON(const string mountPoint)
   {
   Json::Value filesystemsJSON = getFilesystemsAsJSON();
   if (filesystemsJSON == Json::Value::null)
      {
      cout << "FilesystemManager::getMountPointJSON(): JSON is null, returning null" << endl;
      }
   else
      {
      if (filesystemsJSON["mount-points"] != Json::Value::null)
         {
         return filesystemsJSON["mount-points"][mountPoint];
         }
      else
         {
         cout << "FilesystemManager::getMountPointJSON(): JSON attribute mount-points is null, returning null" << endl;
         }
      }
   return Json::Value::null;
   }
   
Json::Value FilesystemManager::getFilesystemsAsJSON()
   {
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/getFilesystemsAsJSON.pl");

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
         cerr << "FilesystemManager(): failed to parse disk usage JSON:" << endl << reader.getFormatedErrorMessages();
         }
      }
   catch (...)
      {
      cerr << "FilesystemManager(): failed to parse disk usage JSON" << endl;
      }

   return Json::Value::null;
   }
