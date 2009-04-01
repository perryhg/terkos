TARGET=fpgac
CXX=arm-linux-gnueabi-g++
CFLAGS=-I../../include
LDFLAGS=-L../../lib -lmmap
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

install: $(TARGET)
	cp $(TARGET) ../../bin

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<