CC = gcc
PROG = reimage
BINDIR = /usr/local/bin

CFLAGS = -Wall -g

$(PROG): dcon.o ymodem.o system.o
	$(CC) $(CFLAGS) -o $(PROG) dcon.o ymodem.o system.o

terkos.dat:
	rm -f *~
	rm -f *#
	tar cvf ../terkos.dat *

fw:	
	make terkos.dat -C firmware -f $(CURDIR)/Makefile

clean:
	$(RM) $(PROG)

