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
