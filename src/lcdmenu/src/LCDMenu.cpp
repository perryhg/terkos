/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include <iostream>
#include <string>

#include "CharacterDisplay.h"
#include "LCDCharacterDisplay.h"
#include "MenuStatusManager.h"
#include "DefaultMenuStatusManager.h"
#include "CharacterDisplayMenu.h"
#include "Menu.h"
#include "keypad.h"

using namespace std;

int main(int argc, char** argv)
   {
   string xmlFilename = "/opt/config/lcd_menu.xml";

   // let the user override the location of the XML file by supplying a command-line argument
   if (argc == 3 && strcmp(argv[1], "--xml") == 0)
      {
      xmlFilename = argv[2];
      }
   std::cout << "XML filename = [" << xmlFilename << "]" << std::endl;

   std::cout << "Creating CharacterDisplay" << std::endl;
   CharacterDisplay* lcdPanel = new LCDCharacterDisplay(2, 16);

   std::cout << "Creating DefaultMenuStatusManager" << std::endl;
   DefaultMenuStatusManager* menuStatusManager = new DefaultMenuStatusManager();

   std::cout << "Creating CharacterDisplayMenu" << std::endl;
   Menu* menu = new CharacterDisplayMenu(xmlFilename, menuStatusManager, lcdPanel);

   // If there's welcome text, then display it and then sleep for a couple seconds so it's visible to the user
   if (menu->hasWelcomeText())
      {
      lcdPanel->setText(menu->getWelcomeText());
      sleep(2);
      }

   std::cout << "Setting initial menu item" << std::endl;
   menuStatusManager->setActiveMenuItem(menu->getDefaultMenuItem());

   // create a map of keypad codes to event handler method pointers
   typedef void (DefaultMenuStatusManager::*methodPtr)();
   map<const unsigned int, methodPtr> eventHandlers;
   eventHandlers[CKeypad::KEY_OK] = &DefaultMenuStatusManager::handleStartEvent;
   eventHandlers[CKeypad::KEY_CANCEL] = &DefaultMenuStatusManager::handleStopEvent;
   eventHandlers[CKeypad::KEY_UP] = &DefaultMenuStatusManager::handleUpEvent;
   eventHandlers[CKeypad::KEY_DOWN] = &DefaultMenuStatusManager::handleDownEvent;
   eventHandlers[CKeypad::KEY_LEFT] = &DefaultMenuStatusManager::handleLeftEvent;
   eventHandlers[CKeypad::KEY_RIGHT] = &DefaultMenuStatusManager::handleRightEvent;

   // loop forever getting the current key press and passing to the appropriate event handler
   CKeypad keypad;
   while (true)
      {
      const unsigned int key = keypad.GetKey();
      if (eventHandlers[key])
         {
         (menuStatusManager->*eventHandlers[key])();
         }
      else
         {
         std::cerr << "Unexpected key [" << key << "]" << std::endl;
         }
      }

   return 0;
   }
