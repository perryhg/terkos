
#include <unistd.h>
#include <stdio.h>
#include <string>
#include "textlcd.h"

using namespace std;

int main(int argc, char **argv)
   {
   CTextLcd &lcd = CTextLcd::GetRef();

   long width = -1;
   long height = -1;
   lcd.GetProperty(TL_PROP_HEIGHT, &height);
   lcd.GetProperty(TL_PROP_WIDTH, &width);
   printf("Dimensions (via properties): (%d,%d)\n", width, height);
   printf("Dimensions (via consts):     (%d,%d)\n", CTextLcd::NUM_COLUMNS, CTextLcd::NUM_ROWS);

   printf("Clearing LCD\n");
   lcd.Clear();
   sleep(1);

   printf("Setting text ''\n");
   lcd.MoveCursor(0,0);
   lcd.printf("");
   sleep(1);

   printf("Setting text 'Hello there'\n");
   lcd.MoveCursor(0,0);
   lcd.printf("Hello there");
   sleep(1);

   printf("Setting text 'Hello there.    How are you?'\n");
   lcd.MoveCursor(0,0);
   lcd.printf("Hello there.    How are you?");
   sleep(1);

   printf("Setting text 'This is some really long text that will not fit'\n");
   lcd.MoveCursor(0,0);
   lcd.printf("This is some really long text that will not fit.");
   sleep(1);

   printf("Special characters\n");
   lcd.Clear();
   lcd.printf("Special chars:");
   lcd.MoveCursor(1,0);
   lcd.printf("%s%s%s%s%s%s%s%s%s%s%s%s%s",
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
   sleep(1);

   printf("backlight off (via property)\n");
   lcd.SetProperty(TL_PROP_BACKLIGHT, false);
   sleep(1);

   printf("backlight on (via property)\n");
   lcd.SetProperty(TL_PROP_BACKLIGHT, true);
   sleep(1);

   printf("Clearing LCD and quitting\n");
   lcd.Clear();

   printf("Releasing the LCD reference\n");
   CTextLcd::Release();

   printf("Done!\n");

   return 0;
   }

