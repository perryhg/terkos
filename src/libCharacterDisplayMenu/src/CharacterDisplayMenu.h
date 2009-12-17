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
