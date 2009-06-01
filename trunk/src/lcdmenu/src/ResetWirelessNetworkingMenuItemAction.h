/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef RESETWIRELESSNETWORKINGMENUITEMACTION_H_
#define RESETWIRELESSNETWORKINGMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "WirelessNetworkingConfigManager.h"

class ResetWirelessNetworkingMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetWirelessNetworkingMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetWirelessNetworkingMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   private:

      WirelessNetworkingConfigManager configManager;

   };

#endif /* RESETWIRELESSNETWORKINGMENUITEMACTION_H_ */
