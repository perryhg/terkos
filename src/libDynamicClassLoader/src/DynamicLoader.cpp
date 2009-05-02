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
      return NULL;
      }
   return new DynamicLibrary(objFile);
   }

