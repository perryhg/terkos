/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef POWERINFOMANAGER_H_
#define POWERINFOMANAGER_H_

#include <string.h>
#include <json/json.h>
#include <pstream.h>
#include <math.h>
#include "qwerkhw.h"

using namespace std;
using namespace redi;

class PowerInfoManager
   {
   public:

      static const unsigned int NUM_BREAKERS;

      PowerInfoManager()
         {
         // nothing to do
         }

      virtual ~PowerInfoManager()
         {
         // nothing to do
         }

      const int getMainBatteryVoltage();
      const int getBackupBatteryVoltage();
      const int getIO5VVoltage();
      const int getMain5VVoltage();

      const int getMainBatteryState();
      const int getBackupBatteryState();
      const int getIO5VState();

      /** Returns true if the breaker is OK, false if it's been tripped or the breakerId is invalid. */
      const bool getBreakerState(const unsigned int breakerId);

      Json::Value getJSON();

   private:

      CQwerkHardware hardware;

      int getHardwareProperty(const int property);
   };

#endif /* POWERINFOMANAGER_H_ */
