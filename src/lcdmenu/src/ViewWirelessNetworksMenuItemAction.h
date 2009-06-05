/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef VIEWWIRELESSNETWORKSMENUITEMACTION_H_
#define VIEWWIRELESSNETWORKSMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pstream.h>
#include <json/json.h>
#include <CharacterDisplayMenuItemAction.h>
#include <StringUtilities.h>

using namespace std;
using namespace redi;

class ViewWirelessNetworksMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      ViewWirelessNetworksMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                            CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         currentWirelessNetworkIndex = 0;
         isDetailViewMode = false;
         }

      virtual ~ViewWirelessNetworksMenuItemAction()
         {
         currentMenuItemAction = NULL;
         delete listModeMenuItemAction;
         delete detailModeMenuItemAction;
         }

      void activate();
      void start();
      void stop();
      void upEvent();
      void downEvent();
      void rightEvent();
      void leftEvent();

   private:

      Json::Value wirelessNetworks;

      unsigned int currentWirelessNetworkIndex;

      bool isDetailViewMode;

      MenuItemAction* currentMenuItemAction;
      MenuItemAction* listModeMenuItemAction;
      MenuItemAction* detailModeMenuItemAction;

      void displayWirelessNetworkDetails();

      const unsigned int getNumberOfWirelessNetworks() const;

   };

#endif /* VIEWWIRELESSNETWORKSMENUITEMACTION_H_ */
