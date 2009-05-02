/*
 * Author: Chris Bartley (bartley@cmu.edu)
 */

#include <iostream>
#include <string>

#include "CharacterDisplay.h"
#include "LCDCharacterDisplay.h"
#include "MenuStatusManager.h"
#include "DefaultMenuStatusManager.h"
#include "CharacterDisplayMenu.h"
#include "Menu.h"

using namespace std;

int main(int argc, char** argv)
   {
   // TODO: define this somewhere else
   string xmlFilename = "menu.xml";

   std::cout << "Creating CharacterDisplay" << std::endl;
   CharacterDisplay* lcdPanel = new LCDCharacterDisplay(2, 16);

   std::cout << "Creating DefaultMenuStatusManager" << std::endl;
   DefaultMenuStatusManager* menuStatusManager = new DefaultMenuStatusManager();

   std::cout << "Creating CharacterDisplayMenu" << std::endl;
   Menu* menu = new CharacterDisplayMenu(xmlFilename, menuStatusManager, lcdPanel);

   // display welcome text
   lcdPanel->setText("Welcome to Vex!    IFI, Inc.");
   sleep(2);

   std::cout << "Setting initial menu item" << std::endl;
   menuStatusManager->setActiveMenuItem(menu->getDefaultMenuItem());

   // do a little testing
   menuStatusManager->handleDownEvent();
   menuStatusManager->handleDownEvent();

   // TODO: go into a loop here which checks the button pad and sends events to the menuStatusManager

   return 0;
   }
