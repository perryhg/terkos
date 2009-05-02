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
