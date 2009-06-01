/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef TOGGLEWIRELESSSTARTUPUPONBOOTMENUITEMACTION_H_
#define TOGGLEWIRELESSSTARTUPUPONBOOTMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "WirelessNetworkingConfigManager.h"

class ToggleWirelessStartupUponBootMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ToggleWirelessStartupUponBootMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                      CharacterDisplay* characterDisplay, map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ToggleWirelessStartupUponBootMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   private:

      WirelessNetworkingConfigManager configManager;
      bool isEnabled;

      void setEnabled(const bool isEnabled);
   };

#endif /* TOGGLEWIRELESSSTARTUPUPONBOOTMENUITEMACTION_H_ */
