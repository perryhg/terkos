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
      lcd.SetBacklight(true);
      sleep(1);
      printf("backlight off\n");
      lcd.SetBacklight(false);
      sleep(1);
    }
#endif
#if 1
  lcd.SetBacklight(true);
  lcd.printf("Hello world!");
#endif
}

