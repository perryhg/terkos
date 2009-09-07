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
    case QP_PROP_HARDWARE_VERSION:
      *value = m_version;
      break;
      
    case QP_PROP_BREAKER_STATE:
      *value = GetBreakerState();
      break;

    case QP_PROP_MAIN_BATT_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(24);
      break;

    case QP_PROP_MAIN_BATT_STATE:
      *value = GetBattState();
      break;

    case  QP_PROP_BACKUP_BATT_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(21);
      break;

    case QP_PROP_BACKUP_BATT_STATE:
     // todo add state read
     *value = 0;
      break;

    case QP_PROP_MAIN_5V_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(23);
      break;

    case QP_PROP_IO_5V_VOLTAGE:
      *value = m_pAnalog->GetADVoltage(22);
      break;

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

