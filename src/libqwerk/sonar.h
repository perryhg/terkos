#ifndef _SONAR_H
#define _SONAR_H
#include "qegpioint.h"

#define SONAR_USPI 150
#define SONAR_BIAS 300

/**
 * CSonar is used to control VEX ultrasonic range modules,
 * catalog number 276-2155.  Up
 * to 8 modules can be used with the VEXPro controller. See CSonarController
 * for methods that control the automated firing of multiple sonar 
 * modules (sonar arrays).
 */
class CSonar
{
public:
  /**
   * Constructor for a sonar module.
   * @param inputPort the digital port number of the "INPUT"
   * connector of the sonar module.  Can be 0 through 15 corresponding 
   * to digital connector 1
   * through 16, respectively.
   * @param outputPort the digital port number of the "OUTPUT"
   * connector of the sonar module.  Can be 0 through 15 corresponding 
   * to digital connector 1
   * through 16, respectively.
   */
  CSonar(unsigned int inputPort, unsigned int outputPort);
  ~CSonar();

  /**
   * Fire the sonar "chirp".
   * @param wait if false (default), this method will not wait until the echo 
   * is received.  If true, this method will wait until the echo is received
   * before returning.
   * @return 0 if not waiting for echo.  Returns distance value in inches
   * if waiting.
   * for echo.
   */
  int Fire(bool wait=false);
  
  /**
   * Get the distnce value in inches. 
   */ 
  int GetVal();

  /**
   * Set a callback routine that get called when an echo is received.
   * @param callback the routine you wish to be called.  This routine returns
   * the inputPort number -- the same number passed to the constructor -- and
   * the distance value in inches. 
   * @return 0 if success -1 if error. 
   */
  int RegisterCallback(void (*callback)(unsigned int inputPort, int val));
  
  /**
   * Remove the callback routine.  After calling this method, the callback
   * routine set from RegisterCallback will not be called again. 
   * @return 0 if success -1 if error. 
   */ 
  int UnregisterCallback();

private:
  static void Callback(unsigned int io, struct timeval *ptv, void *userPointer);

  struct timeval m_tv0;
  void (*m_callback)(unsigned int outputPort, int val);
  unsigned int m_echoPort;
  unsigned int m_triggerPort;
  bool m_flag;
  bool m_valid;
  int m_val;

  CQEGpioInt *m_pgpio;
};

#endif
