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

#ifndef POWERINFOMANAGER_H_
#define POWERINFOMANAGER_H_

#include <string.h>
#include <json/json.h>
#include <pstream.h>
#include <math.h>
#include <qepower.h>

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
         CQEPower::Release();
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

      int getHardwareProperty(const int property);
      
   };

#endif /* POWERINFOMANAGER_H_ */
