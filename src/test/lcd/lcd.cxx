//

#include <unistd.h>
#include <stdio.h>
#include "textlcd.h"

int main(int argc, char **argv)
{
  CTextLcd lcd;

#if 0
  while(1)
    {
      printf("backlight on\n");
      lcd.SetProperty(TL_PROP_BACKLIGHT, true);
      sleep(1);
      printf("backlight off\n");
      lcd.SetProperty(TL_PROP_BACKLIGHT, false);
      sleep(1);
    }
#endif
#if 1
  lcd.SetProperty(TL_PROP_BACKLIGHT, true);
  lcd.MoveCursor(0, 0);
  lcd.printf("Hello world %d", 1234);
  sleep(3);
  lcd.Clear();
  //  lcd.SetProperty(TL_PROP_BACKLIGHT, false);
#endif
}

