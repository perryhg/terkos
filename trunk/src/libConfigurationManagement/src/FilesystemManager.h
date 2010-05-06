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

#ifndef FILESYSTEMMANAGER_H_
#define FILESYSTEMMANAGER_H_

#include <string.h>
#include <json/json.h>
#include <pstream.h>

using namespace std;
using namespace redi;

class FilesystemManager
   {
   public:

      static const string USB_DRIVE_FILESYSTEM_NAME;
      static const string USB_DRIVE_MOUNT_POINT_NAME;

      FilesystemManager()
         {
         // nothing to do
         }

      virtual ~FilesystemManager()
         {
         // nothing to do
         }

      /**
       * Returns true if a filesystem is mounted on the given mount point; false otherwise.
       */
      const bool isMountPointInUse(const string mountPoint);

      /**
       * Returns true if the given filesystem is mounted on the given mount point; false otherwise.
       */
      const bool isFilesystemMounted(const string filesystem, const string mountPoint);

      /**
       * Mounts the given filesystem on the given mount point and returns true upon success.  Note that if the given
       * filesystem is already mounted on the given mount point, then no error is thrown and the return value is still
       * true.
       */
      const bool mountFilesystem(const string filesystem, const string mountPoint);

      /**
       * Unmounts the filesystem on the given mount point and returns true upon success.  Note that if no filesystem is
       * mounted on the given mount point, then no error is thrown and the return value is still true.
       */
      const bool unmountFilesystem(const string mountPoint);

      Json::Value getFilesystemsAsJSON();

   private:

      Json::Value getMountPointJSON(const string mountPoint);
   };

#endif /* FILESYSTEMMANAGER_H_ */
