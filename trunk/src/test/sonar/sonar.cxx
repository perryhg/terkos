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
#include <unistd.h>
#include <signal.h>
#include "qegpioint.h"

#define USPI 150
#define BIAS 300

unsigned long diff(struct timeval *ptv0, struct timeval *ptv1)
{
  long val;

  val = ptv1->tv_usec - ptv0->tv_usec;
  val += (ptv1->tv_sec - ptv0->tv_sec)*1000000;

  return val;
}

void callback(unsigned int io, struct timeval *ptv)
{
  static struct timeval tv0;
  static int flag = 0;

   if (io==0)
     {
       flag = 1;
       tv0 = *ptv;
     }

  if (io==1 && flag)
    {
      printf("%d\n", (diff(&tv0, ptv)+USPI/2-BIAS)/USPI);
    }
}

int main()
{
  CQEGpioInt *pgpio = CQEGpioInt::GetPtr();
  volatile unsigned int d;

  pgpio->SetData(0x0000);
  pgpio->SetDataDirection(0x0001);

  pgpio->RegisterCallback(0, callback);
  pgpio->RegisterCallback(1, callback);
  pgpio->SetInterruptMode(0, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(1, QEG_INTERRUPT_NEGEDGE); 

  while(1)
    {
      pgpio->SetData(0x0001);
      for (d=0; d<120000; d++);
      pgpio->SetData(0x0000);
      for (d=0; d<120000; d++);
    }
}
