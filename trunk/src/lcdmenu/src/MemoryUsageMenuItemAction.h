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

   };

#endif /* MEMORYUSAGEMENUITEMACTION_H_ */
