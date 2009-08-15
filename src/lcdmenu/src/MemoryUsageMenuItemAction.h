/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef MEMORYUSAGEMENUITEMACTION_H_
#define MEMORYUSAGEMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <json/json.h>
#include "CharacterDisplayMenuItemAction.h"
#include "MemoryUsageManager.h"
#include "StringUtilities.h"

class MemoryUsageMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      MemoryUsageMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                            CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties), numColumnsWhenInScrollingMode(characterDisplay->getColumnsWhenInScrollingMode())
         {
         // nothing to do
         }

      virtual ~MemoryUsageMenuItemAction()
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

      static const unsigned int NUM_COMPONENTS;

      static const string UNKNOWN_VALUE;
      
      static const string TOTAL_MEMORY_PROPERTY;
      static const string USED_MEMORY_PROPERTY;
      static const string FREE_MEMORY_PROPERTY;

      static const string TOTAL_MEMORY_DEFAULT_LABEL;
      static const string USED_MEMORY_DEFAULT_LABEL;
      static const string FREE_MEMORY_DEFAULT_LABEL;

      MemoryUsageManager memoryUsageManager;

      const unsigned int numColumnsWhenInScrollingMode;
      string totalMemory;
      string usedMemory;
      string freeMemory;

      unsigned int currentComponentId;

      void loadValues();
      void displayComponentDetails();
      void displayComponentDetailsWorkhorse(const string& label, const string& value);

      const string rightPadOrTrimRight(const string& str, unsigned int desiredLength);
   };

#endif /* MEMORYUSAGEMENUITEMACTION_H_ */
