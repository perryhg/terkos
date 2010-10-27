//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

#include <stdio.h>
#include "qeservo.h"

int main()
{

  int i;
  volatile unsigned int d;
  CQEServo &servo = CQEServo::GetRef();

  while(1)
    {
      for (i=0; i<12; i++)
	servo.SetCommand(i, 0);
      printf("Pos: %d\n", servo.GetCommand(0));
      for (d=0; d<30000000; d++);

      // move middle
      for (i=0; i<12; i++)
	servo.SetCommand(i, 125);
      printf("Pos: %d\n", servo.GetCommand(0));
      for (d=0; d<30000000; d++);

      // move maximum clockwise
      for (i=0; i<12; i++)
	servo.SetCommand(i, 250);
      printf("Pos: %d\n", servo.GetCommand(0));
      for (d=0; d<30000000; d++);
    }

  CQEServo::Release();
}
