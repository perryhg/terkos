#include <iostream>
#include <libconfig.h++>
#include "AudioConfig.h"

using namespace std;
using namespace libconfig;

int main(int argc, char** argv)
   {
   AudioConfig cfg;

   cout << "Volume Level   = [" << cfg.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << cfg.areAlertsEnabled() << "]" << endl;

   cfg.setVolumeLevel(15);
   cfg.setAlertsEnabled(false);

   cout << "Volume Level   = [" << cfg.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << cfg.areAlertsEnabled() << "]" << endl;

   cfg.setVolumeLevel(7);
   cfg.setAlertsEnabled(true);

   cout << "Volume Level   = [" << cfg.getVolumeLevel() << "]" << endl;
   cout << "Alerts Enabled = [" << cfg.areAlertsEnabled() << "]" << endl;

   return 0;
   }
