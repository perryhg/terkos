TARGET=fpgac
CXX=arm-oe-linux-uclibcgnueabi-g++
CFLAGS=-I../../../include
LDFLAGS=-L../../../lib -lmmap
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

all: $(TARGET)

install: $(TARGET)
	cp $(TARGET) ../../../bin

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<#export PATH=${TERKOS_ROOT}/oe/openembedded/bitbake/bin:/usr/local/arm-linux-gnueabi/bin:/usr/local/arm/3.2.1-elf/bin:$PATH
