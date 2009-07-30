#include <stdio.h>
#include "qeservo.h"

int main()
{
  int i;
  volatile unsigned int d;
  CQEServo servo;

  while(1)
    {
      for (i=0; i<12; i++)
	servo.SetPosition(i, 0);
      printf("Pos: %d\n", servo.GetPosition(0));
      for (d=0; d<30000000; d++);

      // move middle
      for (i=0; i<12; i++)
	servo.SetPosition(i, 125);
      printf("Pos: %d\n", servo.GetPosition(0));
      for (d=0; d<30000000; d++);

      // move maximum clockwise
      for (i=0; i<12; i++)
	servo.SetPosition(i, 250);
      printf("Pos: %d\n", servo.GetPosition(0));
      for (d=0; d<30000000; d++);
    }
}
