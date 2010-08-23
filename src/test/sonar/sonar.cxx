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
#include "qeanalog.h"

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

CQEGpioInt *pgpio;

void SigHandler(int signum)
{
  static int j = 0;

  if (j&1)
      pgpio->SetData(0x0000);
  else 
      pgpio->SetData(0x0001);

  j++;
}

// note: connector labeled "INPUT" goes to digital 1 (0), 
// connector labeled "OUTPUT" goes to digital 2 (1).
int main()
{
#if 1
  pgpio = CQEGpioInt::GetPtr();
  volatile unsigned int d;

  pgpio->SetData(0x0000);
  pgpio->SetDataDirection(0x0001);

  pgpio->RegisterCallback(0, callback);
  pgpio->RegisterCallback(1, callback);
  pgpio->SetInterruptMode(0, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(1, QEG_INTERRUPT_NEGEDGE); 

  signal(SIGALRM, SigHandler);  
  ualarm(30000, 30000);

  while(1)
    {
#if 1
      sleep(1);
#else
      pgpio->SetData(0x0001);
      for (d=0; d<500000; d++);
      pgpio->SetData(0x0000);
      for (d=0; d<500000; d++);
#endif
    }
#endif
#if 0 // line sensor
  CQEAnalog *pa = CQEAnalog::GetPtr();
  unsigned short val;

  while(1)
    {
      val = pa->GetADVoltage(0);
      if (val>1000)
	printf("line\n");
      else
	printf("no line\n");
    } 
#endif
#if 0 // light sensor
  CQEAnalog *pa = CQEAnalog::GetPtr();
  unsigned short val;
  // 1200-1300 dark, 600-60 light
  while(1)
    {
      val = pa->GetADVoltage(0);
      printf("%d\n", val);
    }
#endif
#if 0 // bumper
  CQEGpioInt *pgpio = CQEGpioInt::GetPtr();
  pgpio->SetDataDirection(0x0000);
  while(1)
    {
      printf("%x\n", pgpio->GetData()&0x0001^0x0001);
    }
#endif
}
