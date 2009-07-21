/*
 * Simple program enabling access to the AudioConfigManager so that
 * the audio configuration can be set via the command line.
 *
 * Allowed arguments:
 *
 *    -volume INTEGER
 *    -alerts BOOLEAN
 *
 * The INTEGER value will be adjusted if necessary to fall within the
 * bounds of [AudioConfigManager::MIN_VOLUME, AudioConfigManager::MAX_VOLUME].
 * The BOOLEAN value must be either "true" or "false".
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <AudioConfigManager.h>
#include <StringUtilities.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      // parse the command line arguments
      bool isAlertModeSpecified = false;
      bool isVolumeSpecified = false;
      unsigned int volume = AudioConfigManager::MIN_VOLUME;
      bool isAlertsEnabled = false;
      int i = 1;
      while (i < argc)
         {
         string arg = argv[i];

         if (arg == "-volume")
            {
            // make sure there's another argument following the switch
            if (++i < argc)
               {
               arg = argv[i];
               int vol = atoi(argv[i]);

               // convert the arg back to a string to make sure atoi didn't simply fail
               string volStr = StringUtilities::convertIntToString(vol);
               if (volStr == arg)
                  {
                  isVolumeSpecified = true;
                  volume = (unsigned int) (std::min(std::max(vol, (int) AudioConfigManager::MIN_VOLUME), (int) AudioConfigManager::MAX_VOLUME));
                  }
               }
            else
               {
               break;
               }
            }
         else if (arg == "-alerts")
            {
            // make sure there's another argument following the switch
            if (++i < argc)
               {
               arg = argv[i];
               if (arg == "true")
                  {
                  isAlertModeSpecified = true;
                  isAlertsEnabled = true;
                  }
               else if (arg == "false")
                  {
                  isAlertModeSpecified = true;
                  isAlertsEnabled = false;
                  }
               }
            }

         i++;
         }

      if (isVolumeSpecified || isAlertModeSpecified)
         {
         AudioConfigManager configManager;
         if (isVolumeSpecified)
            {
            configManager.setVolumeLevel(volume);
            }
         if (isAlertModeSpecified)
            {
            configManager.setAlertsEnabled(isAlertsEnabled);
            }
         }
      }

   return 0;
   }
