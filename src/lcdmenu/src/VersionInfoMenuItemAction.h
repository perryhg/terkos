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

#ifndef VERSIONINFOMENUITEMACTION_H_
#define VERSIONINFOMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <json/json.h>
#include "CharacterDisplayMenuItemAction.h"
#include "VersionInfoManager.h"
#include "StringUtilities.h"

class VersionInfoMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      VersionInfoMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                                map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         VersionInfoManager versionInfoManager;

         unsigned int numColumnsWhenInScrollingMode = characterDisplay->getColumnsWhenInScrollingMode();

         string screen0Label = getProperty(OPERATING_SYSTEM_VERSION_PROPERTY, OPERATING_SYSTEM_VERSION_DEFAULT_LABEL);
         screens.push_back(StringUtilities::rightPadOrTrimRight(screen0Label + ":", numColumnsWhenInScrollingMode)
                  + versionInfoManager.getOperatingSystemName() + " " + versionInfoManager.getOperatingSystemRelease());

         string screen1Label = getProperty(FIRMWARE_VERSION_PROPERTY, FIRMWARE_VERSION_DEFAULT_LABEL);
         screens.push_back(StringUtilities::rightPadOrTrimRight(screen1Label + ":", numColumnsWhenInScrollingMode)
                  + versionInfoManager.getFirmwareVersion());

         string screen2Label = getProperty(HARDWARE_VERSION_PROPERTY, HARDWARE_VERSION_DEFAULT_LABEL);
         screens.push_back(StringUtilities::rightPadOrTrimRight(screen2Label + ":", numColumnsWhenInScrollingMode)
                  + versionInfoManager.getHardwareVersion());
         }

      virtual ~VersionInfoMenuItemAction()
         {
         // nothing to do
         }

      void activate();
      void start();
      void stop();
      void upEvent();
      void downEvent();
      void rightEvent();
      void leftEvent();

   private:

      static const string UNKNOWN_VALUE;

      static const string OPERATING_SYSTEM_VERSION_PROPERTY;
      static const string FIRMWARE_VERSION_PROPERTY;
      static const string HARDWARE_VERSION_PROPERTY;

      static const string OPERATING_SYSTEM_VERSION_DEFAULT_LABEL;
      static const string FIRMWARE_VERSION_DEFAULT_LABEL;
      static const string HARDWARE_VERSION_DEFAULT_LABEL;

      vector<string> screens;

      unsigned int currentScreenId;

      void displayCurrentScreen();

   };

#endif /* VERSIONINFOMENUITEMACTION_H_ */
