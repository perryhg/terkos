/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef RESETWEBCONTROLPANELPASSWORDMENUITEMACTION_H_
#define RESETWEBCONTROLPANELPASSWORDMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "WebControlPanelPasswordManager.h"

class ResetWebControlPanelPasswordMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetWebControlPanelPasswordMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetWebControlPanelPasswordMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   };

#endif /* RESETWEBCONTROLPANELPASSWORDMENUITEMACTION_H_ */
