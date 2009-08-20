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
#include <TwoOptionMenuItemAction.h>
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

      static const string STATUS_FAILURE_PROPERTY;
      static const string STATUS_FAILURE_DEFAULT;

      static const string STATUS_UNPLUGGED_PROPERTY;
      static const string STATUS_UNPLUGGED_DEFAULT;

      static const string STATUS_DISABLED_PROPERTY;
      static const string STATUS_DISABLED_DEFAULT;

      static const string STATUS_NO_NETWORKS_FOUND_PROPERTY;
      static const string STATUS_NO_NETWORKS_FOUND_DEFAULT;

      static const string SCANNING_LABEL_PROPERTY;
      static const string SCANNING_LABEL_DEFAULT;

      static const string NETWORK_LABEL_PROPERTY;
      static const string NETWORK_LABEL_DEFAULT;

      static const string JOIN_NETWORK_PROMPT_PROPERTY;
      static const string JOIN_NETWORK_PROMPT_DEFAULT;

      static const string YES_OPTION_PROPERTY;
      static const string YES_OPTION_DEFAULT;

      static const string NO_OPTION_PROPERTY;
      static const string NO_OPTION_DEFAULT;

      static const string CONNECTING_TO_LABEL_PROPERTY;
      static const string CONNECTING_TO_LABEL_DEFAULT;

      static const string STATUS_CONNECTION_SUCESS_PROPERTY;
      static const string STATUS_CONNECTION_SUCESS_DEFAULT;

      static const string STATUS_CONNECTION_FAILURE_PROPERTY;
      static const string STATUS_CONNECTION_FAILURE_DEFAULT;

      ViewWirelessNetworksMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                         CharacterDisplay* characterDisplay, map<string, string>& properties) :
         WirelessStatusCheckingMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         wirelessFailureMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_FAILURE_PROPERTY, STATUS_FAILURE_DEFAULT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessUnpluggedMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_UNPLUGGED_PROPERTY, STATUS_UNPLUGGED_DEFAULT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessDisabledMenuItemAction
                  = new NoOpMenuItemAction(getProperty(STATUS_DISABLED_PROPERTY, STATUS_DISABLED_DEFAULT), NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessEnabledMenuItemAction = new WirelessEnabledMenuItemAction(this, NULL, menuItem, menuStatusManager, characterDisplay, properties);
         }

      virtual ~ViewWirelessNetworksMenuItemAction()
         {
         currentMenuItemAction = NULL;

         delete wirelessEnabledMenuItemAction;
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
         currentMenuItemAction = wirelessEnabledMenuItemAction;
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
      MenuItemAction* wirelessEnabledMenuItemAction;
      MenuItemAction* wirelessFailureMenuItemAction;
      MenuItemAction* wirelessUnpluggedMenuItemAction;
      MenuItemAction* wirelessDisabledMenuItemAction;

      class WirelessEnabledMenuItemAction : public CharacterDisplayMenuItemAction
         {
         public:

            WirelessEnabledMenuItemAction(ViewWirelessNetworksMenuItemAction* parentMenuItemAction, void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                   CharacterDisplay* characterDisplay, map<string, string>& properties) :
               CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties),
                        parentMenuItemAction(parentMenuItemAction), wirelessNetworks(Json::Value::null), currentWirelessNetworkIndex(0), isListMode(true)
               {
               // nothing to do
               }

            virtual ~WirelessEnabledMenuItemAction()
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

            ViewWirelessNetworksMenuItemAction* parentMenuItemAction;
            Json::Value wirelessNetworks;
            unsigned int currentWirelessNetworkIndex;
            bool isListMode;
            bool willJoinNetwork;

            void displayWirelessNetworkDetails();

            const unsigned int getNumberOfWirelessNetworks() const
               {
               if (wirelessNetworks != Json::Value::null)
                  {
                  return wirelessNetworks["wireless-networks"].size();
                  }
               return 0;
               }

            const string getCurrentNetworkSSID() const
               {
               if (currentWirelessNetworkIndex < getNumberOfWirelessNetworks())
                  {
                  Json::Value ssid = wirelessNetworks["wireless-networks"][currentWirelessNetworkIndex]["ssid"];
                  return ssid.asString();
                  }

               cerr << "WirelessEnabledMenuItemAction::getCurrentNetworkSSID(): no SSID to return since no network is selected" << endl;
               return "";
               }

            void switchToListModeMode()
               {
               isListMode = true;
               displayWirelessNetworkDetails();
               }

            void switchToJoinNetworkMode()
               {
               isListMode = false;
               activate();
               }

            const string generateJoinNetworkChoiceLine()
               {
               const string yesChoice = (willJoinNetwork ? "*" : " ");
               const string noChoice = (willJoinNetwork ? " " : "*");

               return getProperty(JOIN_NETWORK_PROMPT_PROPERTY, JOIN_NETWORK_PROMPT_DEFAULT) +
                      "[" + yesChoice + "]" +
                      getProperty(YES_OPTION_PROPERTY, YES_OPTION_DEFAULT) +
                      "[" + noChoice + "]" +
                      getProperty(NO_OPTION_PROPERTY, NO_OPTION_DEFAULT);
               }
         };

   };

#endif /* VIEWWIRELESSNETWORKSMENUITEMACTION_H_ */
