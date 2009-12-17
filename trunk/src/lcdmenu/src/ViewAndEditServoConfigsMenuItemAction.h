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

#ifndef VIEWANDEDITSERVOCONFIGSMENUITEMACTION_H_
#define VIEWANDEDITSERVOCONFIGSMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include "CharacterDisplayMenuItemAction.h"
#include "ServoConfigManager.h"
#include "StringUtilities.h"

class ViewAndEditServoConfigsMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ViewAndEditServoConfigsMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                            CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         currentServoId = 0;
         isDisplayMode = true;
         }

      virtual ~ViewAndEditServoConfigsMenuItemAction()
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

      ServoConfigManager configManager;
      unsigned int currentServoId;
      bool isDisplayMode;

      void displayServoConfigDetails();

   };

#endif /* VIEWANDEDITSERVOCONFIGSMENUITEMACTION_H_ */
