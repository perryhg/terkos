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
#include <NoOpMenuItemAction.h>
#include <StringUtilities.h>
#include "WirelessStatusCheckingMenuItemAction.h"

using namespace std;
using namespace redi;

class ViewWirelessNetworksMenuItemAction : public WirelessStatusCheckingMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string STATUS_FAILURE_PROPERTY_ACTION_PROMPT;
      static const string STATUS_FAILURE_DEFAULT_ACTION_PROMPT;

      static const string STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT;
      static const string STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT;

      static const string STATUS_DISABLED_PROPERTY_ACTION_PROMPT;
      static const string STATUS_DISABLED_DEFAULT_ACTION_PROMPT;

      ViewWirelessNetworksMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                         CharacterDisplay* characterDisplay, map<string, string>& properties) :
         WirelessStatusCheckingMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         wirelessFailureMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_FAILURE_PROPERTY_ACTION_PROMPT, STATUS_FAILURE_DEFAULT_ACTION_PROMPT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessUnpluggedMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_UNPLUGGED_PROPERTY_ACTION_PROMPT, STATUS_UNPLUGGED_DEFAULT_ACTION_PROMPT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessDisabledMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_DISABLED_PROPERTY_ACTION_PROMPT, STATUS_DISABLED_DEFAULT_ACTION_PROMPT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         listModeMenuItemAction
                  = new ListModeMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay, properties);
         }

      virtual ~ViewWirelessNetworksMenuItemAction()
         {
         currentMenuItemAction = NULL;

         delete listModeMenuItemAction;
         delete detailModeMenuItemAction;

         delete wirelessFailureMenuItemAction;
         delete wirelessUnpluggedMenuItemAction;
         delete wirelessDisabledMenuItemAction;
         }

      void activate();
      void start();
      void stop();
      void upEvent();
      void downEvent();
      void rightEvent();
      void leftEvent();

   protected:

      virtual void handleWirelessEnabled()
         {
         currentMenuItemAction = listModeMenuItemAction;
         }

      virtual void handleWirelessDisabled()
         {
         currentMenuItemAction = wirelessDisabledMenuItemAction;
         }

      virtual void handleWirelessUnplugged()
         {
         currentMenuItemAction = wirelessUnpluggedMenuItemAction;
         }

      virtual void handleWirelessStatusFailure()
         {
         currentMenuItemAction = wirelessFailureMenuItemAction;
         }

   private:

      MenuItemAction* currentMenuItemAction;

      MenuItemAction* listModeMenuItemAction;
      MenuItemAction* detailModeMenuItemAction;

      MenuItemAction* wirelessFailureMenuItemAction;
      MenuItemAction* wirelessUnpluggedMenuItemAction;
      MenuItemAction* wirelessDisabledMenuItemAction;

      class ListModeMenuItemAction : public CharacterDisplayMenuItemAction
         {
         public:

            ListModeMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                           CharacterDisplay* characterDisplay, map<string, string>& properties) :
               CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties), wirelessNetworks(Json::Value::null), currentWirelessNetworkIndex(0)
               {
               // nothing to do
               }

            virtual ~ListModeMenuItemAction()
               {
               // nothing to do
               }

            void activate();

            void start();

            void stop()
               {
               CharacterDisplayMenuItemAction::stop();
               }

            void upEvent();

            void downEvent();

            void rightEvent()
               {
               downEvent();
               }

            void leftEvent()
               {
               upEvent();
               }

         private:

            Json::Value wirelessNetworks;

            unsigned int currentWirelessNetworkIndex;

            void displayWirelessNetworkDetails();

            const unsigned int getNumberOfWirelessNetworks() const
               {
               if (wirelessNetworks != Json::Value::null)
                  {
                  return wirelessNetworks["wireless-networks"].size();
                  }
               return 0;
               }

         };
   };

#endif /* VIEWWIRELESSNETWORKSMENUITEMACTION_H_ */
