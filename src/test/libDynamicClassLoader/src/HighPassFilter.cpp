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
