TARGET=fpgac
CXX=arm-oe-linux-uclibcgnueabi-g++
CFLAGS=-I../../libmmap
LDFLAGS=-L../../libmmap -lmmap
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

all: $(TARGET)

install: $(TARGET)
	cp $(TARGET) ../../../bin

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
