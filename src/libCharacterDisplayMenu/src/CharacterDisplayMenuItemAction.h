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

#ifndef CHARACTERDISPLAYMENUITEMACTION_H_
#define CHARACTERDISPLAYMENUITEMACTION_H_

#include <map>
#include <DynamicObject.h>
#include "DefaultMenuItemAction.h"
#include "CharacterDisplay.h"

class CharacterDisplayMenuItemAction : public DynamicObject, public DefaultMenuItemAction
   {
   public:

      CharacterDisplayMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                     CharacterDisplay* characterDisplay) :
         DynamicObject(delObj), DefaultMenuItemAction(menuItem, menuStatusManager), characterDisplay(characterDisplay)
         {
         // nothing to do
         }

      CharacterDisplayMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                     CharacterDisplay* characterDisplay, map<string, string>& theProperties) :
         DynamicObject(delObj), DefaultMenuItemAction(menuItem, menuStatusManager), characterDisplay(characterDisplay)
         {
         // copy the properties
         this->properties = theProperties;
         }

      virtual ~CharacterDisplayMenuItemAction()
         {
         // nothing to do
         }

      virtual void activate();

   protected:

      CharacterDisplay* getCharacterDisplay()
         {
         return characterDisplay;
         }

      map<string, string>& getProperties()
         {
         return properties;
         }

      const string getProperty(const string& key, const string& defaultValue = "")
         {
         map<string, string>::iterator iter = properties.find(key);
         return (iter == properties.end()) ? defaultValue : iter->second;
         }

   private:

      CharacterDisplay* characterDisplay;
      map<string, string> properties;
   };

#endif /* CHARACTERDISPLAYMENUITEMACTION_H_ */
