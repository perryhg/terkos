/**
 * The HardwareVersionMenuItemAction class is a CharacterDisplayMenuItemAction for viewing the hardware version.
 *
 * Users and subclasses can override the default label ("Hardware version") by constructing the instance with a map
 * containing the key "label".  The value for that key will be used instead of the default.
 *
 * Chris Bartley (bartley@cmu.edu)
 */


#ifndef HARDWAREVERSIONMENUITEMACTION_H_
#define HARDWAREVERSIONMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <VersionInfoManager.h>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"

using namespace std;

class HardwareVersionMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;
      static const string PROPERTY_LABEL;
      static const string DEFAULT_LABEL;

      HardwareVersionMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                    CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~HardwareVersionMenuItemAction()
         {
         // nothing to do
         }

      virtual void activate();

   private:

      VersionInfoManager versionInfoManager;

   };

#endif /* HARDWAREVERSIONMENUITEMACTION_H_ */
