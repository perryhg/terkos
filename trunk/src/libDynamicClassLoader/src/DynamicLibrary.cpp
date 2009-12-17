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
// The DynamicLibrary class. This class will be created by the dynamic loader, and can be used to create objects out of
// a loaded library.
//
// Code is taken from the Dr. Dobbs Journal article "Dynamically Loaded C++ Objects" by William Nagel (http://www.ddj.com/cpp/184401900)
//======================================================================================================================

#include "DynamicLibrary.h"

DynamicLibrary::DynamicLibrary(void* objFile) :
   _objFile(objFile)
   {
   }

DynamicLibrary::~DynamicLibrary(void)
   {
   dlclose(_objFile);
   }

DynamicObject* DynamicLibrary::newObject(const char* name, int argc, void** argv)
   {
   // If there is no valid library, return null
   if (_objFile == NULL)
      {
      return NULL;
      }

   // Try to find the loadObject() function using the convention "load"+<classname>+"Object".
   // If it doesn't exist, try using simply "loadObject".  If that doesn't exist either, then
   // just return NULL.
   std::string className(name);
   std::string functionName = "load" + className + "Object";
   void* loadSym = dlsym(_objFile, functionName.c_str());
   if (loadSym == NULL)
      {
      loadSym = dlsym(_objFile, "loadObject");
      if (loadSym == NULL)
         {
         return NULL;
         }
      }

   // Load a new instance of the requested class, and return it
   void* obj = ((void* (*)(const char*, int, void**)) (loadSym))(name, argc, argv);
   return reinterpret_cast<DynamicObject*> (obj);
   }

