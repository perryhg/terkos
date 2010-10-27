TARGET=rcservo
CXX=arm-oe-linux-uclibcgnueabi-g++
CFLAGS=-g -I/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/include/terk
LDFLAGS=-L/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/lib/terk -lqwerk
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
