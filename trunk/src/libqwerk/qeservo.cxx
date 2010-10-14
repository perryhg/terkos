//
// begin license header
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
// end license header
//

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include "qeservo.h"

SINGLETON_REGISTER(CQEServo);

CQEServo::CQEServo()
{
  long addr = QES_DEFAULT_ADDR;

  m_p9302hw = C9302Hardware::GetPtr();
  if (m_p9302hw->GetBitstreamMajorVersion()!=0xa0)
    throw std::runtime_error("wrong FPGA bitstream version");

  m_num = QES_DEFAULT_SERVOS;
  m_divider = m_p9302hw->m_fpga.Ushort(addr);
  m_freq = m_p9302hw->m_fpga.Ushort(addr+2);
  m_up = m_p9302hw->m_fpga.Ushort(addr+4);

  *m_divider = QES_DEFAULT_DIVIDER; 
  *m_freq = QES_DEFAULT_FREQ; 

  m_min = QES_DEFAULT_MIN;
  m_max = QES_DEFAULT_MAX;

  // register callback
  SetSignal(true);
}

CQEServo::~CQEServo()
{
  SetSignal(false);
  C9302Hardware::Release();
}

void CQEServo::SetTiming(unsigned short min, unsigned short max)
{
  m_min = min;
  m_max = max;
}

void CQEServo::Disable(unsigned int index)
{
  if (index>=m_num)
    return;

  m_up[index] = 0;
}

unsigned short CQEServo::GetCommand(unsigned int index)
{
  if (index>=m_num)
    return 0;

  return m_up[index] - m_min;
}

void CQEServo::SetCommand(unsigned int index, unsigned short pos)
{
  unsigned short val;

  if (index>=m_num)
    return;

  val = pos + m_min;

  if (val>m_max)
    val = m_max;

  m_up[index] = val;
}

void CQEServo::SigHandler(int signum)
{
  int i;
  CQEServo *pservo = CQEServo::GetPtr();

  // turn off all servos
  for (i=0; i<pservo->m_num; i++)
    pservo->Disable(i);

  pservo->SetSignal(false); 

  CQEServo::Release();

  // call default handler
  kill(getpid(), signum);
}

void CQEServo::SetSignal(bool set)
{
  struct sigaction sa;

  if (set)
    {
      sa.sa_handler = SigHandler;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = 0;
      sigaction(SIGINT, &sa, NULL);
      sigaction(SIGHUP, &sa, NULL);
      sigaction(SIGQUIT, &sa, NULL);
      sigaction(SIGSEGV, &sa, NULL);
      sigaction(SIGILL, &sa, NULL);
      sigaction(SIGFPE, &sa, NULL);
      sigaction(SIGTERM, &sa, NULL);
      sigaction(SIGABRT, &sa, NULL);
    }
  else
    {
      sa.sa_handler = SIG_DFL;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = 0;
      sigaction(SIGINT, &sa, NULL);
      sigaction(SIGHUP, &sa, NULL);
      sigaction(SIGQUIT, &sa, NULL);
      sigaction(SIGSEGV, &sa, NULL);
      sigaction(SIGILL, &sa, NULL);
      sigaction(SIGFPE, &sa, NULL);
      sigaction(SIGTERM, &sa, NULL);
      sigaction(SIGABRT, &sa, NULL);
    }
}



