CC = gcc
PROG = reimage
BINDIR = /usr/local/bin

CFLAGS = -Wall -g

$(PROG): dcon.o ymodem.o system.o
	$(CC) $(CFLAGS) -o $(PROG) dcon.o ymodem.o system.o

terkos.dat:
	rm -f *~
	rm -f *#
	tar cvf ../terkos.dat script zImage root.jffs2

kernel.dat:
	rm -f *~
	rm -f *#
	tar cvf ../kernel.dat script zImage

fw:	
	make terkos.dat -C firmware -f $(CURDIR)/Makefile

kernel:	
	cp ../../../oe/tmp/deploy/images/qwerk/zImage-qwerk.bin kernel_upgrade/zImage
	make kernel.dat -C kernel_upgrade -f $(CURDIR)/Makefile

fwcopy:	
	cp ../../../oe/tmp/deploy/images/qwerk/zImage-qwerk.bin firmware/zImage
	cp ../../../oe/tmp/deploy/images/qwerk/vexpro-image-qwerk.jffs2 firmware/root.jffs2
	make terkos.dat -C firmware -f $(CURDIR)/Makefile

upload: fwcopy $(PROG)
	./$(PROG) terkos.dat

clean:
	$(RM) $(PROG)

