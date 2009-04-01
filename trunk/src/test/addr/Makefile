TARGET=addr
CXX=arm-linux-gnueabi-g++
CFLAGS=-I../../../include
LDFLAGS=-L../../../lib -lmmap
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

clean:
	-rm *.o
	-rm $(TARGET)
	-rm *~
	-rm *#

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
