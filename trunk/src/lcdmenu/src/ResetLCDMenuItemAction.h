/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef RESETLCDMENUITEMACTION_H_
#define RESETLCDMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "LCDConfigManager.h"

class ResetLCDMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetLCDMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetLCDMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   };

#endif /* RESETLCDMENUITEMACTION_H_ */
