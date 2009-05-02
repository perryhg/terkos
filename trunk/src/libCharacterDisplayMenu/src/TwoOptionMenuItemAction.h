/**
 * TwoOptionMenuItemAction provides base functionality for all CharacterDisplayMenuItemActions
 * which need to prompt the user with a choice between two options (and a third option to cancel altogether) where
 * different actions can be performed depending on the user's choice.
 *
 * Users and subclasses should override the default prompt ("Choose Yes or No") and the default "Yes" and "No" choices
 * by constructing the instance with a map containing keys "action.prompt",
 * "choice.option1", and "choice.option2".  The values for those keys will be used instead of the
 * defaults.  The messages displayed upon the user choosing option1, option2, or cancel can be similarly customized by
 * specifying "action.option1", "action.option2", and "action.cancel".
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef TWOOPTIONMENUITEMACTION_H_
#define TWOOPTIONMENUITEMACTION_H_

#include <string>
#include <unistd.h>
#include "CharacterDisplayMenuItemAction.h"

class TwoOptionMenuItemAction : public CharacterDisplayMenuItemAction
   {
   public:

      static const string DEFAULT_ACTION_PROMPT;
      static const string DEFAULT_ACTION_CHOSE_OPTION1;
      static const string DEFAULT_ACTION_CHOSE_OPTION2;
      static const string DEFAULT_ACTION_CHOSE_CANCEL;
      static const string DEFAULT_LABEL_OPTION1;
      static const string DEFAULT_LABEL_OPTION2;

      static const string PROPERTY_CHOICE_OPTION1;
      static const string PROPERTY_CHOICE_OPTION2;
      static const string PROPERTY_ACTION_PROMPT;
      static const string PROPERTY_ACTION_CHOSE_OPTION1;
      static const string PROPERTY_ACTION_CHOSE_OPTION2;
      static const string PROPERTY_ACTION_CHOSE_CANCEL;

      TwoOptionMenuItemAction(void(*delObj)(void*), MenuItem* menuItem, MenuStatusManager* menuStatusManager, CharacterDisplay* characterDisplay,
                              map<string, string>& properties) :
         CharacterDisplayMenuItemAction(delObj, menuItem, menuStatusManager, characterDisplay, properties)
         {
         userChoseOption1 = false;
         }

      virtual ~TwoOptionMenuItemAction()
         {
         // nothing to do
         }

      virtual void activate();

      virtual void start();

      virtual void stop();

      virtual void upEvent();

      virtual void rightEvent();

      virtual void downEvent();

      virtual void leftEvent();

   protected:

      virtual bool shouldOption1BeSelectedUponActivation() const = 0;

      virtual void executeOption1Action() = 0;

      virtual void executeOption2Action() = 0;

   private:

      bool userChoseOption1;

      const string getPromptText()
         {
         return getProperty(PROPERTY_ACTION_PROMPT, DEFAULT_ACTION_PROMPT);
         }

      const string getUserChoseOption1Text()
         {
         return getProperty(PROPERTY_ACTION_CHOSE_OPTION1, DEFAULT_ACTION_CHOSE_OPTION1);
         }

      const string getUserChoseOption2Text()
         {
         return getProperty(PROPERTY_ACTION_CHOSE_OPTION2, DEFAULT_ACTION_CHOSE_OPTION2);
         }

      const string getUserChoseCancelText()
         {
         return getProperty(PROPERTY_ACTION_CHOSE_CANCEL, DEFAULT_ACTION_CHOSE_CANCEL);
         }

      const string generateOptionChoiceLine()
         {
         const string option1Choice = (userChoseOption1 ? "*" : " ");
         const string option2Choice = (userChoseOption1 ? " " : "*");

         return "[" + option1Choice + "]" + getProperty(PROPERTY_CHOICE_OPTION1, DEFAULT_LABEL_OPTION1) + "[" + option2Choice + "]"
                  + getProperty(PROPERTY_CHOICE_OPTION2, DEFAULT_LABEL_OPTION2);
         }

      void sleepThenPopUpToParentMenuItem();
   };

#endif /* TWOOPTIONMENUITEMACTION_H_ */
