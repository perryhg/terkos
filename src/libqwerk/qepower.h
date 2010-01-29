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

/**
 * CQEPower is used to get power information such as battery
 * voltage, battery status, and to initiate a system power-down.  
 * To instantiate this class: 
 * \code
 * // by pointer
 * CQEPower *ppwr = CQEPower::GetPtr(); 
 * // or by reference
 * CQEPower &pwr = CQEPower::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQEPower::Release();
 * \endcode
 */
class CQEPower : public IProperty  
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */ 
  SINGLETON(CQEPower);

  /**
   * Initiates a system power down.  It does a sync of the flash disk before
   * shutting down, so no information is lost. 
   */  
  void  PowerOff();

  /**
   * See the definition of IProperty::GetProperty for more information 
   * on this method.  The following properties are supported:
   */
  virtual int GetProperty(int property, long *value);

  /**
   * See the definition of IProperty::SetProperty for more information 
   * on this method.  The following properties re supported:
   */
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
