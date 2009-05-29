#include "HighPassFilter.h"

const string HighPassFilter::CLASS_NAME = "HighPassFilter";

HighPassFilter::HighPassFilter(void(*delObj)(void*)) :
   Filter(delObj)
   {
   }

HighPassFilter::~HighPassFilter(void)
   {
   }

void HighPassFilter::processAudio(int size, unsigned char* buffer)
   {
   // apply a high pass filter to the buffer
   std::cout << "Hello from the high pass filter" << std::endl;
   }

// The Dynamic library should also contain the following external C function definitions
extern "C"
   {
   void deleteHighPassFilterObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadHighPassFilterObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(HighPassFilter::CLASS_NAME.c_str()));
      if (strncmp(name, HighPassFilter::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         return new HighPassFilter(deleteHighPassFilterObject);
         }

      return NULL;
      }
   }
