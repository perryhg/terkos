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

/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef MEMORYUSAGEMANAGER_H_
#define MEMORYUSAGEMANAGER_H_

#include <string.h>
#include <json/json.h>
#include <pstream.h>

using namespace std;
using namespace redi;

class MemoryUsageManager
   {
   public:

      MemoryUsageManager()
         {
         // nothing to do
         }

      virtual ~MemoryUsageManager()
         {
         // nothing to do
         }

      Json::Value getJSON();

   };

#endif /* MEMORYUSAGEMANAGER_H_ */
