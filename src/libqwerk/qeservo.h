#ifndef _QESERVO_H
#define _QESERVO_H

#if Q1
#define QES_DEFAULT_SERVOS       16
#else
#define QES_DEFAULT_SERVOS       12
#endif

#define QES_DEFAULT_DIVIDER      400  // divide 100mhz by 400->250khz, 4us 
#define QES_DEFAULT_FREQ         4650 // 4650 * 4us = 18.6ms->53.76hz
#define QES_DEFAULT_MIN          250  // 250*4us = 1ms
#define QES_DEFAULT_MAX          500  // 500*4us = 2ms

#define QES_DEFAULT_ADDR         0x400

class CQwerkHardware;

class CQEServo
{
public:
  CQEServo(unsigned int num=QES_DEFAULT_SERVOS, unsigned long addr=QES_DEFAULT_ADDR);
  ~CQEServo();

  void Disable(unsigned int index);
  unsigned short GetPosition(unsigned int index);
  void SetPosition(unsigned int index, unsigned short pos);
  
  // shouldn't be necessary to call SetTiming
  void SetTiming(unsigned short min, unsigned short max);

private:
  unsigned short m_min;
  unsigned short m_max;
  unsigned int m_num;

  volatile unsigned short *m_divider;
  volatile unsigned short *m_freq;
  volatile unsigned short *m_up;

  CQwerkHardware *m_pQwerk; 
};

#endif
