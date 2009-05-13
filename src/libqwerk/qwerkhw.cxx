#include <stdio.h>
#include <unistd.h>
#include "qwerkhw.h"
#include "9302hw.h"

CQwerkHardware::CQwerkHardware()
{
  m_p9302hw = C9302Hardware::GetObject();

  // set FPGA PROG to high and PHY power to high
  *m_p9302hw->PortAData() = 0x0008;
  *m_p9302hw->PortADataDR() = 0x006a;

#if Q1
  // turn LEDs off 
  *m_p9302hw->PortBData() = 0x0000;
  *m_p9302hw->PortBDataDR() = 0x00ff;
  *m_p9302hw->PortHData() &= ~0x0028;
  *m_p9302hw->PortHDataDR() |= 0x0028;
  *m_p9302hw->PortGData() |= 0x02;
  *m_p9302hw->PortGDataDR() |= 0x02;
 
  // disable audio - it enables automatically on first audio command
  SetAudioEnable(false);

  // set port H bits 4 as output (audio)
  *m_p9302hw->PortHDataDR() |= 0x00010;

  SetMotorVoltageScale(QHW_DIV_365TH);
#endif
}

CQwerkHardware::~CQwerkHardware()
{
  C9302Hardware::ReleaseObject();
}

unsigned short CQwerkHardware::GetADRaw(unsigned int channel)
{
  if (channel>=QHW_AD_CHANNELS)
    return 0;

  // set mux
#if Q1
  *m_p9302hw->PortFDataDR() = 0x000e;
  *m_p9302hw->PortFData() = channel << 1;

  return m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL);
#else
  *m_p9302hw->PortHDataDR() |= 0x001c;
  *m_p9302hw->PortHData() &= ~0x001c;
  *m_p9302hw->PortHData() |= (channel&7)<<2;

  if (channel<=7)
    return m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL0_7);
  else if (channel>=8 && channel<=15)
    return m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL8_15);
  else
    return m_p9302hw->GetAD(QHW_AD_ANALOG_IN_CHANNEL16_23);    
#endif
  
}

unsigned short CQwerkHardware::GetADVoltage(unsigned int channel)
{
  unsigned int val;

  if (channel>=QHW_AD_CHANNELS)
    return 0;
  
  val = GetADRaw(channel);

  val *= QHW_AD_ANALOG_IN_NUMERATOR;
  val /= QHW_AD_ANALOG_IN_DENOMINATOR;

  return (unsigned short)val;
}

unsigned short CQwerkHardware::GetBattVoltage()
{
  unsigned int val;

  val = m_p9302hw->GetAD(QHW_AD_BATT_CHANNEL);

  val *= QHW_AD_BATT_NUMERATOR;
  val /= QHW_AD_BATT_DENOMINATOR;

  return (unsigned short)val;
}

unsigned short CQwerkHardware::GetMotorVoltage()
{
#if Q1
  unsigned int val;

  val = m_p9302hw->GetAD(QHW_AD_MOTOR_CHANNEL);

  val *= QHW_AD_MOTOR_NUMERATOR;
  val /= QHW_AD_MOTOR_DENOMINATOR;

  return (unsigned short)val;
#else
  return 0;
#endif
}

unsigned short CQwerkHardware::GetTemp()
{
#if Q1
  unsigned int val;

  val = m_p9302hw->GetAD(QHW_AD_TEMP_CHANNEL);

  val *= QHW_AD_TEMP_NUMERATOR;
  val /= QHW_AD_TEMP_DENOMINATOR;

  return (unsigned short)val;
#else
  return 0;
#endif
}

unsigned short CQwerkHardware::Get5VVoltage()
{
  unsigned int val;

#if Q1
  val = m_p9302hw->GetAD(QHW_AD_5V_CHANNEL);

  val *= QHW_AD_5V_NUMERATOR;
  val /= QHW_AD_5V_DENOMINATOR;

  return (unsigned short)val;
#else
  return 0;
#endif
}

unsigned int CQwerkHardware::GetVersion()
{
  unsigned int version;

  *m_p9302hw->PortFDataDR() = 0x0000;
  version = *m_p9302hw->PortFData()&0x000e;

  version >>= 1;

  return version;
}

void CQwerkHardware::SetPhyEnable(bool state)
{
  *m_p9302hw->PortAData() = state ? *m_p9302hw->PortAData() |= 0x0008 :
    *m_p9302hw->PortAData() &= ~0x0008;
}

bool CQwerkHardware::GetConfigSwitch()
{
  return *m_p9302hw->PortAData()&0x0010 ? false : true;
}

void CQwerkHardware::SetLED(unsigned int led, bool state)
{
  unsigned int resetMask;
  unsigned int setMask;

  if (led==8)
    {
      if (state)
	*m_p9302hw->PortHData() |= 0x0008;
      else
	*m_p9302hw->PortHData() &= ~0x0008;
    }
  else if (led==9)
    {
      if (state)
	*m_p9302hw->PortHData() |= 0x0020;
      else
	*m_p9302hw->PortHData() &= ~0x0020;
    }
  else if (led==QHW_LED_ATTEN)
    {
      if (state)
	*m_p9302hw->PortGData() &= ~0x0002;
      else
	*m_p9302hw->PortGData() |= 0x0002;
    }
  else
    {
      resetMask = state ? 0 : 1<<led;
      setMask = state ? 1<<led : 0;
      
      *m_p9302hw->PortAData() &= ~((resetMask&0x03)<<5);
      *m_p9302hw->PortAData() |= (setMask&0x03)<<5;
      
      *m_p9302hw->PortBData() &= ~(resetMask&0xfc);
      *m_p9302hw->PortBData() |= setMask&0xfc;
    }
}

void CQwerkHardware::SetLEDs(unsigned char stateMask)
{
  *m_p9302hw->PortAData() &= ~0x0060;
  *m_p9302hw->PortAData() |= (stateMask&0x03)<<5;

  *m_p9302hw->PortBData() &= ~0x00fc;
  *m_p9302hw->PortBData() |= stateMask&0x00fc;
}

void CQwerkHardware::SetAudioEnable(bool state)
{
#if Q1
  *m_p9302hw->PortHData() &= ~0x0010;
  if (!state) // low going
    *m_p9302hw->PortHData() |= 0x0010;
  else
    usleep(100); // wait for audio amp to power up
#endif  
}

void CQwerkHardware::SetMotorVoltageScale(EQHWDivScale scale)
{
  if (scale==QHW_DIV_365TH)
    {
      // clear div bit in motor controller
      *m_p9302hw->m_fpga.Ushort(0x0a) &= ~0x0100;
      // set port H bit 2 as input (high-impedance)
      *m_p9302hw->PortHDataDR() &= ~0x0004;
    }
  else if (scale==QHW_DIV_108TH)
    {
      // set div bit in motor controller
      *m_p9302hw->m_fpga.Ushort(0x0a) |= 0x0100;
      // set port H bit 2 as low
      *m_p9302hw->PortHData() &= ~0x0004;
      // set port H bit 2 as output
      *m_p9302hw->PortHDataDR() |= 0x0004;
    }
  else if (scale==QHW_DIV_91TH)
    {
      // set div bit in motor controller
      *m_p9302hw->m_fpga.Ushort(0x0a) |= 0x0100;
      // set port H bit 2 as input
      *m_p9302hw->PortHDataDR() &= ~0x0004;
    }
}


