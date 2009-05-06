TARGET=libqwerk
CXX=arm-linux-gnueabi-g++
AR=arm-linux-gnueabi-ar
CFLAGS=-g -Wall
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

default: all

all: $(TARGET).a

install: $(TARGET).a
	cp $(TARGET).a ../../lib
	cp *.h ../../include

$(TARGET).a: $(OBJECTS)
	$(AR) rvs $(TARGET).a $(OBJECTS)

clean:
	-rm -f $(TARGET)
	-rm -f *~
	-rm -f *#
	-rm -f *.o
	-rm -f *.a

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
