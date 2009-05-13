#include <stdio.h>
#include "qegpioint.h"

int main()
{
  CQEGpioInt gpio;
  
  *gpio.DataDir() = 0x0000;
  *gpio.Data() = 0x00aa;

  while(1)
    printf("%x\n", *gpio.Data());

}
