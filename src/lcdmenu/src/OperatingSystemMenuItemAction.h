/**
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef OPERATINGSYSTEMMENUITEMACTION_H_
#define OPERATINGSYSTEMMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <VersionInfoManager.h>
#include "CharacterDisplay.h"
#include "CharacterDisplayMenuItemAction.h"

using namespace std;

class OperatingSystemMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;
      static const string PROPERTY_LABEL;
      static const string DEFAULT_LABEL;

      OperatingSystemMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                    CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~OperatingSystemMenuItemAction()
         {
         // nothing to do
         }

      virtual void activate();

   private:

      VersionInfoManager versionInfoManager;

   };

#endif /* OPERATINGSYSTEMMENUITEMACTION_H_ */
