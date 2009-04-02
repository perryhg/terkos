#ifndef _QWERKHW_H
#define _QWERKHW_H

#include "9302hw.h"

#ifndef NULL
#define NULL               (void *)0
#endif

#define QHW_AD_CHANNELS               8
#define QHW_AD_ANALOG_IN_CHANNEL      0
#define QHW_AD_BATT_CHANNEL           1
#define QHW_AD_MOTOR_CHANNEL          2
#define QHW_AD_TEMP_CHANNEL           3
#define QHW_AD_5V_CHANNEL             4

#define QHW_AD_ANALOG_IN_NUMERATOR    1134
#define QHW_AD_ANALOG_IN_DENOMINATOR  10000
#define QHW_AD_BATT_NUMERATOR         7502
#define QHW_AD_BATT_DENOMINATOR       10000
#define QHW_AD_MOTOR_NUMERATOR        7502
#define QHW_AD_MOTOR_DENOMINATOR      10000
#define QHW_AD_TEMP_NUMERATOR         1
#define QHW_AD_TEMP_DENOMINATOR       1
#define QHW_AD_5V_NUMERATOR           1357
#define QHW_AD_5V_DENOMINATOR         10000

#define QHW_AUDIO_VOLUME              0xfe0
#define QHW_AUDIO_PERIOD              0xfe2

#define QHW_LED_ATTEN                 0xffff

enum EQHWDivScale
  {
    QHW_DIV_365TH, // 0.365
    QHW_DIV_108TH, // 0.108
    QHW_DIV_91TH   // 0.091
  };

class CQwerkHardware
{
public:
  CQwerkHardware(C9302Hardware *hw=NULL);
  ~CQwerkHardware();

  unsigned int   GetVersion();

  unsigned short GetADRaw(unsigned int channel);
  unsigned short GetADVoltage(unsigned int channel);
  unsigned short GetBattVoltage();
  unsigned short GetMotorVoltage();
  unsigned short GetTemp();
  unsigned short Get5VVoltage();

  void           SetPhyEnable(bool state);
  void           SetAudioEnable(bool state);
  void           SetMotorVoltageScale(EQHWDivScale scale);
  bool           GetConfigSwitch();
  void           SetLED(unsigned int led, bool state);
  void           SetLEDs(unsigned char stateMask);

  // expose this so others can call on its members
  C9302Hardware *m_p9302hw;

private:
  bool m_allocated;
};

#endif
