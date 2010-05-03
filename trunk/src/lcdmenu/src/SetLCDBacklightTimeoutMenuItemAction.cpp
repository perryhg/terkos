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

/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "SetLCDBacklightTimeoutMenuItemAction.h"

const string SetLCDBacklightTimeoutMenuItemAction::CLASS_NAME = "SetLCDBacklightTimeoutMenuItemAction";

const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_BACKLIGHT = "Backlight";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_OFF = "Off";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_ON = "On";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_SECONDS = "seconds";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_SECONDS_ABBREV = "s";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_ALWAYS_OFF = "Backlight set toalways off";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_ALWAYS_ON = "Backlight set toalways on";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_GRAPH_CHARACTER = "*";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_ACTION_PERFORMED = "Timeout set to  ";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_ACTION_CANCELLED = "Cancelled -- No settings changed";
const string SetLCDBacklightTimeoutMenuItemAction::DEFAULT_LABEL_CHANGE_TAKES_EFFECT_AT_REBOOT = "Change takes    effect at reboot";

const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_BACKLIGHT = "label.backlight";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_OFF = "label.off";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_ON = "label.on";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_SECONDS = "label.seconds";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_SECONDS_ABBREV = "label.seconds-abbreviation";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_ALWAYS_OFF = "label.always-off";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_ALWAYS_ON = "label.always-on";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_GRAPH_CHARACTER = "graph.character";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_ACTION_PERFORMED = "action.performed";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_ACTION_CANCELLED = "action.cancelled";
const string SetLCDBacklightTimeoutMenuItemAction::PROPERTY_LABEL_CHANGE_TAKES_EFFECT_AT_REBOOT = "label.change-takes-effect-at-reboot";

const int SetLCDBacklightTimeoutMenuItemAction::TIMEOUT_STEP_VALUE = 5;
const int SetLCDBacklightTimeoutMenuItemAction::MIN_GUI_TIMEOUT = LCDConfigManager::BACKLIGHT_TIMEOUT_MIN_VALUE;
const int SetLCDBacklightTimeoutMenuItemAction::MAX_GUI_TIMEOUT = LCDConfigManager::BACKLIGHT_TIMEOUT_MAX_VALUE + SetLCDBacklightTimeoutMenuItemAction::TIMEOUT_STEP_VALUE;

void SetLCDBacklightTimeoutMenuItemAction::activate()
   {
   timeout = getCurrentTimeout();
   if (timeout == LCDConfigManager::BACKLIGHT_TIMEOUT_ALWAYS_ON_VALUE)
      {
      timeout = MAX_GUI_TIMEOUT;
      }
   getCharacterDisplay()->setLine(0, generateTimeoutLine());
   getCharacterDisplay()->setLine(1, generateTimeoutGraphLine());
   }

void SetLCDBacklightTimeoutMenuItemAction::start()
   {
   if (timeout == MAX_GUI_TIMEOUT)
      {
      setCurrentTimeout(LCDConfigManager::BACKLIGHT_TIMEOUT_ALWAYS_ON_VALUE);
      }
   else
      {
      setCurrentTimeout(timeout);
      }
   getCharacterDisplay()->setText(getActionPerformedText1());
   usleep(1500000);
   getCharacterDisplay()->setText(getActionPerformedText2());
   sleepThenPopUpToParentMenuItem();
   }

void SetLCDBacklightTimeoutMenuItemAction::stop()
   {
   getCharacterDisplay()->setText(getActionCancelledText());
   sleepThenPopUpToParentMenuItem();
   }

void SetLCDBacklightTimeoutMenuItemAction::upEvent()
   {
   rightEvent();
   }

void SetLCDBacklightTimeoutMenuItemAction::downEvent()
   {
   leftEvent();
   }

void SetLCDBacklightTimeoutMenuItemAction::rightEvent()
   {
   timeout += TIMEOUT_STEP_VALUE;
   if (timeout > MAX_GUI_TIMEOUT)
      {
      timeout = MAX_GUI_TIMEOUT;
      }
   getCharacterDisplay()->setLine(0, generateTimeoutLine());
   getCharacterDisplay()->setLine(1, generateTimeoutGraphLine());
   }

void SetLCDBacklightTimeoutMenuItemAction::leftEvent()
   {
   timeout -= TIMEOUT_STEP_VALUE;
   if (timeout < MIN_GUI_TIMEOUT)
      {
      timeout = MIN_GUI_TIMEOUT;
      }
   getCharacterDisplay()->setLine(0, generateTimeoutLine());
   getCharacterDisplay()->setLine(1, generateTimeoutGraphLine());
   }

