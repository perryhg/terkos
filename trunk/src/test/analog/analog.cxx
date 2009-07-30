#include <unistd.h>
#include <stdio.h>
#include "9302hw.h"
#include "qwerkhw.h"

int main(int argc, char **argv) 
{
  unsigned short val;
  C9302Hardware *p9302hw=C9302Hardware::GetObject();
  CQwerkHardware *phw=CQwerkHardware::GetObject();

#if 0
  while(1)
    {
      val = p9302hw->GetAD(4);
      printf("%d\n", val);
    }
#endif
#if 1
  while(1)
    {
      val = phw->GetADVoltage(24);
      printf("%d\n", val);
    }
#endif

#if 0
  while(1)
    {
    printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\t\n",
	   hw.GetADRaw(0), hw.GetADRaw(1), hw.GetADRaw(2), hw.GetADRaw(3),
	   hw.GetADRaw(4), hw.GetADRaw(5), hw.GetADRaw(6), hw.GetADRaw(7),
	   hw.GetADRaw(8), hw.GetADRaw(9), hw.GetADRaw(10), hw.GetADRaw(11),
	   hw.GetADRaw(12), hw.GetADRaw(13), hw.GetADRaw(14), hw.GetADRaw(15));
    printf("%x\t%x\t%x\t%x\t%x\t%x\t%x\t%x\n",
	   hw.GetADRaw(16), hw.GetADRaw(17), hw.GetADRaw(18), hw.GetADRaw(19),
	   hw.GetADRaw(20), hw.GetADRaw(21), hw.GetADRaw(22), hw.GetADRaw(23));
    }
#endif
}

