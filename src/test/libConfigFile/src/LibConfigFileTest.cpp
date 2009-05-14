#include <iostream>
#include <string>
#include <libconfig.h++>
#include "AudioConfig.h"
#include "ServoConfig.h"

using namespace std;
using namespace libconfig;

int main(int argc, char** argv)
   {

   AudioConfig audioConfig;

   cout << "Volume Level   = [" << audioConfig.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << audioConfig.areAlertsEnabled() << "]" << endl;

   audioConfig.setVolumeLevel(15);
   audioConfig.setAlertsEnabled(false);

   cout << "Volume Level   = [" << audioConfig.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << audioConfig.areAlertsEnabled() << "]" << endl;

   audioConfig.setVolumeLevel(7);
   audioConfig.setAlertsEnabled(true);

   cout << "Volume Level   = [" << audioConfig.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << audioConfig.areAlertsEnabled() << "]" << endl;

   cout << "-----------------------------------------------------------------" << endl;

   ServoConfig servoConfig;

   cout << "Reverting servo config to defaults..." << endl;
   servoConfig.revertToDefault();

   cout << "Servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < 16; i++)
      {
      cout << "Servo " << i << ":  min: " << servoConfig.getMinBound(i) << " max: " << servoConfig.getMaxBound(i) << " init: "
               << servoConfig.getInitialPosition(i) << endl;
      }

   cout << "Modifying servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < 16; i++)
      {
      servoConfig.setMinBound(i, i + 10);
      servoConfig.setMaxBound(i, i + 200);
      servoConfig.setInitialPosition(i, i + 100);
      }

   cout << "Servo min/max bounds and initial positions..." << endl;
   for (unsigned int i = 0; i < 16; i++)
      {
      cout << "Servo " << i << ":  min: " << servoConfig.getMinBound(i) << " max: " << servoConfig.getMaxBound(i) << " init: "
               << servoConfig.getInitialPosition(i) << endl;
      }

   return 0;

   }
