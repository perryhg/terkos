/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef RESETAUDIOMENUITEMACTION_H_
#define RESETAUDIOMENUITEMACTION_H_

#include "TwoOptionMenuItemAction.h"
#include "AudioConfig.h"

class ResetAudioMenuItemAction : public TwoOptionMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ResetAudioMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                               map<string, string>& properties) :
         TwoOptionMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~ResetAudioMenuItemAction()
         {
         // nothing to do
         }

   protected:

      bool shouldOption1BeSelectedUponActivation() const;

      void executeOption1Action();

      void executeOption2Action();

   private:

      AudioConfig config;
      bool areAlertsEnabled;

      void setAlertsEnabled(const bool isAlertsEnabled);
   };

#endif /* RESETAUDIOMENUITEMACTION_H_ */
