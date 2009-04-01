#*****************************************************************************
#
# MAKEFILE - Builds the download utility.
#
# Copyright (c) 2004 Cirrus Logic, Inc.
#
#*****************************************************************************

#
# Choose the tools to use based on the host OS.
#
ifeq ("$(OS)","Windows_NT")
CROSSTOOLS = ads
NATIVETOOLS = msvc
else
CROSSTOOLS = gcc
NATIVETOOLS = gcc
endif

#
# The name of the image that we are building.
#
TARGET = flasher

#
# The set of object files that comprise the download utility.
#
OBJS = flasher.o

#
# The read-only base for the image.
#
ROBASE = 0x00000000

#
# Include the common makefile definitions.
#
include makedefs

#
# The default rule, which builds the download utility.
#
.PHONY: default
default: download$(PROGEXT)

#
# A rule to build the download utility.
#
download$(PROGEXT): download.c flasher.h
ifeq ("$(OS)","Windows_NT")
	$(NATIVECC) -DHAVE_STRING_H -o download$(PROGEXT) download.c getopt.c
else
	$(NATIVECC) -o download$(PROGEXT) download.c
endif

#
# A rule to create the FLASH code header file from the binary image.
#
flasher.h: $(TARGET).rom
	od -t u1 -v $(TARGET).rom | awk -f mkarray.awk > flasher.h

#
# A rule to remove the build products.
#
clean::
	@rm -f download$(PROGEXT) flasher.h
