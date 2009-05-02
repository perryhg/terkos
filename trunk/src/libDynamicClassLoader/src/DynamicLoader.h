//======================================================================================================================
// The DynamicLoader class, used for loading DynamicLibraries.
//
// Code is taken from the Dr. Dobbs Journal article "Dynamically Loaded C++ Objects" by William Nagel (http://www.ddj.com/cpp/184401900)
//======================================================================================================================

#ifndef __DYNAMICLOADER_H__
#define __DYNAMICLOADER_H__

#include <stdio.h>
#include "DynamicLibrary.h"

class DynamicLoader
   {
   public:
      // Loads a DynamicLibrary, given the shared library file
      // "file", with the dlopen flags supplied.
      static DynamicLibrary* loadObjectFile(const char* file, int flags);
   };

#endif /* __DYNAMICLOADER_H__ */
