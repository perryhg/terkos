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
#include <unistd.h>
#include <signal.h>
#include "qegpioint.h"

int fd, oflags;

void callback(unsigned int io, struct timeval *ptv)
{
  printf("callback %d %d %d\n", io, ptv->tv_sec, ptv->tv_usec);
}
 
void sig_handler(int signum)
{
  struct timeval tv, tv2;
  gettimeofday(&tv2, NULL);
  read(fd, (void *)&tv, sizeof(tv));
  printf("signal %d %d %d %d %d\n", signum, tv.tv_sec, tv.tv_usec,
	 tv2.tv_sec, tv2.tv_usec);  
}

int main()
{
  CQEGpioInt *pgpio = CQEGpioInt::GetPtr();

#if 0
  *pgpio->DataDir() = 0x0001;
  *pgpio->Data() = 0x0001;
#endif

#if 0
  CMemMap fpga(0x20000000, 0x1000);
  *pgpio->DataDir() = 0x0001;
  *pgpio->Data() = 0x0000;
  *pgpio->IntEdge() = 0x0001;

  *fpga.Uint(0xff0) = 0x00010000;

  printf("%x\n", *fpga.Uint(0xff4));
  *fpga.Uint(0xff4) = 0x00010000;
  printf("%x\n", *fpga.Uint(0xff4));
  
  *pgpio->Data() = 0x0001;

  printf("%x\n", *fpga.Uint(0xff4));
  *fpga.Uint(0xff4) = 0x00010000;
  printf("%x\n", *fpga.Uint(0xff4));

  while(1){
  *pgpio->Data() = 0x0001;

  usleep(1);
  printf("%x\n", *fpga.Uint(0xff4));
  *fpga.Uint(0xff4) = 0x00010000;
  *pgpio->Data() = 0x0000;
  usleep(1);
  printf("%x\n", *fpga.Uint(0xff4));

  }
#endif
#if 0

  fd = open("/dev/qeint16", O_RDWR); 
  if (fd<0)
    printf("error open\n");
  if (signal(SIGIO, sig_handler)<0)
    printf("error signal\n");
  if (fcntl(fd, F_SETOWN, getpid())<0)
    printf("error fcntl\n");
  oflags = fcntl(fd, F_GETFL);
  fcntl(fd, F_SETFL, oflags | FASYNC);

  printf("0\n");
  pgpio->SetProperty(QEG_PROP_DATA_DIR_REG, 0x0001);
  printf("1\n");
  pgpio->SetProperty(QEG_PROP_INTERRUPT_MODE, 0x0001);
  printf("2\n");

  while(1)
    {
      printf("0\n");
      pgpio->SetProperty(QEG_PROP_DATA_REG, 0x0000);
      sleep(1);
      printf("1\n");
      pgpio->SetProperty(QEG_PROP_DATA_REG, 0x0001);
      sleep(1);
    }

#endif
#if 0

  *pgpio->DataDir() = 0x0000;
  *pgpio->Data() = 0x00aa;

  while(1)
    printf("%x\n", *pgpio->Data());
#endif
#if 1
  printf("0\n");
  pgpio->SetData(0x0000);
  pgpio->SetDataDirection(0xffff);
  printf("1\n");
  pgpio->RegisterCallback(0, callback);
  pgpio->RegisterCallback(1, callback);
  pgpio->RegisterCallback(2, callback);
  pgpio->RegisterCallback(3, callback);
  pgpio->RegisterCallback(4, callback);
  pgpio->RegisterCallback(5, callback);
  pgpio->RegisterCallback(6, callback);
  pgpio->RegisterCallback(7, callback);
  pgpio->RegisterCallback(8, callback);
  pgpio->RegisterCallback(9, callback);
  pgpio->RegisterCallback(10, callback);
  pgpio->RegisterCallback(11, callback);
  pgpio->RegisterCallback(12, callback);
  pgpio->RegisterCallback(13, callback);
  pgpio->RegisterCallback(14, callback);
  pgpio->RegisterCallback(15, callback);
  pgpio->SetInterruptMode(0, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(1, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(2, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(3, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(4, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(5, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(6, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(7, QEG_INTERRUPT_POSEDGE); 
  pgpio->SetInterruptMode(8, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(9, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(10, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(11, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(12, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(13, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(14, QEG_INTERRUPT_NEGEDGE); 
  pgpio->SetInterruptMode(15, QEG_INTERRUPT_NEGEDGE); 
  printf("2\n");

  printf("int %d\n", pgpio->SetInterrupt(0, false));
  printf("int %d\n", pgpio->SetInterrupt(8, false));

  while(1)
    {
      printf("0\n");
      pgpio->SetData(0x0000);
      sleep(1);
      printf("1\n");
      pgpio->SetData(0xffff);
      sleep(1);
    }
#endif
}
