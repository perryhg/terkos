ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /home/rich/oe/EMAC-OE/tmp/work/IPAC9302-linux-gnueabi/IPAC9302-kernel-2.6.22-r1.3/linux-2.6.22

PWD := $(shell pwd)

modules:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-linux-gnueabi- modules2

modules2:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

.PHONY: modules modules_install clean

else

qemotor-objs := main.o motor.o

obj-m := qemotor.o 

endif


