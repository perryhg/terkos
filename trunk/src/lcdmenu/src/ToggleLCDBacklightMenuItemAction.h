/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef TOGGLELCDBACKLIGHTMENUITEMACTION_H_
#define TOGGLELCDBACKLIGHTMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "LCDConfigManager.h"

class ToggleLCDBacklightMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ToggleLCDBacklightMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                      CharacterDisplay* characterDisplay, map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ToggleLCDBacklightMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   private:

      LCDConfigManager configManager;
      bool isBacklightEnabled;

      void setBacklightEnabled(const bool isEnabled);
   };

#endif /* TOGGLELCDBACKLIGHTMENUITEMACTION_H_ */
