//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

/**
 * The SetVolumeMenuItemAction class is a CharacterDisplayMenuItemAction for setting the current volume.
 *
 * Users and subclasses can override the default volume label ("Volume") and the default "Lo", "Hi", and "Off" labels by
 * constructing the instance with a map containing keys "label.volume", "label.low", "label.high", and
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
#include <unistd.h>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"
#include "AudioConfigManager.h"
#include "StringUtilities.h"

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
         // nothing to do
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

      AudioConfigManager configManager;
      int volume;

      const string getActionPerformedText();
      const string getActionCancelledText();
      const int getCurrentVolume() const;
      void setCurrentVolume(const int newVolume);
      const string generateVolumeLine();
      const string generateVolumeGraphLine();
      void sleepThenPopUpToParentMenuItem();
   };

#endif /* SETVOLUMEMENUITEMACTION_H_ */
