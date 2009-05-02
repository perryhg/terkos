//======================================================================================================================
// The DynamicObject class deﬁnes a deleteObject() function and operates as a common base class for all dynamically
// loaded objects.
//
// Code is taken from the Dr. Dobbs Journal article "Dynamically Loaded C++ Objects" by William Nagel (http://www.ddj.com/cpp/184401900)
//======================================================================================================================

#ifndef __DYNAMICOBJECT_H__
#define __DYNAMICOBJECT_H__

#include <stdio.h>

class DynamicObject
   {
   private:
      // Callback function that should be called to delete dynamic object
      void (*_deleteObject)(void*);

   public:
      // The constructor sets the callback function to use
      DynamicObject(void(*delObj)(void*));

      // The destructor
      virtual ~DynamicObject(void);

      // Sends "this" to the callback destructor function.
      void deleteSelf(void);
   };

#endif /* __DYNAMICOBJECT_H__ */
