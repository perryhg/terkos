//======================================================================================================================
// The DynamicLibrary class. This class will be created by the dynamic loader, and can be used to create objects out of
// a loaded library.
//
// Code is taken from the Dr. Dobbs Journal article "Dynamically Loaded C++ Objects" by William Nagel (http://www.ddj.com/cpp/184401900)
//======================================================================================================================

#ifndef __DYNAMICLIBRARY_H__
#define __DYNAMICLIBRARY_H__

#include <stdio.h>
#include <dlfcn.h>
#include <string>
#include "DynamicObject.h"

using namespace std;

class DynamicLibrary
   {
   public:
      // Destructor, closes the open shared library
      ~DynamicLibrary(void);

      // Creates a new instance of the named class, or returns NULL if the class isn't found. The array of void*'s takes
      // the arguments for the class's constructor, with argc giving the number of arguments.
      DynamicObject* newObject(const char* name, int argc, void** argv);

      // Declare the DynamicLoader as a friend, so it can instantiate DynamicLibrary.
      friend class DynamicLoader;

   protected:
      // The handle to the shared library that was opened
      void *_objFile;

      // Creates a new library, with the object file handle that is passed in.  Protected so that only the DynamicLoader
      // can create an instance (since it is declared friend).
      DynamicLibrary(void* objFile);

   private:
      // Since an instance of DynamicLibrary manages lifetime of an open library, it is
      // important to make sure that the object isn't copied.
      DynamicLibrary(const DynamicLibrary&)
         {
         }
      DynamicLibrary& operator=(const DynamicLibrary& rhs)
         {
         return *this;
         }
   };

#endif /* __DYNAMICLIBRARY_H__ */
