#ifndef _QELEDS_H
#define _QELEDS_H

#include <sys/time.h>

#include "qwerkhw.h"

// defines the number of LEDs supported by revision 1 Qwerk
#define QEL_REV1_NUM_LEDS	8

// defines the maximum number of LEDs a qwerk may support.  The actual number
// supported may be smaller, and should be obtained via CQELEDController::GetLEDCount()
#define QEL_MAX_NUM_LEDS	10

typedef enum {
  LEDEVENT_ON = 0,
  LEDEVENT_OFF,
} ledevent_type_t;

typedef struct ledevent_t {
  unsigned int led;
  ledevent_type_t type;
  unsigned int base_time;
  unsigned int time;
  ledevent_t *next;
} ledevent_t;

typedef struct leddata_t {
  int flashing;
  unsigned int period;
  ledevent_t start;
  ledevent_t end;
} leddata_t;

class CQwerkHardware;

class CQELEDController
{
 public:
  CQELEDController(CQwerkHardware *pQwerk);
  ~CQELEDController();

  /**
   * Causes an LED to turn on
   *
   * @param led LED to affect
   */
  void LEDon(unsigned int led);

  /**
   * Causes an LED to turn off
   *
   * @param led LED to affect
   */
  void LEDoff(unsigned int led);

  /**
   * Causes an LED to blink with 50% duty cycle, 1 second period, no offset
   *
   * @param led LED to blink
   */
  void LEDblink(unsigned int led);

  /**
   * Causes an LED to blink with specified parameters
   *
   * @param led LED to blink
   * @param period Period (in ms) of the blinking
   * @param duty_cycle Fraction (0..100) of the period in which LED is on
   * @param offset Offset (in ms) into the period before turning on LED
   */
  void LEDset(unsigned int led, unsigned int period, unsigned int duty_cycle, unsigned int offset);

  /**
   * Returns the number of LEDs supported by this qwerk.
   */
  unsigned int GetLEDCount();

 protected:
  CQwerkHardware *m_pQwerk; 
  bool m_allocated;

  pthread_t m_thread;
  bool m_threadRun;

  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;

  leddata_t m_ledState[QEL_MAX_NUM_LEDS];

  struct timeval m_eventStart;
  ledevent_t *m_eventlist;

  void eventlist_reset(void);
  void eventlist_insert(ledevent_t *event);

  static void *LEDControllerThread(void *arg);
};

#endif

