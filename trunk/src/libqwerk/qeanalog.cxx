#include "qeanalog.h"

SINGLETON_REGISTER(CQEAnalog);

// todo current into the processor is not insignificant
// channel 21 and 24 have 
#if 0
const unsigned int CQEAnalog::m_num[25] = {
  6639,  6639,  6639,  6639,  6639,  6639,  6639,  6639,
  6639,  6639,  6639,  6639,  6639,  6639,  6639,  6639,
  39441, 39441, 39441, 39441, 39441, 50394, 14904, 14904, 
  26384
};
#else
const unsigned int CQEAnalog::m_num[25] = {
  6639,  6639,  6639,  6639,  6639,  6639,  6639,  6639,
  6639,  6639,  6639,  6639,  6639,  6639,  6639,  6639,
  39441, 39441, 39441, 39441, 39441, 50394, 14034, 14034, 
  48098
};
#endif

CQEAnalog::CQEAnalog() 
{
  unsigned int val;

  m_p9302hw = C9302Hardware::GetPtr();

  // get a/d dc bias/offset
  val = m_p9302hw->GetAD(4);
  val += m_p9302hw->GetAD(4);
  val += m_p9302hw->GetAD(4);
  val += m_p9302hw->GetAD(4);
  m_adcOffset = val>>2;
}

CQEAnalog::~CQEAnalog()
{
  C9302Hardware::Release();
}

unsigned short CQEAnalog::GetADRaw(unsigned int channel)
{
  unsigned short val;

  if (channel>=QAD_AD_CHANNELS)
    return 0;
  
  // set mux
  *m_p9302hw->PortHDataDR() |= 0x001c;
  *m_p9302hw->PortHData() &= ~0x001c;
  *m_p9302hw->PortHData() |= (channel&7)<<2;

  if (channel<=7)
    val = m_p9302hw->GetAD(QAD_AD_ANALOG_IN_CHANNEL0_7);
  else if (channel>=8 && channel<=15)
    val = m_p9302hw->GetAD(QAD_AD_ANALOG_IN_CHANNEL8_15);
  else if (channel>=16 && channel<=23)
    val = m_p9302hw->GetAD(QAD_AD_ANALOG_IN_CHANNEL16_23);  
  else
    val = m_p9302hw->GetAD(QAD_AD_ANALOG_IN_CHANNEL24);  

  // subtract dc offset
  if (val>m_adcOffset)
    val -= m_adcOffset;
  else 
    val = 0;

  return val;
}


unsigned short CQEAnalog::GetADVoltage(unsigned int channel)
{
  unsigned int val;

  if (channel>=QAD_AD_CHANNELS)
    return 0;
  
  val = GetADRaw(channel);

  val *= m_num[channel];
  val >>= QAD_AD_SCALE;

  return (unsigned short)val;
}
 
int CQEAnalog::GetProperty(int property, long *value)
{
  return PROP_ERROR_NOT_SUPPORTED;
}

int CQEAnalog::SetProperty(int property, long value)
{
  return PROP_ERROR_NOT_SUPPORTED;
}
