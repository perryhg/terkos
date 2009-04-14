#include <unistd.h>
#include <stdio.h>
#include "9302hw.h"

int main(int argc, char **argv)
{
  C9302Hardware hw;

  
  printf("%x\n", *hw.m_fpga.Ushort(0xfd0));
  while(1)
    {
      *hw.m_fpga.Ushort(0x000) = 0x00f0 | 0x0100; 
      sleep(1);
      *hw.m_fpga.Ushort(0x000) = 0x00f0 | 0x0200;
      sleep(1);
    }

}
