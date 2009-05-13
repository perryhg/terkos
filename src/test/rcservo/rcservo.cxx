#include <stdio.h>
#include "rcservo.h"

int main()
{
  int i;
  volatile unsigned int d;
  CRCServo servo;

 for (i=0; i<16; i++)
    servo.SetBounds(i, 32, 228);

  while(1)
    {
      for (i=0; i<16; i++)
	servo.SetPosition(i, 0x00);
      printf("Pos: 0x%x\n", servo.GetPosition(0));
      for (d=0; d<30000000; d++);

      // move middle
      for (i=0; i<16; i++)
	servo.SetPosition(i, 0x80);
      printf("Pos: 0x%x\n", servo.GetPosition(0));
      for (d=0; d<30000000; d++);

      // move maximum clockwise
      for (i=0; i<16; i++)
	servo.SetPosition(i, 0xff);
      printf("Pos: 0x%x\n", servo.GetPosition(0));
      for (d=0; d<30000000; d++);
    }
}
