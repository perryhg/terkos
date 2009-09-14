TARGET=PowerTest
CXX=arm-oe-linux-uclibcgnueabi-g++
CFLAGS=-g -I../../libqwerk
LDFLAGS=-L../../libqwerk -lqwerk
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f *~
	-rm -f *#

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
