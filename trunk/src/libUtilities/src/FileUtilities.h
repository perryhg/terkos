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

#ifndef FILEUTILITIES_H_
#define FILEUTILITIES_H_

#include <algorithm>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>

using namespace std;

class FileUtilities
   {
   public:

      /**
       * Return an ASCII-sorted vector of filename entries in a given directory.
       * If no path is specified, the current working directory is used.  Will
       * return a listing of files only unless the filesOnly argument is given and
       * set to false.
       *
       * Code taken from:  http://www.cplusplus.com/forum/unices/3548/
       */
      static vector<string> getFilenamesInDirectory(const string& directoryPath = string(), const bool filesOnly = true);

      /**
       * Copies the file referenced by the given source path to the destination path.
       */
      static void copyFile(const string& source, const string& destination)
         {
         string copyCommand = "cp " + source + " " + destination;
         system(copyCommand.c_str());
         }

      /**
       * Returns true if the file represented by the given file path is readable, false otherwise.
       */
      static const bool isReadable(const string& filename);

      /**
       * Returns true if the file represented by the given file path exists, false otherwise.
       */
      static const bool fileExists(const string& filename);

   private:

      FileUtilities()
         {
         // nothing to do
         }

      virtual ~FileUtilities()
         {
         // nothing to do
         }
   };

#endif /* FILEUTILITIES_H_ */
