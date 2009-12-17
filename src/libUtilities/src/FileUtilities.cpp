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

#include "FileUtilities.h"

vector<string> FileUtilities::getFilenamesInDirectory(const string& directoryPath, const bool filesOnly)
   {
   vector<string> result;
   DIR* dp = opendir(directoryPath.empty() ? "." : directoryPath.c_str());
   if (dp)
      {
      while (true)
         {
         dirent* de = readdir(dp);
         if (de == NULL)
            {
            break;
            }
         if (filesOnly && de->d_type != DT_REG)
            {
            continue;
            }
         result.push_back(string(de->d_name));
         }
      closedir(dp);
      sort(result.begin(), result.end());
      }
   return result;
   }

const bool FileUtilities::isReadable(const string& filename)
   {
   ifstream ifile(filename.c_str());
   return ifile;
   }
