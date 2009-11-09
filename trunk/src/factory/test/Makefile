TARGET=test
CXX=arm-oe-linux-uclibcgnueabi-g++
CFLAGS=-g -I../../../include -Os

LDFLAGS=-Os -shared -L../../libqwerk/dist -lpthread -lqwerk
#LDFLAGS=-L../../../lib -lpthread -lqwerk
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -g $(OBJECTS) -o $(TARGET) $(LDFLAGS)

clean:
	-rm *.o
	-rm $(TARGET)
	-rm *~
	-rm *#

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
