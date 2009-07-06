ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /home/rich/terk/terkos/trunk/oe/tmp/work/qwerk-oe-linux-uclibcgnueabi/linux-2.6.29.4-r2/linux-2.6.29.4

PWD := $(shell pwd)

modules:
	$(MAKE) ARCH=arm CROSS_COMPILE=arm-oe-linux-uclibcgnueabi- modules2

modules2:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install

clean:
	-rm -rf *.o *~ *# core .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers

.PHONY: modules modules_install clean

else

    obj-m := qeinterrupt.o 

endif


