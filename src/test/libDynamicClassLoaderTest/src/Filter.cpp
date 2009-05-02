#include "Filter.h"

Filter::Filter(void(*delObj)(void*)) :
   DynamicObject(delObj)
   {
   }

Filter::~Filter(void)
   {
   }
