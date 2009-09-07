#ifndef _QELEDS_H
#define _QELEDS_H

#include <sys/time.h>
#include "9302hw.h"
#include "singleton.h"

// defines the number of LEDs
#define QEL_NUM_LEDS	         3
#define QEL_NUM_COLORS	         5

#define QEL_DEFAULT_ADDR         0x4c0

#define QEL_GREEN         0xff00
#define QEL_RED           0x00ff
#define QEL_YELLOW        0xff40
#define QEL_ORANGE        0xffff

typedef enum
  {
    LED_ROBOT = 0,
    LED_VEXNET,
    LED_GAME
  }
ELEDIndex;

typedef enum 
  {
    LED_COLOR_OFF = 0,
    LED_COLOR_RED ,
    LED_COLOR_GREEN,
    LED_COLOR_YELLOW,
    LED_COLOR_ORANGE
  } 
ELEDColor;

typedef enum
  {
    LED_SOLID,
    LED_SLOW_BLINK,
    LED_FAST_BLINK,
    LED_1BLIP,
    LED_2BLIP
  } 
ELEDMode;

class CQELEDController
{
public:
  SINGLETON(CQELEDController);
  
  void SetLED(ELEDIndex led, ELEDColor color);

private:
  CQELEDController();
  ~CQELEDController();

  C9302Hardware *m_p9302hw; 

  volatile unsigned short *m_ledreg;

  pthread_t m_thread;
  bool m_threadRun;
  pthread_mutex_t m_mutex;
  pthread_cond_t m_cond;

  static void *LEDControllerThread(void *arg);
};

#endif

