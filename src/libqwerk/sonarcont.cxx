#include <unistd.h>
#include <signal.h>
#include "sonarcont.h"

unsigned int CSonarController::m_currentIndex;
unsigned int CSonarController::m_numSonar;
CSonar *CSonarController::m_sonarArray[QEG_NUM_IO];

CSonarController::CSonarController()
{
  SetSignal(true);
}

CSonarController::CSonarController(unsigned int numSonar, CSonar *sonarArray[], unsigned int fireIntervalMs)
{
  SetSignal(true);
  SetFiring(numSonar, sonarArray, fireIntervalMs);
}

CSonarController::~CSonarController()
{
  SetSignal(false);
  EndFiring();
}

void CSonarController::SetSignal(bool set)
{
  struct sigaction sa;

  if (set)
    {
      sa.sa_handler = SigHandler;
      sigemptyset(&sa.sa_mask);
      sigaddset(&sa.sa_mask, SIGIO);
      sa.sa_flags = 0;
      sigaction(SIGALRM, &sa, NULL);
    }
  else
    {
      sa.sa_handler = SIG_IGN;
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = 0;
      sigaction(SIGALRM, &sa, NULL);
    }
}

void CSonarController::SetAlarm(int intervalMs)
{
  struct itimerval itimer;
  
  itimer.it_interval.tv_sec=intervalMs/1000;
  itimer.it_interval.tv_usec=intervalMs%1000*1000;
  itimer.it_value.tv_sec=intervalMs/1000;
  itimer.it_value.tv_usec=intervalMs%1000*1000;
  setitimer(ITIMER_REAL, &itimer, NULL); 
}


int CSonarController::SetFiring(unsigned int numSonar, CSonar *sonarArray[], unsigned int fireIntervalMs)
{
  unsigned int i;

  if (fireIntervalMs<10)
    return -1;

  EndFiring();
  for (i=0; i<numSonar; i++)
    m_sonarArray[i] = sonarArray[i];

  m_numSonar = numSonar;
  m_currentIndex = 0;

  SetAlarm(fireIntervalMs);
  return 0;
}

int CSonarController::EndFiring()
{
  SetAlarm(0);
  return 0;
}

int CSonarController:: GetVals(int vals[])
{
  unsigned int i;

  for (i=0; i<m_numSonar; i++)
    vals[i] = m_sonarArray[i]->GetVal();

  return 0;
}

void CSonarController::SigHandler(int signum)
{
  m_sonarArray[m_currentIndex++]->Fire();
  if (m_currentIndex==m_numSonar)
    m_currentIndex = 0;
}

