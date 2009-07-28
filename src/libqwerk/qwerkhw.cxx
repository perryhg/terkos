#include <stdio.h>
#include <unistd.h>
#include "qwerkhw.h"
#include "9302hw.h"

CQwerkHardware *CQwerkHardware::m_pQwerk = NULL;
int CQwerkHardware::m_refCount = 0;

// todo urrent into the processor is not insignificant
// channel 21 and 24 have 
#if 1
const unsigned int CQwerkHardware::m_num[25] = {
  6583,  6583,  6583,  6583,  6583,  6583,  6583,  6583,
  6583,  6583,  6583,  6583,  6583,  6583,  6583,  6583,
  39441, 39441, 39441, 39441, 39441, 50394, 14904, 14904, 
  26384
};
#else
const unsigned int CQwerkHardware::m_num[25] = {
  6583,  6583,  6583,  6583,  6583,  6583,  6583,  6583,
  6583,  6583,  6583,  6583,  6583,  6583,  6583,  6583,
  39441, 39441, 39441, 39441, 39441, 50394, 14034, 14034, 
  47665
};
#endif

CQwerkHardware::CQwerkHardware()
{
  unsigned int val;

  m_p9302hw = C9302Hardware::GetObject();

  // turn on I/O 5V regulator
  *m_p9302hw->PortHDataDR() |= 0x0020;  
  *m_p9302hw->PortHData() |= 0x0020;

  // get a/d dc bias/offset
  val = m_p9302hw->GetAD(4);
  val += m_p9302hw->GetAD(4);
  val += m_p9302hw->GetAD(4);
  val += m_p9302hw->GetAD(4);
  m_adcOffset = val>>2;

  // todo: set version
  m_version = 0;
}

CQwerkHardware::~CQwerkHardware()
{
  C9302Hardware::ReleaseObject();
}

CQwerkHardware *CQwerkHardware::GetObject()
{
  if (m_pQwerk==NULL)
    m_pQwerk = new CQwerkHardware();
  
  m_refCount++;
  return m_pQwerk;
}

void CQwerkHardware::ReleaseObject()
{
  if (m_refCount)
    m_refCount--;

  if (m_refCount==0 && m_pQwerk!=NULL)
    delete m_pQwerk;
}

unsigned short CQwerkHardware::GetADRaw(unsigned int channel)
{
  unsigned short val;

  if (channel>=QHW_AD_CHANNELS)
    return 0;
  
  // set mux
  *m_p9302hw->PortHDataDR() |= 0x001c;
  *m_p9302hw->PortHData() &= ~0x001c;
  *m_p9302hw->PortHData() |= (channel&7)<<2;

  if (channel<=7)
    val = m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL0_7);
  else if (channel>=8 && channel<=15)
    val = m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL8_15);
  else if (channel>=16 && channel<=23)
    val = m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL16_23);  
  else
    val = m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL24);  


  #if 1
  // subtract dc offset
  if (val>m_adcOffset)
    val -= m_adcOffset;
  else 
    val = 0;
  #endif
  return val;
}

unsigned short CQwerkHardware::GetADVoltage(unsigned int channel)
{
  unsigned int val;

  if (channel>=QHW_AD_CHANNELS)
    return 0;
  
  val = GetADRaw(channel);

  val *= m_num[channel];
  val >>= QHW_AD_SCALE;

  return (unsigned short)val;
}

void CQwerkHardware::PowerOff()
{
  // sync disk
  sync();
  // wait a nominal amount
  sleep(1);
   *m_p9302hw->PortCData() = 0; // turn off unit
}

int CQwerkHardware::GetProperty(int property, long *value)
{
  if (value==NULL)
    return PROP_ERROR;

  switch(property)
    {
    case QHW_PROP_HARDWARE_VERSION:
      *value = m_version;
      break;
      
    case QHW_PROP_BREAKER_STATE:
      *value = GetBreakerState();
      break;

    case QHW_PROP_MAIN_BATT_VOLTAGE:
      *value = GetADVoltage(24);
      break;

    case QHW_PROP_MAIN_BATT_STATE:
      *value = GetBattState();
      break;

    case  QHW_PROP_BACKUP_BATT_VOLTAGE:
      *value = GetADVoltage(21);
      break;

    case QHW_PROP_BACKUP_BATT_STATE:
     // todo add state read
     *value = 0;
      break;

    case QHW_PROP_MAIN_5V_VOLTAGE:
      *value = GetADVoltage(23);
      break;

    case QHW_PROP_IO_5V_VOLTAGE:
      *value = GetADVoltage(22);
      break;

    case QHW_PROP_IO_5V_STATE:
      *value =   *m_p9302hw->PortHData() & 0x0020 ? 1 : 0;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}

int CQwerkHardware::SetProperty(int property, long value)
{
  switch (property)
    {
    case QHW_PROP_HARDWARE_VERSION:
    case QHW_PROP_BREAKER_STATE:
    case QHW_PROP_MAIN_BATT_VOLTAGE:
    case QHW_PROP_MAIN_BATT_STATE:
    case QHW_PROP_BACKUP_BATT_VOLTAGE:
    case QHW_PROP_BACKUP_BATT_STATE:
    case QHW_PROP_MAIN_5V_VOLTAGE:
    case QHW_PROP_IO_5V_VOLTAGE:
      return PROP_ERROR_READ_ONLY;

    case QHW_PROP_IO_5V_STATE:
      if (value)
	*m_p9302hw->PortHData() |= 0x0020;
      break;

    default:
      return PROP_ERROR_NOT_SUPPORTED;
    }

  return PROP_OK;
}

unsigned short CQwerkHardware::GetBreakerState()
{
  unsigned short val;

  val = GetADVoltage(16) < 3000 ? 1 : 0;
  val |= GetADVoltage(17) < 3000 ? 2 : 0;
  val |= GetADVoltage(18) < 3000 ? 4 : 0;
  val |= GetADVoltage(19) < 3000 ? 8 : 0;
  val |= GetADVoltage(20) < 3000 ? 16 : 0;

  return val;
}

unsigned int CQwerkHardware::GetBattState()
{
  // todo put logic here to detect low battery state using hysteresis, etc
  return 0;
}