const string SetLCDBacklightTimeoutMenuItemAction::getActionPerformedText1()
   {
   int theTimeout = getCurrentTimeout();
   if (theTimeout == LCDConfigManager::BACKLIGHT_TIMEOUT_ALWAYS_OFF_VALUE)
      {
      return getProperty(PROPERTY_LABEL_ALWAYS_OFF, DEFAULT_LABEL_ALWAYS_OFF);
      }
   else if (theTimeout == LCDConfigManager::BACKLIGHT_TIMEOUT_ALWAYS_ON_VALUE)
      {
      return getProperty(PROPERTY_LABEL_ALWAYS_ON, DEFAULT_LABEL_ALWAYS_ON);
      }

   return getProperty(PROPERTY_ACTION_PERFORMED, DEFAULT_LABEL_ACTION_PERFORMED) +
          StringUtilities::convertIntToString(getCurrentTimeout()) +
          " " +
          getProperty(PROPERTY_LABEL_SECONDS, DEFAULT_LABEL_SECONDS);
   }
/*
0123456789012345
Backlight: On
Backlight: Off
Backlight: 3 s
always on
*/
const string SetLCDBacklightTimeoutMenuItemAction::getActionPerformedText2()
   {
   return getProperty(PROPERTY_LABEL_CHANGE_TAKES_EFFECT_AT_REBOOT, DEFAULT_LABEL_CHANGE_TAKES_EFFECT_AT_REBOOT);
   }

const string SetLCDBacklightTimeoutMenuItemAction::getActionCancelledText()
   {
   return getProperty(PROPERTY_ACTION_CANCELLED, DEFAULT_LABEL_ACTION_CANCELLED);
   }

const int SetLCDBacklightTimeoutMenuItemAction::getCurrentTimeout() const
   {
   return configManager.getBacklightTimeout();
   }

void SetLCDBacklightTimeoutMenuItemAction::setCurrentTimeout(const int newTimeout)
   {
   configManager.setBacklightTimeout(newTimeout);
   }

const string SetLCDBacklightTimeoutMenuItemAction::generateTimeoutLine()
   {
   string timeoutLabel = getProperty(PROPERTY_LABEL_BACKLIGHT, DEFAULT_LABEL_BACKLIGHT) + ": ";

   if (timeout == MIN_GUI_TIMEOUT)
      {
      timeoutLabel.append(getProperty(PROPERTY_LABEL_OFF, DEFAULT_LABEL_OFF));
      }
   else if (timeout == MAX_GUI_TIMEOUT)
      {
      timeoutLabel.append(getProperty(PROPERTY_LABEL_ON, DEFAULT_LABEL_ON));
      }
   else
      {
      timeoutLabel.append(StringUtilities::convertIntToString(timeout));
      timeoutLabel.append(" ");
      timeoutLabel.append(getProperty(PROPERTY_LABEL_SECONDS_ABBREV, DEFAULT_LABEL_SECONDS_ABBREV));
      }

   return timeoutLabel;
   }

const string SetLCDBacklightTimeoutMenuItemAction::generateTimeoutGraphLine()
   {
   const string graphCharacter = getProperty(PROPERTY_GRAPH_CHARACTER, DEFAULT_GRAPH_CHARACTER);

   string s;
   for (int i = 0; i < timeout; i += TIMEOUT_STEP_VALUE)
      {
      s += graphCharacter;
      }
   for (int i = timeout; i < MAX_GUI_TIMEOUT; i += TIMEOUT_STEP_VALUE)
      {
      s += " ";
      }
   return getProperty(PROPERTY_LABEL_OFF, DEFAULT_LABEL_OFF) + " " + s + " " + getProperty(PROPERTY_LABEL_ON, DEFAULT_LABEL_ON);
   }

void SetLCDBacklightTimeoutMenuItemAction::sleepThenPopUpToParentMenuItem()
   {
   usleep(1500000);
   CharacterDisplayMenuItemAction::stop();
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteSetLCDBacklightTimeoutMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadSetLCDBacklightTimeoutMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(SetLCDBacklightTimeoutMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, SetLCDBacklightTimeoutMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new SetLCDBacklightTimeoutMenuItemAction(deleteSetLCDBacklightTimeoutMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
