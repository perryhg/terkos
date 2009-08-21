/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef RESETROOTUSERCONTROLPANELPASSWORDMENUITEMACTION_H_
#define RESETROOTUSERCONTROLPANELPASSWORDMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "RootUserPasswordManager.h"

class ResetRootUserControlPanelPasswordMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetRootUserControlPanelPasswordMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetRootUserControlPanelPasswordMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   };

#endif /* RESETROOTUSERCONTROLPANELPASSWORDMENUITEMACTION_H_ */
