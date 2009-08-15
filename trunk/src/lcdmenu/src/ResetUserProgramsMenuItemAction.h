/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef RESETUSERPROGRAMSMENUITEMACTION_H_
#define RESETUSERPROGRAMSMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "UserProgramsConfigManager.h"

class ResetUserProgramsMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetUserProgramsMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetUserProgramsMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   };

#endif /* RESETUSERPROGRAMSMENUITEMACTION_H_ */
