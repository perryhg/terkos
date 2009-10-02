ifeq ($(KERNELRELEASE),)

KERNELDIR ?= $(TERKOS_ROOT)/oe/tmp/work/qwerk-oe-linux-uclibcgnueabi/linux-2.6.29.6-r2/linux-2.6.29.6

PWD := $(shell pwd)

modules:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-oe-linux-uclibcgnueabi- modules2

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


