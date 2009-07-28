/**
 * The FirmwareVersionMenuItemAction class is a CharacterDisplayMenuItemAction for viewing the firmware version.
 *
 * Users and subclasses can override the default label ("Firmware version") by constructing the instance with a map
 * containing the key "label".  The value for that key will be used instead of the default.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef FIRMWAREVERSIONMENUITEMACTION_H_
#define FIRMWAREVERSIONMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <SystemInfoManager.h>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"

using namespace std;

class FirmwareVersionMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;
      static const string PROPERTY_LABEL;
      static const string DEFAULT_LABEL;

      FirmwareVersionMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                    CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~FirmwareVersionMenuItemAction()
         {
         // nothing to do
         }

      virtual void activate();

   private:

      SystemInfoManager systemInfoManager;

   };

#endif /* FIRMWAREVERSIONMENUITEMACTION_H_ */
