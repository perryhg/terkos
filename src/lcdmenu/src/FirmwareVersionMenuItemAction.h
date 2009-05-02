/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef FIRMWAREVERSIONMENUITEMACTION_H_
#define FIRMWAREVERSIONMENUITEMACTION_H_

#include <algorithm>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"

using namespace std;

class FirmwareVersionMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      FirmwareVersionMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                    CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // TODO: Remove this debug info
         printf("FirmwareVersionMenuItemAction::FirmwareVersionMenuItemAction()\n");
         printf("argv[0]=%p\n", menuItem);
         printf("argv[1]=%p\n", menuStatusManager);
         printf("argv[2]=%p\n", characterDisplay);
         printf("argv[3]=%p\n", &properties);

         // nothing to do
         }

      virtual ~FirmwareVersionMenuItemAction()
         {
         // nothing to do
         }

      virtual void activate();
   };

#endif /* FIRMWAREVERSIONMENUITEMACTION_H_ */
