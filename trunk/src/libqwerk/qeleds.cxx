#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#include "qeleds.h"
#include "qwerkhw.h"
#include "timeutil.h"
#include "util.h"

void *CQELEDController::LEDControllerThread(void *arg)
{
  CQELEDController *ledctl = (CQELEDController *)arg;
  struct timeval currtimeval;
  unsigned int currtime;
  ledevent_t *currevent;

  while (1) {
    pthread_mutex_lock(&ledctl->m_mutex);
    while (ledctl->m_eventlist == NULL && ledctl->m_threadRun) {
      pthread_cond_wait(&ledctl->m_cond, &ledctl->m_mutex);
    }
    if (!ledctl->m_threadRun) {
      pthread_mutex_unlock(&ledctl->m_mutex);
      break;
    }
    gettimeofday(&currtimeval, NULL);
    TIMEVAL_SUB(&currtimeval, &currtimeval, &ledctl->m_eventStart);
    currtime = 1000*currtimeval.tv_sec + currtimeval.tv_usec / 1000;
//    printf("currtime = %d\n", currtime);
    while (1) {
      currevent = ledctl->m_eventlist;
      if (currevent == NULL || currevent->time > currtime)
        break;

      ledctl->m_eventlist = ledctl->m_eventlist->next;

//      printf("Processing %d: %d %s\n", currevent->time, currevent->led, (currevent->type == LEDEVENT_ON ? "on" : "off"));
      if (currevent->type == LEDEVENT_ON)
        // todo ledctl->m_pQwerk->SetLED(currevent->led, 1);
	;
      else if (currevent->type == LEDEVENT_OFF)
        // todo ledctl->m_pQwerk->SetLED(currevent->led, 0);
	;
      currevent->time += ledctl->m_ledState[currevent->led].period;
      ledctl->eventlist_insert(currevent);
    }
    pthread_mutex_unlock(&ledctl->m_mutex);
    usleep(10000); //be nice to other processes
  }

  return NULL;
}

CQELEDController::CQELEDController()
{
  int i;

  m_pQwerk = CQwerkHardware::GetObject();

  pthread_mutex_init(&m_mutex, NULL);
  pthread_cond_init(&m_cond, NULL);

  for (i = 0; i < QEL_MAX_NUM_LEDS; i++) {
    m_ledState[i].flashing = 0;
    m_ledState[i].period = 0;
    m_ledState[i].start.led = i;
    m_ledState[i].start.type = LEDEVENT_ON;
    m_ledState[i].start.time = 0;
    m_ledState[i].start.next = NULL;
    m_ledState[i].end.led = i;
    m_ledState[i].end.type = LEDEVENT_OFF;
    m_ledState[i].end.time = 0;
    m_ledState[i].end.next = NULL;
  }

  m_eventlist = NULL;

  gettimeofday(&m_eventStart, NULL);

  m_threadRun = true;

  pthread_create(&m_thread, NULL, LEDControllerThread, (void *)this);
}

CQELEDController::~CQELEDController()
{
  m_threadRun = false;

  // signal thread 
  pthread_mutex_lock(&m_mutex);
  pthread_cond_signal(&m_cond);
  pthread_mutex_unlock(&m_mutex);

  // wait for it to exit
  pthread_join(m_thread, NULL);

  pthread_mutex_destroy(&m_mutex);
  pthread_cond_destroy(&m_cond);

  CQwerkHardware::ReleaseObject();
}

void CQELEDController::LEDon(unsigned int led)
{
  if (led >= GetLEDCount())
    return;
  
  LEDset(led, 1000, 100, 0);
}

void CQELEDController::LEDoff(unsigned int led)
{
  if (led >= GetLEDCount())
    return;

  LEDset(led, 1000, 0, 0);
}

void CQELEDController::LEDblink(unsigned int led)
{
  if (led >= GetLEDCount())
    return;

  LEDset(led, 1000, 50, 0);
}

/**
 * Causes an LED to blink with specified parameters
 *
 * @param led LED to blink
 * @param period Period (in ms) of the blinking
 * @param duty_cycle Fraction (0..100) of the period in which LED is on
 * @param offset Offset (in ms) into the period before turning on LED
 */
void CQELEDController::LEDset(unsigned int led, unsigned int period, unsigned int duty_cycle, unsigned int offset)
{
  unsigned int start_time, on_time, end_time;
  leddata_t *ledstate;

  if (led >= GetLEDCount())
    return;

  //clip duty cycle at 100%
  if (duty_cycle > 100)
    duty_cycle = 100;

  //if bad offset (larger than period), make LED always off
  if (offset > period) {
    offset = 0;
    duty_cycle = 0;
  }

  start_time = offset;
  on_time = duty_cycle*period/100;
  on_time = MIN(on_time,period-start_time);
  end_time = start_time+on_time;

  pthread_mutex_lock(&m_mutex);

  ledstate = &m_ledState[led];
/*
  printf("start_time = %d\n", start_time);
  printf("on_time = %d\n", on_time);
  printf("end_time = %d\n", end_time);
*/

  if (on_time == 0) {
    //constant off
    // todo m_pQwerk->SetLED(led, 0);
    ledstate->flashing = 0;
    ledstate->period = 0;
  }
  else if (on_time == period) {
    //constant on
    // todo m_pQwerk->SetLED(led, 1);
    ledstate->flashing = 0;
    ledstate->period = 0;
  }
  else {
    //flashing
    ledstate->period = period;
    ledstate->start.time = ledstate->start.base_time = start_time;
    ledstate->end.time = ledstate->end.base_time = end_time;
    ledstate->flashing = 1;
  }

  eventlist_reset();

  gettimeofday(&m_eventStart, NULL);
/*
  printf("Events:\n");
  for (currevent = m_eventlist; currevent != NULL; currevent = currevent->next) {
    printf("%d: %d %s\n", currevent->time, currevent->led, (currevent->type == LEDEVENT_ON ? "on" : "off"));
  }
*/
  pthread_cond_signal(&m_cond);

  pthread_mutex_unlock(&m_mutex);
}

unsigned int CQELEDController::GetLEDCount()
{
  // todo 
  return 6; 
}

//mutex must be held when called; does not lock or unlock
void CQELEDController::eventlist_reset(void)
{
  unsigned int led;
  leddata_t *ledstate;
  ledevent_t *currevent;

  while (m_eventlist != NULL) {
    currevent = m_eventlist;
    m_eventlist = currevent->next;
    currevent->next = NULL;
  }

  for (led = 0; led < QEL_MAX_NUM_LEDS; led++) {
    ledstate = &m_ledState[led];

    ledstate->start.time = ledstate->start.base_time;
    ledstate->end.time = ledstate->end.base_time;

    if (!ledstate->flashing)
      continue;

    eventlist_insert(&ledstate->start);
    eventlist_insert(&ledstate->end);
  }
}

//mutex must be held when called; does not lock or unlock
void CQELEDController::eventlist_insert(ledevent_t *event)
{
  ledevent_t *currevent;

  if (m_eventlist == NULL || m_eventlist->time > event->time) {
    event->next = m_eventlist;
    m_eventlist = event;
  }
  else {
    for (currevent = m_eventlist; currevent->next != NULL; currevent = currevent->next) {
      if (currevent->next->time > event->time)
        break;
    }

    event->next = currevent->next;
    currevent->next = event;
  }
}

