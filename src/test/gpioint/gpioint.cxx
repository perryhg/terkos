#include <stdio.h>
#include "qegpioint.h"

int main()
{
  CQEGpioInt *pgpio = CQEGpioInt::GetPtr();
  
  *pgpio->DataDir() = 0x0000;
  *pgpio->Data() = 0x00aa;

  while(1)
    printf("%x\n", *pgpio->Data());

}
