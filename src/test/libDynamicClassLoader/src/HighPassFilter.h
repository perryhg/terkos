#ifndef HIGHPASSFILTER_H_
#define HIGHPASSFILTER_H_

#include <algorithm>
#include <iostream>
#include <string.h>
#include "Filter.h"

using namespace std;

class HighPassFilter : public Filter
   {
   public:
      static const string CLASS_NAME;

      HighPassFilter(void(*delObj)(void*));
      virtual ~HighPassFilter(void);
      virtual void processAudio(int size, unsigned char* buffer);
   };

#endif /* HIGHPASSFILTER_H_ */

