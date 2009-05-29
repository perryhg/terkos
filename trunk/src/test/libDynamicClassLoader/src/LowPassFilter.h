#ifndef LOWPASSFILTER_H_
#define LOWPASSFILTER_H_

#include <algorithm>
#include <iostream>
#include <string.h>
#include "Filter.h"

using namespace std;

class LowPassFilter : public Filter
   {
   public:
      static const string CLASS_NAME;

      LowPassFilter(void(*delObj)(void*));
      virtual ~LowPassFilter(void);
      virtual void processAudio(int size, unsigned char* buffer);
   };

#endif /* LOWPASSFILTER_H_ */

