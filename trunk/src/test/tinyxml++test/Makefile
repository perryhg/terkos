SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
DIST_DIR=dist

TINY_XML_DIR=../tinyxml++
TINY_XML_SRC_DIR=$(TINY_XML_DIR)/ticpp
TINY_XML_DIST_DIR=$(TINY_XML_DIR)/dist
TINY_XML_LIB_SHORT_NAME=ticppd

#CXX=arm-linux-g++
CFLAGS=-fPIC -c -Wall -I$(TINY_XML_SRC_DIR) -D "TIXML_USE_TICPP" -D "UNICODE" -D "_UNICODE" -D "DEBUG" -D "_DEBUG"
LDFLAGS=

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: init \
       XmlTest

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(EXE_DIR)

#-----------------------------------------------------------------------------------------------------------------------

XmlTest.o: init $(SRC_DIR)/XmlTest.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/XmlTest.cpp -o $(OBJ_DIR)/XmlTest.o
	@echo 'Finished building target: $@'
	@echo ' '

XmlTest: libTinyXml XmlTest.o
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/XmlTest $(OBJ_DIR)/XmlTest.o -L$(LIB_DIR) -l$(TINY_XML_LIB_SHORT_NAME)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libTinyXml: init
	@echo 'Building target: $@'
	$(MAKE) -C $(TINY_XML_SRC_DIR)
	cp $(TINY_XML_DIST_DIR)/*.so $(LIB_DIR)/.
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

dist: 
	mkdir -p $(DIST_DIR)
	@echo 'Copying shared libraries to $(DIST_DIR)...'
	cp $(LIB_DIR)/*.so $(DIST_DIR)/.
	@echo 'Finished copying shared libraries'
	@echo ' '
	@echo 'Copying executables to $(DIST_DIR)...'
	cp $(EXE_DIR)/* $(DIST_DIR)/.
	@echo 'Finished copying executables'
	@echo ' '
	@echo 'Copying resources to $(DIST_DIR)...'
	cp menu.xml $(DIST_DIR)/.
	@echo 'Finished copying resources'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
