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
