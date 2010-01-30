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
     
void sig_handler(int signum)
{
  struct timeval tv;
  read(fd, (void *)&tv, sizeof(tv));
  printf("signal %d %d %d\n", signum, tv.tv_sec, tv.tv_usec);
  gettimeofday(&tv, NULL);
  printf("%d %d\n", tv.tv_sec, tv.tv_usec);
  
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
#if 1

  fd = open("/dev/qeint16", O_RDWR); 
  if (fd<0)
    printf("error open\n");
  if (signal(SIGIO, sig_handler)<0)
    printf("error signal\n");
  if (fcntl(fd, F_SETOWN, getpid())<0)
    printf("error fcntl\n");
  fcntl(fd, F_GETFL);
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
}
