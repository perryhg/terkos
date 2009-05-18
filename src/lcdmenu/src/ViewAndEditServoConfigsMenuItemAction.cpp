/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ViewAndEditServoConfigsMenuItemAction.h"

const string ViewAndEditServoConfigsMenuItemAction::CLASS_NAME = "ViewAndEditServoConfigsMenuItemAction";

void ViewAndEditServoConfigsMenuItemAction::activate()
   {
   // reset the current servo id
   currentServoId = 0;

   displayServoConfigDetails();
   }

void ViewAndEditServoConfigsMenuItemAction::start()
   {
   if (isDisplayMode)
      {
      // TODO
      }
   else
      {
      // TODO
      }
   }

void ViewAndEditServoConfigsMenuItemAction::stop()
   {
   if (isDisplayMode)
      {
      CharacterDisplayMenuItemAction::stop();
      }
   else
      {
      // TODO
      }
   }

void ViewAndEditServoConfigsMenuItemAction::upEvent()
   {
   if (isDisplayMode)
      {
      // decrement the current servo ID, wrapping around if necessary
      if (currentServoId == 0)
         {
         currentServoId = ServoConfigManager::NUM_SERVOS - 1;
         }
      else
         {
         currentServoId--;
         }

      displayServoConfigDetails();
      }
   else
      {
      // TODO
      }
   }

void ViewAndEditServoConfigsMenuItemAction::downEvent()
   {
   if (isDisplayMode)
      {
      // increment the current servo ID, wrapping around if necessary
      if (currentServoId >= ServoConfigManager::NUM_SERVOS - 1)
         {
         currentServoId = 0;
         }
      else
         {
         currentServoId++;
         }

      displayServoConfigDetails();
      }
   else
      {
      // TODO
      }
   }

void ViewAndEditServoConfigsMenuItemAction::rightEvent()
   {
   if (isDisplayMode)
      {
      downEvent();
      }
   else
      {
      // TODO
      }
   }

void ViewAndEditServoConfigsMenuItemAction::leftEvent()
   {
   if (isDisplayMode)
      {
      upEvent();
      }
   else
      {
      // TODO
      }
   }

void ViewAndEditServoConfigsMenuItemAction::displayServoConfigDetails()
   {
   string servoStr = StringUtilities::convertIntToString(currentServoId, 2, '0');
   string initialPositionStr = StringUtilities::convertIntToString(configManager.getInitialPosition(currentServoId), 3, '0');
   string minBoundStr = StringUtilities::convertIntToString(configManager.getMinBound(currentServoId), 3, '0');
   string maxBoundStr = StringUtilities::convertIntToString(configManager.getMaxBound(currentServoId), 3, '0');

   string text = "Servo" + servoStr + " Min" + minBoundStr + "Init" + initialPositionStr + " Max" + maxBoundStr;

   getCharacterDisplay()->setTextWithScrollArrows(text);
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
      int numCharsToCompare = std::min(strlen(name), strlen(ViewAndEditServoConfigsMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, ViewAndEditServoConfigsMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new ViewAndEditServoConfigsMenuItemAction(deleteObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
