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
