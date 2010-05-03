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
 * The SetLCDBacklightTimeoutMenuItemAction class is a CharacterDisplayMenuItemAction for setting the LCD backlight
 * timeout (in seconds).
 *
 * Users and subclasses can override the default label ("Timeout (s)") and the default "Off" and "On" labels by
 * constructing the instance with a map containing keys "label.timeout", "label.off", and "label.on".  The values for
 * those keys will be used instead of the defaults. The messages displayed upon action success or cancel can be
 * similarly customized by specifying "action.performed", "action.cancelled", "label.seconds",
 * "label.seconds-abbreviation", "label.always-off", "label.always-on", and "label.change-takes-effect-at-reboot".
 * Finally, the character used in the bar graph can be customized by setting the "graph.character" property.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef SETLCDBACKLIGHTTIMEOUTMENUITEMACTION_H_
#define SETLCDBACKLIGHTTIMEOUTMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"
#include "LCDConfigManager.h"
#include "StringUtilities.h"

class SetLCDBacklightTimeoutMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string DEFAULT_LABEL_BACKLIGHT;
      static const string DEFAULT_LABEL_OFF;
      static const string DEFAULT_LABEL_ON;
      static const string DEFAULT_LABEL_SECONDS;
      static const string DEFAULT_LABEL_SECONDS_ABBREV;
      static const string DEFAULT_LABEL_ALWAYS_OFF;
      static const string DEFAULT_LABEL_ALWAYS_ON;
      static const string DEFAULT_GRAPH_CHARACTER;
      static const string DEFAULT_LABEL_ACTION_PERFORMED;
      static const string DEFAULT_LABEL_ACTION_CANCELLED;
      static const string DEFAULT_LABEL_CHANGE_TAKES_EFFECT_AT_REBOOT;

      static const string PROPERTY_LABEL_BACKLIGHT;
      static const string PROPERTY_LABEL_OFF;
      static const string PROPERTY_LABEL_ON;
      static const string PROPERTY_LABEL_SECONDS;
      static const string PROPERTY_LABEL_SECONDS_ABBREV;
      static const string PROPERTY_LABEL_ALWAYS_OFF;
      static const string PROPERTY_LABEL_ALWAYS_ON;
      static const string PROPERTY_GRAPH_CHARACTER;
      static const string PROPERTY_ACTION_PERFORMED;
      static const string PROPERTY_ACTION_CANCELLED;
      static const string PROPERTY_LABEL_CHANGE_TAKES_EFFECT_AT_REBOOT;

      SetLCDBacklightTimeoutMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                      CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~SetLCDBacklightTimeoutMenuItemAction()
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

      static const int TIMEOUT_STEP_VALUE;
      static const int MIN_GUI_TIMEOUT;
      static const int MAX_GUI_TIMEOUT;

      LCDConfigManager configManager;
      int timeout;

      const string getActionPerformedText1();
      const string getActionPerformedText2();
      const string getActionCancelledText();
      const int getCurrentTimeout() const;
      void setCurrentTimeout(const int newTimeout);
      const string generateTimeoutLine();
      const string generateTimeoutGraphLine();
      void sleepThenPopUpToParentMenuItem();
   };

#endif /* SETLCDBACKLIGHTTIMEOUTMENUITEMACTION_H_ */
