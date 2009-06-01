#include <iostream>
#include <string>
#include <AudioConfigManager.h>
#include <ServoConfigManager.h>
#include <WirelessNetworkingConfigManager.h>

using namespace std;

int main(int argc, char** argv)
   {

   AudioConfigManager audioConfigManager;

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

   ServoConfigManager servoConfigManager;

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

   WirelessNetworkingConfigManager wirelessNetworkingConfigManager;

   const bool wirelessIsEnabled = wirelessNetworkingConfigManager.isEnabled();
   const bool wirelessWillConnectUsingSpecificProfile = wirelessNetworkingConfigManager.willConnectUsingSpecificProfile();
   cout << "Wireless: Is Enabled                          = [" << wirelessIsEnabled << "]" << endl;
   cout << "Wireless: Will Connect Using Specific Profile = [" << wirelessWillConnectUsingSpecificProfile << "]" << endl;

   wirelessNetworkingConfigManager.setEnabled(!wirelessIsEnabled);
   wirelessNetworkingConfigManager.setWillConnectUsingSpecificProfile(!wirelessWillConnectUsingSpecificProfile);

   cout << "Wireless: Is Enabled                          = [" << wirelessNetworkingConfigManager.isEnabled() << "]" << endl;
   cout << "Wireless: Will Connect Using Specific Profile = [" << wirelessNetworkingConfigManager.willConnectUsingSpecificProfile() << "]" << endl;

   cout << "-----------------------------------------------------------------" << endl;

   return 0;
   }

