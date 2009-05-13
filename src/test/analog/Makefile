TARGET=analog
CXX=arm-linux-gnueabi-g++
CFLAGS=-g -I../../../include
LDFLAGS=-L../../../lib -lqwerk
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
