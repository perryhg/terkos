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

#include <iostream>
#include <DynamicLibrary.h>
#include <DynamicLoader.h>
#include "Filter.h"

using namespace std;

int main(int argc, char** argv)
   {
   string filter1ClassName = "HighPassFilter";
   string filter2ClassName = "LowPassFilter";
   string filter1LibraryName = "lib" + filter1ClassName + ".so";
   string filter2LibraryName = "lib" + filter2ClassName + ".so";

   // Load the dynamic library with the high-pass filter class
   DynamicLibrary* dynLibrary1 = DynamicLoader::loadObjectFile(filter1LibraryName.c_str(), RTLD_NOW);
   if (dynLibrary1 == NULL)
      {
      cerr << "Couldn't load dynamic library 1." << endl;
      return -1;
      }

   // Load a HighPassFilter object
   Filter* filter1 = dynamic_cast<Filter*> (dynLibrary1->newObject(filter1ClassName.c_str(), 0, NULL));
   if (filter1 == NULL)
      {
      cerr << "Couldn't create filter object 1." << endl;
      return -1;
      }

   // Load the dynamic library with the low-pass filter class
   DynamicLibrary* dynLibrary2 = DynamicLoader::loadObjectFile(filter2LibraryName.c_str(), RTLD_NOW);
   if (dynLibrary2 == NULL)
      {
      cerr << "Couldn't load dynamic library 2." << endl;
      return -1;
      }

   // Load a LowPassFilter object
   Filter* filter2 = dynamic_cast<Filter*> (dynLibrary2->newObject(filter2ClassName.c_str(), 0, NULL));
   if (filter2 == NULL)
      {
      cerr << "Couldn't create filter object 2." << endl;
      return -1;
      }

   // A buffer for storing audio
   int bufLen;
   unsigned char* buffer;

   filter1->processAudio(bufLen, buffer);
   filter2->processAudio(bufLen, buffer);

   // Get rid of the filter objects
   filter1->deleteSelf();
   filter1 = NULL;
   filter2->deleteSelf();
   filter2 = NULL;

   // Close the dynamic libraries
   delete dynLibrary1;
   delete dynLibrary2;

   // Exit the program
   return 0;
   }
