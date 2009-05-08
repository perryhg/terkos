/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef TOGGLEAUDIOALERTSMENUITEMACTION_H_
#define TOGGLEAUDIOALERTSMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"

class ToggleAudioAlertsMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ToggleAudioAlertsMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                      CharacterDisplay* characterDisplay, map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         isAlertsEnabled = false;
         }

      virtual ~ToggleAudioAlertsMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   private:

      bool isAlertsEnabled;

      void setAlertsEnabled(const bool isAlertsEnabled);
   };

#endif /* TOGGLEAUDIOALERTSMENUITEMACTION_H_ */
