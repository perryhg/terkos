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

/**
 * The three LED's on the VEXPro.
 */
typedef enum
  {
    LED_ROBOT = 0,
    LED_VEXNET,
    LED_GAME
  }
ELEDIndex;

/**
 * The possible colors of each of the 3 LEDs on the VEXPro.
 */
typedef enum 
  {
    LED_COLOR_OFF = 0,
    LED_COLOR_RED ,
    LED_COLOR_GREEN,
    LED_COLOR_YELLOW,
    LED_COLOR_ORANGE
  } 
ELEDColor;

/** 
 * The possible blinking modes of the 3 LEDs on the VEXPro.  These modes
 * are not supported as of this writing.
 */
typedef enum
  {
    LED_SOLID,
    LED_SLOW_BLINK,
    LED_FAST_BLINK,
    LED_1BLIP,
    LED_2BLIP
  } 
ELEDMode;

/**
 * CQELEDController controls the 3 tri-color LEDs on the VEXPro.
 * To instantiate this class: 
 * \code
 * // by pointer
 * CQELEDController *pled = CQELEDController::GetPtr(); 
 * // or by reference
 * CQELEDController &led = CQELEDController::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQELEDController::Release();
 * \endcode
 */
class CQELEDController
{
public:
  /** 
   * This internal macro handles the instantiation of this class.
   */
  SINGLETON(CQELEDController);
  
  /**
   * Set the color of the LED.
   * @param led the LED you wish to set.
   * @param color the color you wish to set it to.
   */
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

