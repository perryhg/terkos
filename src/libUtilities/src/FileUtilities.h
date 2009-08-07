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
