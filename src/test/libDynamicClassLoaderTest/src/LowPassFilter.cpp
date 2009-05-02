#include "LowPassFilter.h"

const string LowPassFilter::CLASS_NAME = "LowPassFilter";

LowPassFilter::LowPassFilter(void(*delObj)(void*)) :
   Filter(delObj)
   {
   }

LowPassFilter::~LowPassFilter(void)
   {
   }

void LowPassFilter::processAudio(int size, unsigned char* buffer)
   {
   // apply a low pass filter to the buffer
   std::cout << "Hello from the low pass filter" << std::endl;
   }

// The Dynamic library should also contain the following external C function definitions
extern "C"
   {
   void deleteObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(LowPassFilter::CLASS_NAME.c_str()));
      if (strncmp(name, LowPassFilter::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         return new LowPassFilter(deleteObject);
         }

      return NULL;
      }
   }
