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

#include <iostream>
#include <string>
#include <time.h>
#include <unistd.h>

#include <CharacterDisplay.h>
#include <CharacterDisplayMenu.h>
#include <LCDConfigManager.h>
#include <MenuStatusManager.h>
#include <DefaultMenuStatusManager.h>
#include <Menu.h>
#include <keypad.h>

#include "LCDCharacterDisplay.h"

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
   LCDCharacterDisplay* lcdPanel = new LCDCharacterDisplay(2, 16);

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

   // Get the backlight timeout in seconds, and then make sure the backlight is set to the proper state
   LCDConfigManager lcdConfigManager;
   int backlightTimeoutInSeconds = lcdConfigManager.getBacklightTimeout();
   if (backlightTimeoutInSeconds == LCDConfigManager::BACKLIGHT_TIMEOUT_VALUE_ALWAYS_ON)
      {
      lcdPanel->setBacklight(true);
      }
   else
      {
      lcdPanel->setBacklight(false);
      }

   // loop forever getting the current key press and passing to the appropriate event handler
   try
      {
      time_t timeOfLastKeypress, currentTime;

      // initialize the time of the last keypress to the current time
      time(&timeOfLastKeypress);

      // get a reference to the keypad
      CKeypad &keypad = CKeypad::GetRef();

      // we need to keep track of the previous key so that when the user holds a button down
      // it's not treated as more than 1 keypress
      unsigned int previousKey = 0;

      while (true)
         {
         // get current key, if any
         const unsigned int key = keypad.GetKey(false);

         // don't do anything unless the key is different
         if (key != previousKey)
            {
            // react to key press
            if (key == 0)
               {
               // do nothing since no key was pressed
               }
            else if (eventHandlers[key])
               {
               // record the current time as the time of the last keypress
               time(&timeOfLastKeypress);

               // if backlight timeout is enabled, then turn on the backlight
               if (backlightTimeoutInSeconds != 0)
                  {
                  lcdPanel->setBacklight(true);
                  }

               (menuStatusManager->*eventHandlers[key])();
               }
            else
               {
               std::cerr << "Unexpected key [" << key << "]" << std::endl;
               }
            }

         // see if we need to turn off the backlight
         if (backlightTimeoutInSeconds > 0)
            {
            // get the current time
            time(&currentTime);

            // if the timeout has expired, then turn off the backlight
            if (difftime(currentTime, timeOfLastKeypress) > backlightTimeoutInSeconds)
               {
               lcdPanel->setBacklight(false);
               }
            }

         previousKey = key;
         usleep(33333);   // roughly 30 Hz
         }
         
      // release the keypad reference
      CKeypad::Release();
      }
   catch (...)
      {
      // TODO: change to real logging
      std::cerr << "Caught exception, aborting..." << std::endl;
      }

   return 0;
   }
