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

#ifndef SETUSERPROGRAMTORUNONBOOTMENUITEMACTION_H_
#define SETUSERPROGRAMTORUNONBOOTMENUITEMACTION_H_

#include <algorithm>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <CharacterDisplayMenuItemAction.h>
#include <StringUtilities.h>
#include "UserProgramsConfigManager.h"

using namespace std;

class SetUserProgramToRunOnBootMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string CLASS_NAME;

      static const string DEFAULT_LABEL_PROGRAM;
      static const string DEFAULT_LABEL_NO_PROGRAM;
      static const string DEFAULT_LABEL_NO_PROGRAM_WILL_RUN_ON_BOOT;
      static const string DEFAULT_LABEL_ACTION_PERFORMED;
      static const string DEFAULT_LABEL_ACTION_CANCELLED;
      static const string DEFAULT_LABEL_ACTION_FAILED;

      static const string PROPERTY_PROGRAM;
      static const string PROPERTY_NO_PROGRAM;
      static const string PROPERTY_NO_PROGRAM_WILL_RUN_ON_BOOT;
      static const string PROPERTY_ACTION_PERFORMED;
      static const string PROPERTY_ACTION_CANCELLED;
      static const string PROPERTY_ACTION_FAILED;

      SetUserProgramToRunOnBootMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager,
                                              CharacterDisplay* characterDisplay, map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         // nothing to do
         }

      virtual ~SetUserProgramToRunOnBootMenuItemAction()
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

      UserProgramsConfigManager configManager;
      vector<string> userProgramNames;
      unsigned int selectedUserProgramIndex;

      void displayActionPerformedText();
      void displayActionCancelledText();
      void displayActionFailedText();
      void displaySelectedUserProgram();
      void sleepThenPopUpToParentMenuItem();

      const unsigned int getNumberOfUserPrograms() const
         {
         return userProgramNames.size();
         }

      const string getSelectedProgramName() const
         {
         return userProgramNames[selectedUserProgramIndex];
         }
   };

#endif /* SETUSERPROGRAMTORUNONBOOTMENUITEMACTION_H_ */
