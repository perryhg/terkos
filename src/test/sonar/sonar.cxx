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

#include "sonar.h"


static unsigned long diff(struct timeval *ptv0, struct timeval *ptv1)
{
  long val;

  val = ptv1->tv_usec - ptv0->tv_usec;
  val += (ptv1->tv_sec - ptv0->tv_sec)*1000000;

  return val;
}

CSonar::CSonar(unsigned int inputPort, unsigned int outputPort)
{
  m_triggerPort = inputPort;
  m_echoPort = outputPort;
  m_callback = NULL;
  m_flag = false;
  m_val = -1;
  m_valid = false;
  m_pgpio = CQEGpioInt::GetPtr();
  m_pgpio->ResetDataBit(m_triggerPort);
  // set trigger port as output
  m_pgpio->SetDataDirection((1<<m_triggerPort) | m_pgpio->GetDataDirection());
  // set trigger port as input
  m_pgpio->SetDataDirection(~(1<<m_echoPort) & m_pgpio->GetDataDirection());

  // set callbacks and interrupt modes
  m_pgpio->RegisterCallback(m_triggerPort, (void *)this, Callback);
  m_pgpio->RegisterCallback(m_echoPort, (void *)this, Callback);
  m_pgpio->SetInterruptMode(m_triggerPort, QEG_INTERRUPT_NEGEDGE); 
  m_pgpio->SetInterruptMode(m_echoPort, QEG_INTERRUPT_NEGEDGE); 
}
 
CSonar::~CSonar()
{
  m_pgpio->UnregisterCallback(m_triggerPort);
  m_pgpio->UnregisterCallback(m_echoPort);
  m_pgpio->Release();
}

void CSonar::Callback(unsigned int io, struct timeval *ptv, void *userPointer)
{
  CSonar *psonar = (CSonar *)userPointer;
  if (io==psonar->m_triggerPort)
    {
      psonar->m_flag = true;
      psonar->m_tv0 = *ptv;
    }

  if (io==psonar->m_echoPort && psonar->m_flag)
    {
      psonar->m_val = (diff(&psonar->m_tv0, ptv)+SONAR_USPI/2-SONAR_BIAS)/SONAR_USPI;
      psonar->m_valid = true;
      if (psonar->m_callback)
	(*psonar->m_callback)(psonar->m_echoPort, psonar->m_val);
    } 
}

int CSonar::Fire(bool wait)
{
  volatile int d;

  // invalidate value
  m_valid = false;

  m_pgpio->SetDataBit(m_triggerPort);
  for (d=0; d<100; d++);
  m_pgpio->ResetDataBit(m_triggerPort);

  if (wait)
    {
      while(m_valid==false)
	  usleep(1);
      return m_val;
    }
  
  return m_val;
}

int CSonar::GetVal()
{
  return m_val;
}

int CSonar::RegisterCallback(void (*callback)(unsigned int inputPort, int val))
{
  m_callback = callback;
  return 0;
}

int CSonar::UnregisterCallback()
{
  m_callback = 0;
  return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "qegpioint.h"
#include "qeanalog.h"

#define USPI 150
#define BIAS 300


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
  volatile int d;

  pgpio->SetData(0x0001);
  for (d=0; d<100; d++);
  pgpio->SetData(0x0000);

}

// note: connector labeled "INPUT" goes to digital 1 (0) trigger, 
// connector labeled "OUTPUT" goes to digital 2 (1) echo.

void callback2(unsigned int out, int val)
{
  printf("callback %d %d\n", out, val);
}

int main()
{
#if 0
  pgpio = CQEGpioInt::GetPtr();
  volatile unsigned int d;

  pgpio->SetData(0x0000);
  pgpio->SetDataDirection(0x0001);

  pgpio->RegisterCallback(0, callback);
  pgpio->RegisterCallback(1, callback);
  pgpio->SetInterruptMode(0, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(1, QEG_INTERRUPT_NEGEDGE); 

  signal(SIGALRM, SigHandler);  
  ualarm(60000, 60000);

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

#if 1
  CSonar s0(2, 3);
  
  s0.RegisterCallback(callback2);
  printf("%d\n", s0.Fire(true));
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
