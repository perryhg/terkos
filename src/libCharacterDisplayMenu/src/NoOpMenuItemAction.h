/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef NOOPMENUITEMACTION_H_
#define NOOPMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <CharacterDisplayMenuItemAction.h>
#include <StringUtilities.h>

using namespace std;
using namespace redi;

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

#endif /* NOOPMENUITEMACTION_H_ */
