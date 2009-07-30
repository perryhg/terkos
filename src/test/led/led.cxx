#include <stdio.h>
#include "qeleds.h"

int main()
{
  CQELEDController led;

  led.SetLED(LED_ROBOT, LED_COLOR_YELLOW);
  led.SetLED(LED_VEXNET, LED_COLOR_RED);
  led.SetLED(LED_GAME, LED_COLOR_GREEN);
}
 
  
