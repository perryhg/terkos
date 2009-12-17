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

#ifndef POWERSTATUSMENUITEMACTION_H_
#define POWERSTATUSMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include "CharacterDisplayMenuItemAction.h"
#include "PowerInfoManager.h"
#include "StringUtilities.h"

class PowerStatusMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      PowerStatusMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                            CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties), numColumnsWhenInScrollingMode(characterDisplay->getColumnsWhenInScrollingMode())
         {
         // nothing to do
         }

      virtual ~PowerStatusMenuItemAction()
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

      static const unsigned int NUM_NONBREAKER_COMPONENTS;
      static const unsigned int NUM_COMPONENTS;

      static const string VOLTAGE_PROPERTY;
      static const string STATE_PROPERTY;
      static const string MAIN_BATTERY_PROPERTY;
      static const string BACKUP_BATTERY_PROPERTY;
      static const string IO_5_VOLT_PROPERTY;
      static const string MAIN_5_VOLT_PROPERTY;
      static const string BREAKER_PROPERTY;
      static const string BREAKER_OK_PROPERTY;
      static const string BREAKER_FAULT_PROPERTY;

      static const string VOLTAGE_DEFAULT_LABEL;
      static const string STATE_DEFAULT_LABEL;
      static const string MAIN_BATTERY_DEFAULT_LABEL;
      static const string BACKUP_BATTERY_DEFAULT_LABEL;
      static const string IO_5_VOLT_DEFAULT_LABEL;
      static const string MAIN_5_VOLT_DEFAULT_LABEL;
      static const string BREAKER_DEFAULT_LABEL;
      static const string BREAKER_OK_DEFAULT_LABEL;
      static const string BREAKER_FAULT_DEFAULT_LABEL;

      PowerInfoManager powerInfoManager;

      const unsigned int numColumnsWhenInScrollingMode;

      unsigned int currentComponentId;

      void displayComponentDetails();
      void displayComponentDetailsWorkhorse(const string& line1, const string& line2, const string& value);

   };

#endif /* POWERSTATUSMENUITEMACTION_H_ */
