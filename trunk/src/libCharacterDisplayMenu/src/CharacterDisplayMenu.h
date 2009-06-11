/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef CHARACTERDISPLAYMENU_H_
#define CHARACTERDISPLAYMENU_H_

#include <iostream>
#include <map>
#include <string>
#include <ticpp.h>
#include <tinyxml.h>

#include "Menu.h"
#include "MenuStatusManager.h"
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"
#include "MenuItem.h"
#include "MenuItemImpl.h"
#include "DynamicLoader.h"
#include "DynamicLibrary.h"

using namespace std;
using namespace ticpp;

class CharacterDisplayMenu : public Menu
   {
   public:

      static const string EMPTY_STRING;
      static const unsigned int DYNAMIC_OBJECT_ARGUMENT_ARRAY_SIZE;

      CharacterDisplayMenu(const string& xmlFilename, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay);

      virtual ~CharacterDisplayMenu()
         {
         delete menuItem;

         // delete the dynamic libraries
         for (unsigned int i = 0; i < dynamicLibraries.size(); i++)
            {
            delete dynamicLibraries[i];
            }
         dynamicLibraries.clear();
         }

      MenuItem* getDefaultMenuItem() const
         {
         return menuItem->getFirstChild();
         }

      const bool hasWelcomeText() const
         {
         return welcomeText.length() > 0;
         }

      const string getWelcomeText() const
         {
         return welcomeText;
         }

   private:

      MenuItem* menuItem;
      string welcomeText;
      vector<DynamicLibrary*> dynamicLibraries;

      MenuItemImpl* buildMenuItemTree(ticpp::Element* parentElement, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay);

      MenuItemAction* createMenuItemAction(ticpp::Element* parentElement, MenuItemImpl* menuItem, MenuStatusManager* menuStatusManager,
                                           CharacterDisplay* characterDisplay);

      MenuItemAction* instantiateMenuItemAction(const string& sharedLibraryName, const string& className, MenuItem* menuItem,
                                                MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                                                map<string, string>& properties);

   };

#endif /* CHARACTERDISPLAYMENU_H_ */
