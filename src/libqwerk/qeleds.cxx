#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#include "qeleds.h"


SINGLETON_REGISTER(CQELEDController);

void *CQELEDController::LEDControllerThread(void *arg)
{
  CQELEDController *ledctl = (CQELEDController *)arg;

  return NULL;
}

CQELEDController::CQELEDController()
{
  m_p9302hw = C9302Hardware::GetPtr();

  if (m_p9302hw->GetBitstreamMajorVersion()!=0xa0)
    throw std::runtime_error("wrong FPGA bitstream version");

  m_ledreg = m_p9302hw->m_fpga.Ushort(QEL_DEFAULT_ADDR);
 
#if 0
  pthread_mutex_init(&m_mutex, NULL);
  pthread_cond_init(&m_cond, NULL);

  m_threadRun = true;

  pthread_create(&m_thread, NULL, LEDControllerThread, (void *)this);
#endif
}

CQELEDController::~CQELEDController()
{

#if 0
  m_threadRun = false;

  // signal thread 
  pthread_mutex_lock(&m_mutex);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);

  // wait for it to exit
  pthread_join(m_thread, NULL);

  pthread_mutex_destroy(&m_mutex);
  pthread_cond_destroy(&m_cond);
#endif

  C9302Hardware::Release();
}
  
void CQELEDController::SetLED(ELEDIndex led, ELEDColor color)
{
  unsigned int ledIndex, colorIndex;
  static const unsigned short colorTab[] = {
    0x0000, QEL_RED, QEL_GREEN, QEL_YELLOW, QEL_ORANGE
  };

  ledIndex = (unsigned int)led;
  colorIndex = (unsigned int)color;

  if (ledIndex>=QEL_NUM_LEDS || colorIndex>=QEL_NUM_COLORS)
    return;

  m_ledreg[ledIndex] = colorTab[colorIndex];
}

