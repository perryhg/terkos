/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "SetVolumeMenuItemAction.h"

const string SetVolumeMenuItemAction::CLASS_NAME = "SetVolumeMenuItemAction";
const int SetVolumeMenuItemAction::MIN_VOLUME = AudioConfigManager::MIN_VOLUME;
const int SetVolumeMenuItemAction::MAX_VOLUME = AudioConfigManager::MAX_VOLUME;
const string SetVolumeMenuItemAction::DEFAULT_LABEL_VOLUME = "Volume";
const string SetVolumeMenuItemAction::DEFAULT_LABEL_LOW = "Lo";
const string SetVolumeMenuItemAction::DEFAULT_LABEL_HIGH = "Hi";
const string SetVolumeMenuItemAction::DEFAULT_LABEL_OFF = "Off";
const string SetVolumeMenuItemAction::DEFAULT_VOLUME_GRAPH_CHARACTER = "*";
const string SetVolumeMenuItemAction::DEFAULT_LABEL_ACTION_PERFORMED = "Volume set to ";
const string SetVolumeMenuItemAction::DEFAULT_LABEL_ACTION_CANCELLED = "Cancelled!";
const string SetVolumeMenuItemAction::PROPERTY_GRAPH_CHARACTER = "graph.character";
const string SetVolumeMenuItemAction::PROPERTY_LABEL_VOLUME = "label.volume";
const string SetVolumeMenuItemAction::PROPERTY_LABEL_HIGH = "label.high";
const string SetVolumeMenuItemAction::PROPERTY_LABEL_LOW = "label.low";
const string SetVolumeMenuItemAction::PROPERTY_LABEL_OFF = "label.off";
const string SetVolumeMenuItemAction::PROPERTY_ACTION_PERFORMED = "action.performed";
const string SetVolumeMenuItemAction::PROPERTY_ACTION_CANCELLED = "action.cancelled";

void SetVolumeMenuItemAction::activate()
   {
   volume = getCurrentVolume();
   getCharacterDisplay()->setLine(0, generateVolumeLine());
   getCharacterDisplay()->setLine(1, generateVolumeGraphLine());
   }

void SetVolumeMenuItemAction::start()
   {
   setCurrentVolume(volume);
   getCharacterDisplay()->setText(getActionPerformedText());
   sleepThenPopUpToParentMenuItem();
   }

void SetVolumeMenuItemAction::stop()
   {
   getCharacterDisplay()->setText(getActionCancelledText());
   sleepThenPopUpToParentMenuItem();
   }

void SetVolumeMenuItemAction::upEvent()
   {
   rightEvent();
   }

void SetVolumeMenuItemAction::downEvent()
   {
   leftEvent();
   }

void SetVolumeMenuItemAction::rightEvent()
   {
   volume++;
   if (volume > MAX_VOLUME)
      {
      volume = MAX_VOLUME;
      }
   getCharacterDisplay()->setLine(0, generateVolumeLine());
   getCharacterDisplay()->setLine(1, generateVolumeGraphLine());
   }

void SetVolumeMenuItemAction::leftEvent()
   {
   volume--;
   if (volume < MIN_VOLUME)
      {
      volume = MIN_VOLUME;
      }
   getCharacterDisplay()->setLine(0, generateVolumeLine());
   getCharacterDisplay()->setLine(1, generateVolumeGraphLine());
   }

const string SetVolumeMenuItemAction::getActionPerformedText()
   {
   return getProperty(PROPERTY_ACTION_PERFORMED, DEFAULT_LABEL_ACTION_PERFORMED) + SetVolumeMenuItemAction::convertIntToString(getCurrentVolume());
   }

const string SetVolumeMenuItemAction::getActionCancelledText()
   {
   return getProperty(PROPERTY_ACTION_CANCELLED, DEFAULT_LABEL_ACTION_CANCELLED);
   }

const int SetVolumeMenuItemAction::getCurrentVolume() const
   {
   return configManager.getVolumeLevel();
   }

void SetVolumeMenuItemAction::setCurrentVolume(const int newVolume)
   {
   configManager.setVolumeLevel(newVolume);
   }

const string SetVolumeMenuItemAction::generateVolumeLine()
   {
   const string volumeLabel = getProperty(PROPERTY_LABEL_VOLUME, DEFAULT_LABEL_VOLUME);
   const string volumeValue =
            (volume == 0 ? getProperty(PROPERTY_LABEL_OFF, DEFAULT_LABEL_OFF) : SetVolumeMenuItemAction::convertIntToString(volume));
   return volumeLabel + ": " + volumeValue;
   }

const string SetVolumeMenuItemAction::generateVolumeGraphLine()
   {
   const string graphCharacter = getProperty(PROPERTY_GRAPH_CHARACTER, DEFAULT_VOLUME_GRAPH_CHARACTER);

   string s;
   for (int i = 0; i < volume; i++)
      {
      s += graphCharacter;
      }
   for (int i = volume; i < MAX_VOLUME; i++)
      {
      s += " ";
      }
   return getProperty(PROPERTY_LABEL_LOW, DEFAULT_LABEL_LOW) + " " + s + " " + getProperty(PROPERTY_LABEL_HIGH, DEFAULT_LABEL_HIGH);
   }

void SetVolumeMenuItemAction::sleepThenPopUpToParentMenuItem()
   {
   sleep(2);
   CharacterDisplayMenuItemAction::stop();
   }

const string SetVolumeMenuItemAction::convertIntToString(const int val)
   {
   std::stringstream out;
   out << val;
   return out.str();
   }

// required definitions for dynamic loading
extern "C"
   {
   void deleteObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(SetVolumeMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, SetVolumeMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new SetVolumeMenuItemAction(deleteObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
