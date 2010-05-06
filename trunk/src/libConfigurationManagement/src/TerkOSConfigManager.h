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

#ifndef TERKOSCONFIGMANAGER_H_
#define TERKOSCONFIGMANAGER_H_

#include <vector>
#include <json/json.h>
#include <StringUtilities.h>
#include "AudioConfigManager.h"
#include "UserProgramsConfigManager.h"
#include "FirmwareVersionManager.h"
#include "LCDConfigManager.h"
#include "VersionInfoManager.h"
#include "WirelessNetworkingConfigManager.h"

using namespace std;

class TerkOSConfigManager
   {
   public:

      TerkOSConfigManager()
         {
         // nothing to do
         }

      virtual ~TerkOSConfigManager()
         {
         // nothing to do
         }

      Json::Value getJSON();
      
      Json::Value setJSON(Json::Value& config);
   };

#endif /* TERKOSCONFIGMANAGER_H_ */
