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

