TARGET=libmmap
CXX=arm-oe-linux-uclibcgnueabi-g++
CFLAGS=
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)

all: $(TARGET).a

install: $(TARGET).a
	cp $(TARGET).a ../../lib
	cp *.h ../../include

$(TARGET).a: $(OBJECTS)
	arm-oe-linux-uclibcgnueabi-ar rvs $(TARGET).a $(OBJECTS)

clean:
	-rm *.o
	-rm *.a

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
