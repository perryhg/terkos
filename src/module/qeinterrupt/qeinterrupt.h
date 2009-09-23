#ifndef _QEINTERRUPT_H
#define _QEINTERRUPT_H

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

int qe_interrupt_enable(unsigned char vector);
int qe_interrupt_disable(unsigned char vector);
int qe_interrupt_register(unsigned char vector, unsigned char mode, void (*callback)(unsigned char));
void qe_interrupt_time(int vector, struct timeval *ptv);
int qe_interrupt_unregister(int vector);
#endif
