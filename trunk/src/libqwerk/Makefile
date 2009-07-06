TARGET=libqwerk
CXX=arm-oe-linux-uclibcgnueabi-g++
AR=arm-oe-linux-uclibcgnueabi-ar
CFLAGS=-g -Wall
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)
DIST_DIR=dist
VPATH=$(DIST_DIR)

default: all

all: $(TARGET).a $(TARGET).so

$(DIST_DIR):
	mkdir -p $@

install: all
	cp $(TARGET).a ../../lib
	cp *.h ../../include

$(TARGET).a: $(OBJECTS)
	$(AR) rvs $(TARGET).a $(OBJECTS)

$(TARGET).so: $(DIST_DIR) $(OBJECTS)
	$(CXX) -shared -Wl,-soname,$(TARGET).so -o $(DIST_DIR)/$(TARGET).so $(OBJECTS)

clean:
	-rm -rf $(DIST_DIR)
	-rm -f $(TARGET)
	-rm -f *~
	-rm -f *#
	-rm -f *.o
	-rm -f *.a

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
