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
