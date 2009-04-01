CC = gcc
PROG = reimage
BINDIR = /usr/local/bin

CFLAGS = -Wall -g

$(PROG): dcon.o ymodem.o system.o
	$(CC) $(CFLAGS) -o $(PROG) dcon.o ymodem.o system.o

firmware.dat:
	rm -f *~
	rm -f *#
	tar cvf ../firmware.dat *

fw:	
	make firmware.dat -C firmware -f $(CURDIR)/Makefile

clean:
	$(RM) $(PROG)

