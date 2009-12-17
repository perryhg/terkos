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

#include <iostream>
#include <string>
#include <AudioConfigManager.h>
#include <ServoConfigManager.h>
#include <WirelessNetworkingConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {

   AudioConfigManager audioConfigManager("./");

   cout << "Volume Level   = [" << audioConfigManager.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << audioConfigManager.areAlertsEnabled() << "]" << endl;

   audioConfigManager.setVolumeLevel(15);
   audioConfigManager.setAlertsEnabled(false);

   cout << "Volume Level   = [" << audioConfigManager.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << audioConfigManager.areAlertsEnabled() << "]" << endl;

   audioConfigManager.setVolumeLevel(7);
   audioConfigManager.setAlertsEnabled(true);

   cout << "Volume Level   = [" << audioConfigManager.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << audioConfigManager.areAlertsEnabled() << "]" << endl;

   cout << "-----------------------------------------------------------------" << endl;

   ServoConfigManager servoConfigManager("./");

   cout << "Number of servos: " << ServoConfigManager::NUM_SERVOS << endl;

   cout << "Reverting servo config to defaults..." << endl;
   servoConfigManager.revertToDefault();

   cout << "Servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < ServoConfigManager::NUM_SERVOS; i++)
      {
      cout << "Servo " << i << ":  min: " << servoConfigManager.getMinBound(i) << " max: " << servoConfigManager.getMaxBound(i) << " init: "
               << servoConfigManager.getInitialPosition(i) << endl;
      }

   cout << "Modifying servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < ServoConfigManager::NUM_SERVOS; i++)
      {
      servoConfigManager.setMinBound(i, i + 10);
      servoConfigManager.setMaxBound(i, i + 200);
      servoConfigManager.setInitialPosition(i, i + 100);
      }

   cout << "Servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < ServoConfigManager::NUM_SERVOS; i++)
      {
      cout << "Servo " << i << ":  min: " << servoConfigManager.getMinBound(i) << " max: " << servoConfigManager.getMaxBound(i) << " init: "
               << servoConfigManager.getInitialPosition(i) << endl;
      }

   unsigned int bogusServoId = ServoConfigManager::NUM_SERVOS + 10;
   cout << "Testing error handling for invalid servo ID [" << bogusServoId << "]..." << endl;
   servoConfigManager.getMinBound(bogusServoId);
   servoConfigManager.getMaxBound(bogusServoId);
   servoConfigManager.getInitialPosition(bogusServoId);
   servoConfigManager.setMinBound(bogusServoId, 127);
   servoConfigManager.setMaxBound(bogusServoId, 127);
   servoConfigManager.setInitialPosition(bogusServoId, 127);

   cout << "-----------------------------------------------------------------" << endl;

   WirelessNetworkingConfigManager wirelessNetworkingConfigManager("./");

   const bool willStartOnBootup = wirelessNetworkingConfigManager.willStartOnBootup();
   cout << "Wireless: Will Start on Boot = [" << willStartOnBootup << "]" << endl;

   wirelessNetworkingConfigManager.setWillStartOnBootup(!willStartOnBootup);

   cout << "Wireless: Will Start on Boot = [" << wirelessNetworkingConfigManager.willStartOnBootup() << "]" << endl;

   cout << "-----------------------------------------------------------------" << endl;

   return 0;
   }

