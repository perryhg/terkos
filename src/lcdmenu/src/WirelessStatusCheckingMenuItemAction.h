/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WIRELESSSTATUSCHECKINGMENUITEMACTION_H_
#define WIRELESSSTATUSCHECKINGMENUITEMACTION_H_

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

class WirelessStatusCheckingMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      WirelessStatusCheckingMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                           CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~WirelessStatusCheckingMenuItemAction()
         {
         // nothing to do
         }

      void activate();

   protected:

      virtual void handleWirelessEnabled() = 0;
      virtual void handleWirelessDisabled() = 0;
      virtual void handleWirelessUnplugged() = 0;
      virtual void handleWirelessStatusFailure() = 0;
      Json::Value getWirelessNetworkingStatus();

   private:

      Json::Value wirelessNetworkingStatusJson;

   };

#endif /* WIRELESSSTATUSCHECKINGMENUITEMACTION_H_ */
