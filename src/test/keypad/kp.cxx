#include <unistd.h>
#include <stdio.h>
#include "keypad.h"

int main(int argc, char **argv)
{
  CKeypad kp;

#if 1
  while(1)
    printf("%x\n", kp.GetKey());
#endif
#if 0
  while(1)
    {
      if (kp.KeyDown())
	printf("down\n");
      if (kp.KeyUp())
	printf("up\n");
      if (kp.KeyLeft())
	printf("left\n");
      if (kp.KeyRight())
	printf("right\n");
      if (kp.KeyOk())
	printf("ok\n");
      if (kp.KeyCancel())
	printf("cancel\n");
    }
#endif
}

