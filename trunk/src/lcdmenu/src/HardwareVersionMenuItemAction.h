/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef HARDWAREVERSIONMENUITEMACTION_H_
#define HARDWAREVERSIONMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"

using namespace std;

class HardwareVersionMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

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
   };

#endif /* HARDWAREVERSIONMENUITEMACTION_H_ */
