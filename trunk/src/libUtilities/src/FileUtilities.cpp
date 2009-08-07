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
