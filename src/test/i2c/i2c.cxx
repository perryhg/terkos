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

#include <stdio.h>
#include "9302hw.h"

int main(int argc, char **argv)
{
  C9302Hardware hw;

  // 0x0002 is input with clk high
  // 0x0007 is output with data and clk high

  *hw.m_fpga.Ushort(0x4c0) = 0x0020;
  
  *hw.m_fpga.Ushort(0x480) = 0x0002;
  while(1)
    printf("%x\n", *hw.m_fpga.Ushort(0x480));
}
