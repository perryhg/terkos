//======================================================================================================================
// The DynamicObject class deﬁnes a deleteObject() function and operates as a common base class for all dynamically
// loaded objects.
//
// Code is taken from the Dr. Dobbs Journal article "Dynamically Loaded C++ Objects" by William Nagel (http://www.ddj.com/cpp/184401900)
//======================================================================================================================

#include "DynamicObject.h"

DynamicObject::DynamicObject(void(*delObj)(void*)) :
   _deleteObject(delObj)
   {
   }

DynamicObject::~DynamicObject(void)
   {
   }

void DynamicObject::deleteSelf(void)
   {
   (*_deleteObject)(reinterpret_cast<void*> (this));
   }
