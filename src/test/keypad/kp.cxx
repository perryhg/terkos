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

#include <unistd.h>
#include <stdio.h>
#include "keypad.h"

int main(int argc, char **argv)
{
  CKeypad &kp = CKeypad::GetRef();

  while(1)
  {
#if 1
    printf("%x\n", kp.GetKey());
#endif
#if 0
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
#endif
    usleep(33333);   // roughly 30 Hz
  }

  CKeypad::Release();
}

