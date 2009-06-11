/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WIRELESSSTATUSMENUITEMACTION_H_
#define WIRELESSSTATUSMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <pstream.h>
#include <json/json.h>
#include <CharacterDisplayMenuItemAction.h>
#include <StringUtilities.h>
#include "WirelessStatusCheckingMenuItemAction.h"

using namespace std;
using namespace redi;

class WirelessStatusMenuItemAction : public WirelessStatusCheckingMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      WirelessStatusMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                   CharacterDisplay* characterDisplay, map<string, string>& properties) :
         WirelessStatusCheckingMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         wirelessFailureMenuItemAction
                  = new NoOpMenuItemAction("Failed to get   wireless status.", NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessUnpluggedMenuItemAction
                  = new NoOpMenuItemAction("Wireless adapteris unplugged.", NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessDisabledMenuItemAction = new WirelessDisabledMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay, properties);
         wirelessEnabledMenuItemAction = new WirelessEnabledMenuItemAction(NULL, menuItem, menuStatusManager, characterDisplay, properties);
         }

      virtual ~WirelessStatusMenuItemAction()
         {
         currentMenuItemAction = NULL;
         delete wirelessFailureMenuItemAction;
         delete wirelessUnpluggedMenuItemAction;
         delete wirelessDisabledMenuItemAction;
         delete wirelessEnabledMenuItemAction;
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
         cout << "WirelessStatusMenuItemAction.handleWirelessEnabled()" << endl;
         currentMenuItemAction = wirelessEnabledMenuItemAction;
         }

      virtual void handleWirelessDisabled()
         {
         cout << "WirelessStatusMenuItemAction.handleWirelessDisabled()" << endl;
         currentMenuItemAction = wirelessDisabledMenuItemAction;
         }

      virtual void handleWirelessUnplugged()
         {
         cout << "WirelessStatusMenuItemAction.handleWirelessUnplugged()" << endl;
         currentMenuItemAction = wirelessUnpluggedMenuItemAction;
         }

      virtual void handleWirelessStatusFailure()
         {
         cout << "WirelessStatusMenuItemAction.handleWirelessStatusFailure()" << endl;
         currentMenuItemAction = wirelessFailureMenuItemAction;
         }

   private:

      MenuItemAction* currentMenuItemAction;
      MenuItemAction* wirelessFailureMenuItemAction;
      MenuItemAction* wirelessUnpluggedMenuItemAction;
      MenuItemAction* wirelessDisabledMenuItemAction;
      MenuItemAction* wirelessEnabledMenuItemAction;

      class NoOpMenuItemAction : public CharacterDisplayMenuItemAction
         {
         public:

            NoOpMenuItemAction(string message, void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                               CharacterDisplay* characterDisplay, map<string, string>& properties) :
               CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties), message(message)
               {
               // nothing to do
               }

            virtual ~NoOpMenuItemAction()
               {
               // nothing to do
               }

            void activate()
               {
               getCharacterDisplay()->setText(message);
               }

            void start()
               {
               // start and stop should do the same thing, so just call stop()
               stop();
               }

            void stop()
               {
               // call stop on CharacterDisplayMenuItemAction so that we pop back up to the parent menu item
               CharacterDisplayMenuItemAction::stop();
               }

            void upEvent()
               {
               // do nothing, just swallow the event
               }

            void downEvent()
               {
               // do nothing, just swallow the event
               }

         private:

            string message;

         };

      class WirelessDisabledMenuItemAction : public CharacterDisplayMenuItemAction
         {
         public:

            WirelessDisabledMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                           CharacterDisplay* characterDisplay, map<string, string>& properties) :
               CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
               {
               // nothing to do
               }

            virtual ~WirelessDisabledMenuItemAction()
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

         };

      class WirelessEnabledMenuItemAction : public CharacterDisplayMenuItemAction
         {
         public:

            WirelessEnabledMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                          CharacterDisplay* characterDisplay, map<string, string>& properties) :
               CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
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

         };
   };

#endif /* WIRELESSSTATUSMENUITEMACTION_H_ */
