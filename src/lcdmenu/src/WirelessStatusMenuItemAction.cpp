/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WirelessStatusMenuItemAction.h"

const string WirelessStatusMenuItemAction::CLASS_NAME = "WirelessStatusMenuItemAction";

void WirelessStatusMenuItemAction::activate()
   {
   cout << "WirelessStatusMenuItemAction::activate()" << endl;
   WirelessStatusCheckingMenuItemAction::activate();
   currentMenuItemAction->activate();
   }

void WirelessStatusMenuItemAction::start()
   {
   cout << "WirelessStatusMenuItemAction::start()" << endl;
   currentMenuItemAction->start();
   }

void WirelessStatusMenuItemAction::stop()
   {
   cout << "WirelessStatusMenuItemAction::stop()" << endl;
   currentMenuItemAction->stop();
   }

void WirelessStatusMenuItemAction::upEvent()
   {
   cout << "WirelessStatusMenuItemAction::upEvent()" << endl;
   currentMenuItemAction->upEvent();
   }

void WirelessStatusMenuItemAction::downEvent()
   {
   cout << "WirelessStatusMenuItemAction::downEvent()" << endl;
   currentMenuItemAction->downEvent();
   }

void WirelessStatusMenuItemAction::rightEvent()
   {
   cout << "WirelessStatusMenuItemAction::rightEvent()" << endl;
   currentMenuItemAction->rightEvent();
   }

void WirelessStatusMenuItemAction::leftEvent()
   {
   cout << "WirelessStatusMenuItemAction::leftEvent()" << endl;
   currentMenuItemAction->leftEvent();
   }

// ---------------------------------------------------------------------------------------------------------------------

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::activate()
   {
   cout << "WirelessDisabledMenuItemAction::activate()" << endl;

   getCharacterDisplay()->setText("Wireless adapteris disabled.");
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::start()
   {
   cout << "WirelessDisabledMenuItemAction::start()" << endl;

   // start and stop should do the same thing, so just call stop()
   stop();
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::stop()
   {
   cout << "WirelessDisabledMenuItemAction::stop()" << endl;

   // call stop on CharacterDisplayMenuItemAction so that we pop back up to the parent menu item
   CharacterDisplayMenuItemAction::stop();
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::upEvent()
   {
   cout << "WirelessDisabledMenuItemAction::upEvent()" << endl;

   // do nothing, just swallow the event
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::downEvent()
   {
   cout << "WirelessDisabledMenuItemAction::downEvent()" << endl;

   // do nothing, just swallow the event
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::rightEvent()
   {
   cout << "WirelessDisabledMenuItemAction::rightEvent()" << endl;

   // do nothing, just swallow the event
   }

void WirelessStatusMenuItemAction::WirelessDisabledMenuItemAction::leftEvent()
   {
   cout << "WirelessDisabledMenuItemAction::leftEvent()" << endl;

   // do nothing, just swallow the event
   }

// ---------------------------------------------------------------------------------------------------------------------

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::activate()
   {
   cout << "WirelessEnabledMenuItemAction::activate()" << endl;

   getCharacterDisplay()->setText("Wireless adapteris enabled.");
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::start()
   {
   cout << "WirelessEnabledMenuItemAction::start()" << endl;

   // start and stop should do the same thing, so just call stop()
   stop();
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::stop()
   {
   cout << "WirelessEnabledMenuItemAction::stop()" << endl;

   // call stop on CharacterDisplayMenuItemAction so that we pop back up to the parent menu item
   CharacterDisplayMenuItemAction::stop();
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::upEvent()
   {
   cout << "WirelessEnabledMenuItemAction::upEvent()" << endl;

   // do nothing, just swallow the event
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::downEvent()
   {
   cout << "WirelessEnabledMenuItemAction::downEvent()" << endl;

   // do nothing, just swallow the event
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::rightEvent()
   {
   cout << "WirelessEnabledMenuItemAction::rightEvent()" << endl;

   // do nothing, just swallow the event
   }

void WirelessStatusMenuItemAction::WirelessEnabledMenuItemAction::leftEvent()
   {
   cout << "WirelessEnabledMenuItemAction::leftEvent()" << endl;

   // do nothing, just swallow the event
   }

// ---------------------------------------------------------------------------------------------------------------------

// required definitions for dynamic loading
extern "C"
   {
   void deleteWirelessStatusMenuItemActionObject(void* obj)
      {
      delete reinterpret_cast<DynamicObject*> (obj);
      }

   void* loadWirelessStatusMenuItemActionObject(const char* name, int argc, void** argv)
      {
      int numCharsToCompare = std::min(strlen(name), strlen(WirelessStatusMenuItemAction::CLASS_NAME.c_str()));
      if (strncmp(name, WirelessStatusMenuItemAction::CLASS_NAME.c_str(), numCharsToCompare) == 0)
         {
         map<string, string> properties;
         return new WirelessStatusMenuItemAction(deleteWirelessStatusMenuItemActionObject, (MenuItem*) argv[0], (MenuStatusManager*) argv[1], (CharacterDisplay*) argv[2], *((map<
                  string, string>*) argv[3]));
         }

      return NULL;
      }
   }
