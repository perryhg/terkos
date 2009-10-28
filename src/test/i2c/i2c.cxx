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
