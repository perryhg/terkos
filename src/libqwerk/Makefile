TARGET=libqwerk
CFLAGS=-g -fPIC -Wall
SRCS=$(wildcard *.cxx)
OBJECTS=$(SRCS:.cxx=.o)
DIST_DIR=dist
VPATH=$(DIST_DIR)

ifdef WINDOWS_MACHINE
CXX=/OpenEmbedded/bin/arm-oe-linux-uclibcgnueabi-g++
AR=/OpenEmbedded/bin/arm-oe-linux-uclibcgnueabi-ar
CP=copy
RM=del
MKDIR=mkdir
LIB_INSTALL_DIR=\OpenEmbedded\arm-oe-linux-uclibcgnueabi\lib
HEADER_INSTALL_DIR=\OpenEmbedded\arm-oe-linux-uclibcgnueabi\include
else
CXX=/usr/local/terkos/arm/bin/arm-oe-linux-uclibcgnueabi-g++
AR=/usr/local/terkos/arm/bin/arm-oe-linux-uclibcgnueabi-ar
CP=cp
RM=rm -rf
MKDIR=mkdir -p
LIB_INSTALL_DIR=/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/lib
HEADER_INSTALL_DIR=/usr/local/terkos/arm/arm-oe-linux-uclibcgnueabi/include
endif

default: all

all: $(TARGET).a $(TARGET).so

html: 
	$(RM) doc
	doxygen Doxyfile

$(DIST_DIR):
	$(MKDIR) $@

$(LIB_INSTALL_DIR):
	$(MKDIR) $@

$(HEADER_INSTALL_DIR):
	$(MKDIR) $@

# for now, only install the static library...
install: all $(LIB_INSTALL_DIR) $(HEADER_INSTALL_DIR)
	$(CP) $(TARGET).a $(LIB_INSTALL_DIR)
	$(CP) *.h $(HEADER_INSTALL_DIR)

$(TARGET).a: $(OBJECTS)
	$(AR) rvs $(TARGET).a $(OBJECTS)

$(TARGET).so: $(DIST_DIR) $(OBJECTS)
	$(CXX) -shared -Wl,-soname,$(TARGET).so -o $(DIST_DIR)/$(TARGET).so $(OBJECTS)

clean:
	$(RM) $(DIST_DIR)
	$(RM) *~
	$(RM) *#
	$(RM) *.o
	$(RM) *.a
	$(RM) *.so

%.o: %.cxx
	$(CXX) -c $(CFLAGS) $<
