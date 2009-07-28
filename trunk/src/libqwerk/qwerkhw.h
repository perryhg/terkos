#ifndef _QWERKHW_H
#define _QWERKHW_H

#include "9302hw.h"
#include "property.h"

#ifndef NULL
#define NULL               (void *)0
#endif

#define QHW_AD_CHANNELS               25
#define QHW_AD_ANALOG_IN_CHANNEL0_7   1
#define QHW_AD_ANALOG_IN_CHANNEL8_15  2
#define QHW_AD_ANALOG_IN_CHANNEL16_23 3
#define QHW_AD_ANALOG_IN_CHANNEL24    0

#define QHW_AD_SCALE                  16 // 2^13

#define QHW_OBJECT                    2
#define QHW_PROP_HARDWARE_VERSION     PROP_ID(QHW_OBJECT, 0)
#define QHW_PROP_BREAKER_STATE        PROP_ID(QHW_OBJECT, 1)
#define QHW_PROP_MAIN_BATT_VOLTAGE    PROP_ID(QHW_OBJECT, 2)
#define QHW_PROP_MAIN_BATT_STATE      PROP_ID(QHW_OBJECT, 3)
#define QHW_PROP_BACKUP_BATT_VOLTAGE  PROP_ID(QHW_OBJECT, 4)
#define QHW_PROP_BACKUP_BATT_STATE    PROP_ID(QHW_OBJECT, 5)
#define QHW_PROP_MAIN_5V_VOLTAGE      PROP_ID(QHW_OBJECT, 6)
#define QHW_PROP_IO_5V_VOLTAGE        PROP_ID(QHW_OBJECT, 7)
#define QHW_PROP_IO_5V_STATE          PROP_ID(QHW_OBJECT, 8)

class CQwerkHardware
{
public:
  CQwerkHardware();
  ~CQwerkHardware();

  static CQwerkHardware *GetObject();
  static void ReleaseObject();

  unsigned short GetADRaw(unsigned int channel);
  unsigned short GetADVoltage(unsigned int channel);

  void           PowerOff();

  // IProperty
  virtual int GetProperty(int property, long *value);
  virtual int SetProperty(int property, long value);
  
  // expose this so others can call on its members
  C9302Hardware *m_p9302hw;

private:
  
  unsigned short GetBreakerState();
  unsigned int GetBattState();

  unsigned int m_version;
  unsigned short m_adcOffset;

  static CQwerkHardware *m_pQwerk;
  static int m_refCount;
  static const unsigned int m_num[25];


};

#endif
