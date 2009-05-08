/**
 * The SetVolumeMenuItemAction class is a CharacterDisplayMenuItemAction for setting the current volume.
 *
 * Users and subclasses can override the default volume label ("Volume") and the default "Lo", "Hi", and "Off" labels by
 * constructing the instance with a {@link Map} containing keys "label.volume", "label.low", "label.high", and
 * "label.off".  The values for those keys will be used instead of the defaults. The messages displayed upon action
 * success or cancel can be similarly customized by specifying "action.performed" and "action.cancelled".  Finally, the
 * character used in the bar graph can be customized by setting the "graph.character" property.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef SETVOLUMEMENUITEMACTION_H_
#define SETVOLUMEMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <sstream>
#include <unistd.h>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"

using namespace std;

class SetVolumeMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;
      static const int MIN_VOLUME;
      static const int MAX_VOLUME;
      static const string DEFAULT_LABEL_VOLUME;
      static const string DEFAULT_LABEL_LOW;
      static const string DEFAULT_LABEL_HIGH;
      static const string DEFAULT_LABEL_OFF;
      static const string DEFAULT_VOLUME_GRAPH_CHARACTER;
      static const string DEFAULT_LABEL_ACTION_PERFORMED;
      static const string DEFAULT_LABEL_ACTION_CANCELLED;
      static const string PROPERTY_GRAPH_CHARACTER;
      static const string PROPERTY_LABEL_VOLUME;
      static const string PROPERTY_LABEL_HIGH;
      static const string PROPERTY_LABEL_LOW;
      static const string PROPERTY_LABEL_OFF;
      static const string PROPERTY_ACTION_PERFORMED;
      static const string PROPERTY_ACTION_CANCELLED;

      SetVolumeMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                              map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         volume = 5;
         tempVolume = 5;
         }

      virtual ~SetVolumeMenuItemAction()
         {
         // nothing to do
         }

      void activate();

      void start();

      void stop();

      void upEvent();

      void downEvent();

      void rightEvent();

      void leftEvent();

   private:

      int volume;
      int tempVolume;

      const string getActionPerformedText();

      const string getActionCancelledText();

      const int getCurrentVolume() const;

      void setCurrentVolume(const int newVolume);

      const string generateVolumeLine();

      const string generateVolumeGraphLine();

      void sleepThenPopUpToParentMenuItem();

      static const string convertIntToString(const int val);
   };

#endif /* SETVOLUMEMENUITEMACTION_H_ */
