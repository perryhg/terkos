#include <stdio.h>
#include "9302hw.h"

C9302Hardware *C9302Hardware::m_p9302hw = NULL;
int C9302Hardware::m_refCount = 0;

C9302Hardware::C9302Hardware() :
  m_scr(0x80080000, 0x44),
  m_gpio(0x80840000, 0xca),
  m_syscon(0x80930000, 0xc4),
  m_fpga(0x20000000, 0x1000),
  m_adc(0x80900000, 0x28),
  m_uart1(0x808c0000, 0x21c)
{
  // set up A/D converter
  *m_syscon.Uint(0xc0) = 0xaa; // unlock
  *m_syscon.Uint(0x90) = 0x80010000; // enable ADC clk
  *m_syscon.Uint(0xc0) = 0xaa; // unlock
  *m_syscon.Uint(0x80) |= 0x08020800; // set ADCEN
  *m_syscon.Uint(0xc0) = 0xaa; // unlock
  *m_syscon.Uint(0x80) &= ~0x00000004; // reset ADCPD
  
  // set fpga bus timing
  *m_scr.Uint(0x08) = 0x10000440; 

  // set LEDs for output
  *PortEDataDR() = 0x03;
}

C9302Hardware::~C9302Hardware()
{
}

C9302Hardware *C9302Hardware::GetObject()
{
  if (m_p9302hw==NULL)
    m_p9302hw = new C9302Hardware();
  
  m_refCount++;
  return m_p9302hw;
}

void C9302Hardware::ReleaseObject()
{
  if (m_refCount)
    m_refCount--;

  if (m_refCount==0 && m_p9302hw!=NULL)
    delete m_p9302hw;
}

unsigned short C9302Hardware::GetAD(unsigned int channel)
{
  unsigned int clut[HW_AD_CHANNELS] = {0x608, 0x680, 0x640, 0x620, 0x610};
  volatile unsigned int d;
  unsigned short val;

  if (channel>=HW_AD_CHANNELS)
    return 0;

  // set mux
  *m_adc.Uint(0x20) = 0xaa;
  *m_adc.Uint(0x18) = clut[channel];

  // wait for conversion
  while(!(*m_adc.Uint(0x08)&0x80000000));
  // wait to settle
  for (d=0; d<10000; d++);

  // read
  val = *m_adc.Uint(0x08)&0xffff;
  if (val>25000 && val<=32767) 
    val=25000; //Enforce +25000 count limit
  if (val>=32678 && val<40536) 
    val=40536;//Enforce -25000 count limit
  val-=40536; //Adjust bias value

  return val;
}
