#include <unistd.h>
#include <stdio.h>
#include "9302hw.h"
#include "qemotoruser.h"
#include "qemotortraj.h"
#include "textlcd.h"

int main(int argc, char **argv)
{
  CTextLcd &lcd = CTextLcd::GetRef();
  lcd.Clear();
  lcd.printf("LCD testLCD test");
  lcd.MoveCursor(1, 0);
  lcd.printf("LCD testLCD test");
  
  
#if 0
  CQEMotorTraj motor(atoi(argv[1]));
  motor.SetPIDVGains(0, 100, 0, 500, 0);
  printf("running\n");

  while(1)
    {
      motor.Move(0, 40000, 20000, 8000);
      while(!motor.Done(0)); 
      motor.Move(0, -40000, 20000, 8000);
      while(!motor.Done(0)); 
    }
#endif
}
