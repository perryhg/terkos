/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef RESETALLMENUITEMACTION_H_
#define RESETALLMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "AudioConfigManager.h"
#include "WirelessNetworkingConfigManager.h"
#include "UserProgramsConfigManager.h"
#include "WebControlPanelPasswordManager.h"

class ResetAllMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetAllMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetAllMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   };

#endif /* RESETALLMENUITEMACTION_H_ */
