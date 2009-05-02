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

   // Get the loadObject() function.  If it doesn't exist, return NULL.
   void* loadSym = dlsym(_objFile, "loadObject");
   if (loadSym == NULL)
      {
      return NULL;
      }

   // Load a new instance of the requested class, and return it
   void* obj = ((void* (*)(const char*, int, void**)) (loadSym))(name, argc, argv);
   return reinterpret_cast<DynamicObject*> (obj);
   }

