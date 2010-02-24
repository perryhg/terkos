//
// begin license header
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
// end license header
//

#ifndef _QEINTERRUPT_H
#define _QEINTERRUPT_H

#include <linux/ioctl.h>

#ifndef TRUE
#define TRUE                 1
#endif
#ifndef FALSE               
#define FALSE                0
#endif

#define QEINT_MAJOR_NUMBER     210
#define QEINT_MINOR_NUMBER     0

#define QEINT_NUM_INTERRUPTS   32
#define QEINT_BASE_ADDR        0x20000ff0
#define QEINT_WIDTH            0x10
#define QEINT_INTERRUPT        IRQ_EP93XX_EXT0

#define QEINT_MODE_UNUSED      0
#define QEINT_MODE_FAST        1
#define QEINT_MODE_DEFERRED    2

#define QEINT_IOC_MAGIC        0xe1

#define QEINT_IOC_READ_STATUS  _IOR(QEINT_IOC_MAGIC, 1, char)
#define QEINT_IOC_RESET_STATUS _IO(QEINT_IOC_MAGIC, 2)
#define QEINT_IOC_DISABLE      _IO(QEINT_IOC_MAGIC, 3)
#define QEINT_IOC_ENABLE       _IO(QEINT_IOC_MAGIC, 4)

#define QEINT_IOC_MAXNR        4

int qe_interrupt_enable(unsigned char vector);
int qe_interrupt_disable(unsigned char vector);
int qe_interrupt_register(unsigned char vector, unsigned char mode, void (*callback)(unsigned char));
void qe_interrupt_time(int vector, struct timeval *ptv);
int qe_interrupt_unregister(int vector);
#endif
