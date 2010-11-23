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

#include <unistd.h>
#include <stdio.h>
#include <string>
#include "textlcd.h"

using namespace std;

CTextLcd *lcd;

void waitForLCDPtr()
   {
   if (!lcd)
      {
      while (1)
         {
         // Try to get the pointer, waiting up to 1 second to get it.  If it fails, GetPtr returns null.  The true arg
         // suppresses errors from being printed
         lcd = CTextLcd::GetPtr(true);

         if (lcd)
            {
            break;
            }
         }
      }
   }

void releaseLCDPtrIfRequested()
   {
   if (lcd && lcd->Requested())
     {
     lcd->Release();
     lcd = NULL;
     }
   }

void defaultTest()
   {
   long width = -1;
   long height = -1;
   waitForLCDPtr();
   lcd->GetProperty(TL_PROP_HEIGHT, &height);
   lcd->GetProperty(TL_PROP_WIDTH, &width);
   printf("Dimensions (via properties): (%d,%d)\n", width, height);
   printf("Dimensions (via consts):     (%d,%d)\n", CTextLcd::NUM_COLUMNS, CTextLcd::NUM_ROWS);
   releaseLCDPtrIfRequested();

   printf("Clearing LCD\n");
   waitForLCDPtr();
   lcd->Clear();
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("Setting text ''\n");
   waitForLCDPtr();
   lcd->MoveCursor(0,0);
   lcd->printf("");
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("Setting text 'Hello there'\n");
   waitForLCDPtr();
   lcd->MoveCursor(0,0);
   lcd->printf("Hello there");
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("Setting text 'Hello there.    How are you?'\n");
   waitForLCDPtr();
   lcd->MoveCursor(0,0);
   lcd->printf("Hello there.    How are you?");
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("Setting text 'This is some really long text that will not fit'\n");
   waitForLCDPtr();
   lcd->MoveCursor(0,0);
   lcd->printf("This is some really long text that will not fit.");
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("Special characters\n");
   waitForLCDPtr();
   lcd->Clear();
   lcd->printf("Special chars:");
   lcd->MoveCursor(1,0);
   lcd->printf("%s%s%s%s%s%s%s%s%s%s%s%s%s",
          TL_STRING_UP          ,
          TL_STRING_DOWN        ,
          TL_STRING_LEFT        ,
          TL_STRING_RIGHT       ,
          TL_STRING_LOWBATT     ,
          TL_STRING_COPYRIGHT   ,
          TL_STRING_WSA0        ,
          TL_STRING_WSB0        ,
          TL_STRING_WSA1        ,
          TL_STRING_WSB3        ,
          TL_STRING_WSB0        ,
          TL_STRING_WSB1        ,
          TL_STRING_WSB2);
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("backlight off (via property)\n");
   waitForLCDPtr();
   lcd->SetProperty(TL_PROP_BACKLIGHT, false);
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("backlight on (via property)\n");
   waitForLCDPtr();
   lcd->SetProperty(TL_PROP_BACKLIGHT, true);
   releaseLCDPtrIfRequested();
   sleep(1);

   printf("Clearing LCD and quitting\n");
   waitForLCDPtr();
   lcd->Clear();
   releaseLCDPtrIfRequested();

   printf("Done!\n");
   }

void writeChar(unsigned int row, unsigned int col, const char c)
   {
   waitForLCDPtr();
   lcd->MoveCursor(row, col);
   lcd->SetCharacter(c);
   releaseLCDPtrIfRequested();
   sleep(1);
   }

void test1()
   {
   printf("Test 1\n");

   writeChar(0,0,'A');
   writeChar(0,1,'B');
   writeChar(0,2,'C');
   writeChar(0,3,'D');
   writeChar(0,4,'E');
   writeChar(0,5,'F');
   writeChar(0,6,'G');
   writeChar(0,7,'H');
   writeChar(0,8,'I');
   writeChar(0,9,'J');
   writeChar(0,10,'K');
   writeChar(0,11,'L');
   writeChar(0,12,'M');
   writeChar(0,13,'N');
   writeChar(0,14,'O');
   writeChar(0,15,'P');
   }

void test2()
   {
   printf("Test 2\n");

   writeChar(1,0,'0');
   writeChar(1,1,'1');
   writeChar(1,2,'2');
   writeChar(1,3,'3');
   writeChar(1,4,'4');
   writeChar(1,5,'5');
   writeChar(1,6,'6');
   writeChar(1,7,'7');
   writeChar(1,8,'8');
   writeChar(1,9,'9');
   writeChar(1,10,'0');
   writeChar(1,11,'1');
   writeChar(1,12,'2');
   writeChar(1,13,'3');
   writeChar(1,14,'4');
   writeChar(1,15,'5');
   }

int main(int argc, char **argv)
   {
   if (argc > 1)
      {
      int testNum = atoi(argv[1]);
      if (testNum == 1)
         {
         test1();
         }
      else if (testNum == 2)
         {
         test2();
         }
      else
         {
         defaultTest();
         }
      }
   else
      {
      defaultTest();
      }

   return 0;
   }

