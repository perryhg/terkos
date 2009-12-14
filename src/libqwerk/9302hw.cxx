//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v3 (http://www.gnu.org/licenses/gpl-3.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

#include <stdio.h>
#include "9302hw.h"


C9302Hardware::C9302Hardware() :
  m_scr(0x80080000, 0x44),
  m_gpio(0x80840000, 0xca),
  m_syscon(0x80930000, 0xc4),
  m_fpga(0x20000000, 0x1000),
  m_adc(0x80900000, 0x28),
  m_uart1(0x808c0000, 0x21c),
  m_uart2(0x808d0000, 0x21c)
{
  // set up A/D converter
  *m_syscon.Uint(0xc0) = 0xaa; // unlock
  *m_syscon.Uint(0x90) = 0x80010000; // enable ADC clk
  *m_syscon.Uint(0xc0) = 0xaa; // unlock
  *m_syscon.Uint(0x80) |= 0x08120800; // set ADCEN, U2EN
  *m_syscon.Uint(0xc0) = 0xaa; // unlock
  *m_syscon.Uint(0x80) &= ~0x00000004; // reset ADCPD
  
  // set fpga bus timing
  *m_scr.Uint(0x08) = 0x10000440; 

  // enable UART2
  *m_uart2.Uint(0x14) = 0x01;

  // set LEDs for output
  *PortEDataDR() = 0x03;

  // turn on 5V I/O regulator
  *PortHDataDR() |= 0x0020;
  *PortHData() |= 0x0020;
}

C9302Hardware::~C9302Hardware()
{
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

unsigned short C9302Hardware::GetBitstreamVersion()
{
  return *m_fpga.Ushort(0xfd0);
}

unsigned char C9302Hardware::GetBitstreamMinorVersion()
{
  unsigned short ver = GetBitstreamVersion();
  return (ver&0xff);
}

unsigned char C9302Hardware::GetBitstreamMajorVersion()
{
  unsigned short ver = GetBitstreamVersion();
  return ((ver>>8)&0xff);
}
