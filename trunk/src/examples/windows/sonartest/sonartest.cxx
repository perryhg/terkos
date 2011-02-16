#include <stdio.h>
#include <unistd.h>
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

void callback(unsigned int io, struct timeval *ptv, void *userdata)
{
  static struct timeval tv0;
  static int flag = 0;
  int sonarVal;

   if (io==0)
     {
       flag = 1;
       tv0 = *ptv;
     }

  if (io==1 && flag)
    {
	  sonarVal = diff(&tv0, ptv);
	  if (sonarVal>BIAS)
		  sonarVal = (sonarVal-BIAS)/USPI;
      printf("%d\n", sonarVal);
    }
}

// Note: connector labeled "INPUT" on sonar sensor goes to
// digital 1 (bit 0), and connector labeled "OUTPUT" goes to
// digital 2 (bit 1).
int main()
{
  CQEGpioInt &gpio = CQEGpioInt::GetRef();
  volatile unsigned int d;

  // reset bit 0, set as output for sonar trigger
  gpio.SetData(0x0000);
  gpio.SetDataDirection(0x0001);

  // set callbacks on negative edge for both bits 0 (trigger)
  // and 1 (echo)
  gpio.RegisterCallback(0, NULL, callback);
  gpio.RegisterCallback(1, NULL, callback);
  gpio.SetInterruptMode(0, QEG_INTERRUPT_NEGEDGE);
  gpio.SetInterruptMode(1, QEG_INTERRUPT_NEGEDGE);

  // trigger sonar by toggling bit 0
  while(1)
    {
      gpio.SetData(0x0001);
      for (d=0; d<120000; d++);
      gpio.SetData(0x0000);
      for (d=0; d<120000; d++);
    }

}


