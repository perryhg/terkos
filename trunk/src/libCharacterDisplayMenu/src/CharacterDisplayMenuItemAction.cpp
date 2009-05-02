/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "CharacterDisplayMenuItemAction.h"

void CharacterDisplayMenuItemAction::activate()
   {
   if (getMenuItem()->hasSiblings())
      {
      characterDisplay->setTextWithScrollArrows(getMenuItem()->getText());
      }
   else
      {
      characterDisplay->setText(getMenuItem()->getText());
      }
   }


