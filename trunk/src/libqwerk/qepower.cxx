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

#include "qepower.h"

SINGLETON_REGISTER(CQEPower);

CQEPower::CQEPower()
{
  m_p9302hw = C9302Hardware::GetPtr();
  m_pAnalog = CQEAnalog::GetPtr();

  // todo: set version
  m_version = 0;
}

CQEPower::~CQEPower()
{
  C9302Hardware::Release();
  CQEAnalog::Release();
}

void CQEPower::PowerOff()
{
  // sync disk
  sync();
  // wait a nominal amount
  sleep(1);
   *m_p9302hw->PortCData() = 1; // turn off unit
}


int CQEPower::GetProperty(int property, long *value)
{
  if (value==NULL)
    return PROP_ERROR;

  switch(property)
    {
      /**
       * - QP_PROP_HARDWARE_VERSION=version of VEXPro hardware
       */
    case QP_PROP_HARDWARE_VERSION:
      *value = m_version;
      break;
      
      /**
       * - QP_PROP_BREAKER_STATE=bitmap of the 5 PSD fuses in the power system.
       * A 1 indicates that the fuse is tripped (open), a 0 indicates the fuse
       * is normal (closed).  
       *   - Bit 0=PSD state for motors 1 through 4
       *   - Bit 1=PSD state for motors 5 through 8
       *   - Bit 2=PSD state for motors 9 through 12
       *   - Bit 3=PSD state for motors 13 through 14
       *   - Bit 4=PSD state for motors 15 through 16
       *   .
       */
    case QP_PROP_BREAKER_STATE:
      *value = GetBreakerState();
      break;

      /**
       * - QP_PROP_MAIN_BATT_VOLTAGE=main battery voltage in millivolts.
       */
    case QP_PROP_MAIN_BATT_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(24);
      break;


      /**
       * - QP_PROP_MAIN_BATT_STATE=state of main battery.
       */
    case QP_PROP_MAIN_BATT_STATE:
      *value = GetBattState();
      break;

      /**
       * - QP_PROP_BACKUP_BATT_VOLTAGE=voltage of backup battery in millivolts.
       */
    case  QP_PROP_BACKUP_BATT_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(21);
      break;
  
      /**
       * - QP_PROP_BACKUP_BATT_STATE=state of backup battery.
       */
    case QP_PROP_BACKUP_BATT_STATE:
     // todo add state read
     *value = 0;
      break;

      /**
       * - QP_PROP_MAIN_5V_VOLTAGE=voltage of main 5V regulator in millivolts.
       */
    case QP_PROP_MAIN_5V_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(23);
      break;

      /**
       * - QP_PROP_IO_5V_VOLTAGE=voltage of I/O connector 5V regulator 
       * in millivolts.
       */
    case QP_PROP_IO_5V_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(22);
      break;

      /**
       * - QP_PROP_IO_5V_STATE=on/off state of 5V regulator for I/O connectors
       * (analog, digital, I2C), 
       * 1=on, 0=off.
       */
    case QP_PROP_IO_5V_STATE:
      *value =   *m_p9302hw->PortHData() & 0x0020 ? 1 : 0;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}

int CQEPower::SetProperty(int property, long value)
{
  switch (property)
    {
    case QP_PROP_HARDWARE_VERSION:
    case QP_PROP_BREAKER_STATE:
    case QP_PROP_MAIN_BATT_VOLTAGE:
    case QP_PROP_MAIN_BATT_STATE:
    case QP_PROP_BACKUP_BATT_VOLTAGE:
    case QP_PROP_BACKUP_BATT_STATE:
    case QP_PROP_MAIN_5V_VOLTAGE:
    case QP_PROP_IO_5V_VOLTAGE:
      return PROP_ERROR_READ_ONLY;
      
      /**
       * - QP_PROP_IO_5V_STATE=control the 5V regulator for I/O connectors
       * (analog, digital, I2C)
       *   - 1 (nonzero)=on default, 
       *   - 0=off or power saving mode
       * where all devices connected to I/O connectors receive no 5V power.
       *   .
       */ 
    case QP_PROP_IO_5V_STATE:
      if (value)
	*m_p9302hw->PortHData() |= 0x0020;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}

unsigned short CQEPower::GetBreakerState()
{
  unsigned short val;

  val = m_pAnalog->GetADVoltage(16) < 3000 ? 1 : 0;
  val |= m_pAnalog->GetADVoltage(17) < 3000 ? 2 : 0;
  val |= m_pAnalog->GetADVoltage(18) < 3000 ? 4 : 0;
  val |= m_pAnalog->GetADVoltage(19) < 3000 ? 8 : 0;
  val |= m_pAnalog->GetADVoltage(20) < 3000 ? 16 : 0;

  return val;
}

unsigned int CQEPower::GetBattState()
{
  // todo put logic here to detect low battery state using hysteresis, etc
  return 0;
}

