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
#include "qemotoruser.h"
#include "qemotortraj.h"

int main(int argc, char **argv)
{
  int i = 0;
  
  
#if 0
  C9302Hardware hw;
  printf("%x\n", *hw.m_fpga.Ushort(0xfd0));

  *hw.m_fpga.Ushort(0x000) = 0x00ff | 0x0100; 
  *hw.m_fpga.Ushort(0x008) = 0x0700 | 0x0030; 
  *hw.m_fpga.Ushort(0x00a) = 0x0009;
#endif
#if 0
  while(1)
    {
      i++;
      // wait for measurement
      while(*hw.m_fpga.Ushort(0x00c));
      // wait for measurement to end
      while(!(*hw.m_fpga.Ushort(0x00c)));
      printf("%x\n", *hw.m_fpga.Ushort(0x200));
      //if (i%200==0)
      //printf("%d\n", i/200);
    }
#endif
#if 0
  while(1)
    {
      sleep(1);
      printf("brake\n");
      *hw.m_fpga.Ushort(0x000) = 0x00f0 | 0x0300; 
      sleep(1);
      *hw.m_fpga.Ushort(0x000) = 0x00f0 | 0x0200;
      sleep(1);
      printf("coast\n");
      *hw.m_fpga.Ushort(0x000) = 0x00f0 | 0x0000; 
      sleep(1);
    }
#endif
#if 0
  Axis_position pos;
  printf("1\n");
  CQEMotorUser mu(NULL, 0);
  printf("2\n");
  
  //mu.SetPWM(0, 128);
  mu.SetPIDVGains(0, 100, 0, 500, 0);
  mu.SetPosition(0, 0);
  mu.Hold(0, 1);
  while(1)
    {
      pos = mu.GetPosition(0);
      printf("%d\n", pos);
    }
#endif
#if 1
  int axis;
  if (argc<2)
    {
      printf("specify axis\n");
      exit(1);
    }
  axis = atoi(argv[1]);

  printf("axis %d\n", axis);

  CQEMotorTraj &motor = CQEMotorTraj::GetRef();

  motor.SetPIDVGains(axis, 100, 0, 500, 0);
  printf("running\n");

  while(1)
    {
      motor.Move(axis, 40000, 20000, 8000);
      while(!motor.Done(axis)); 
      motor.Move(axis, -40000, 20000, 8000);
      while(!motor.Done(axis)); 
    }
#endif
}
