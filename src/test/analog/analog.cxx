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

#include <unistd.h>
#include <stdio.h>
#include "9302hw.h"
#include "qeanalog.h"
#include "qepower.h"

int main(int argc, char **argv) 
{
  CQEAnalog *phw = CQEAnalog::GetPtr();
  //CQEPower *pp = CQEPower::GetPtr();
#if 0
  long val;
  pp->GetProperty(QP_PROP_BREAKER_STATE, &val);
  printf("breaker %x\n", val);
  pp->GetProperty(QP_PROP_MAIN_BATT_VOLTAGE, &val);
  printf("breaker %d\n", val);
  exit(0);
#endif
#if 0
  while(1)
    {
      val = p9302hw->GetAD(4);
      printf("%d\n", val);
    }
#endif
#if 0
  while(1)
    {
      val = phw->GetADVoltage(24);
      printf("%d\n", val);
    }
#endif

#if 1
  while(1)
    {
#if 1
    printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
	   phw->GetADVoltage(0), phw->GetADVoltage(1), phw->GetADVoltage(2), phw->GetADVoltage(3),
	   phw->GetADVoltage(4), phw->GetADVoltage(5), phw->GetADVoltage(6), phw->GetADVoltage(7),
	   phw->GetADVoltage(8), phw->GetADVoltage(9), phw->GetADVoltage(10), phw->GetADVoltage(11),
	   phw->GetADVoltage(12), phw->GetADVoltage(13), phw->GetADVoltage(14), phw->GetADVoltage(15));
#endif
#if 1
    printf("# %d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
	   phw->GetADVoltage(16), phw->GetADVoltage(17), phw->GetADVoltage(18), phw->GetADVoltage(19),
	   phw->GetADVoltage(20), phw->GetADVoltage(21), phw->GetADVoltage(22), phw->GetADVoltage(23), phw->GetADVoltage(24));
    if (phw->Requested())
      {
	printf("releasing\n");
	phw->Release();
	while(1)
	  {
	    phw = CQEAnalog::GetPtr(true);
	    if (phw)
	      break;
	    printf("waiting\n");
	  }
      }
#endif
    }
#endif
}

