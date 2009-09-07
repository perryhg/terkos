#ifndef _QEPOWER_H
#define _QEPOWER_H

#include "9302hw.h"
#include "qeanalog.h"
#include "property.h"
#include "singleton.h"

#define QP_OBJECT                    2
#define QP_PROP_HARDWARE_VERSION     PROP_ID(QP_OBJECT, 0)
#define QP_PROP_BREAKER_STATE        PROP_ID(QP_OBJECT, 1)
#define QP_PROP_MAIN_BATT_VOLTAGE    PROP_ID(QP_OBJECT, 2)
#define QP_PROP_MAIN_BATT_STATE      PROP_ID(QP_OBJECT, 3)
#define QP_PROP_BACKUP_BATT_VOLTAGE  PROP_ID(QP_OBJECT, 4)
#define QP_PROP_BACKUP_BATT_STATE    PROP_ID(QP_OBJECT, 5)
#define QP_PROP_MAIN_5V_VOLTAGE      PROP_ID(QP_OBJECT, 6)
#define QP_PROP_IO_5V_VOLTAGE        PROP_ID(QP_OBJECT, 7)
#define QP_PROP_IO_5V_STATE          PROP_ID(QP_OBJECT, 8)

class CQEPower : public IProperty
  
{
public:
  SINGLETON(CQEPower);
  
  void  PowerOff();

  // IProperty
  virtual int GetProperty(int property, long *value);
  virtual int SetProperty(int property, long value);

private:
  CQEPower();
  ~CQEPower();

  unsigned short GetBreakerState();
  unsigned int GetBattState();

  CQEAnalog *m_pAnalog;
  C9302Hardware *m_p9302hw;
  unsigned int m_version;
};  


#endif
