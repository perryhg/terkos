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

//======================================================================================================================
// The DynamicLoader class, used for loading DynamicLibraries.
//
// Code is taken from the Dr. Dobbs Journal article "Dynamically Loaded C++ Objects" by William Nagel (http://www.ddj.com/cpp/184401900)
//======================================================================================================================

#include "DynamicLoader.h"

DynamicLibrary* DynamicLoader::loadObjectFile(const char* file, int flags)
   {
   void* objFile = dlopen(file, flags);
   if (objFile == NULL)
      {
      printf("DynamicLoader::loadObjectFile(%s): dlopen returned NULL.  Reason = [%s]\n",file,dlerror());
      return NULL;
      }
   return new DynamicLibrary(objFile);
   }

