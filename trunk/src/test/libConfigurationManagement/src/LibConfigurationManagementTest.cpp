#include <iostream>
#include <string>
#include <libconfig.h++>
#include "AudioConfigManager.h"
#include "ServoConfigManager.h"

using namespace std;
using namespace libconfig;

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

   cout << "Reverting servo config to defaults..." << endl;
   servoConfigManager.revertToDefault();

   cout << "Servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < 16; i++)
      {
      cout << "Servo " << i << ":  min: " << servoConfigManager.getMinBound(i) << " max: " << servoConfigManager.getMaxBound(i) << " init: "
               << servoConfigManager.getInitialPosition(i) << endl;
      }

   cout << "Modifying servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < 16; i++)
      {
      servoConfigManager.setMinBound(i, i + 10);
      servoConfigManager.setMaxBound(i, i + 200);
      servoConfigManager.setInitialPosition(i, i + 100);
      }

   cout << "Servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < 16; i++)
      {
      cout << "Servo " << i << ":  min: " << servoConfigManager.getMinBound(i) << " max: " << servoConfigManager.getMaxBound(i) << " init: "
               << servoConfigManager.getInitialPosition(i) << endl;
      }

   return 0;

   }
