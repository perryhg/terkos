SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
DIST_DIR=dist

LIB_JSONCPP_DIR=../../jsoncpp
LIB_JSONCPP_SRC_DIR=$(LIB_JSONCPP_DIR)/jsoncpp/include
LIB_JSONCPP_DIST_DIR=$(LIB_JSONCPP_DIR)/dist/arm-oe-linux-uclibcgnueabi
LIB_JSONCPP_LIB_SHORT_NAME=jsoncpp

LIB_UTILITIES_DIR=../../libUtilities
LIB_UTILITIES_SRC_DIR=$(LIB_UTILITIES_DIR)/src
LIB_UTILITIES_DIST_DIR=$(LIB_UTILITIES_DIR)/dist
LIB_UTILITIES_LIB_SHORT_NAME=Utilities

LIB_CONFIGURATION_MANAGEMENT_DIR=../../libConfigurationManagement
LIB_CONFIGURATION_MANAGEMENT_SRC_DIR=$(LIB_CONFIGURATION_MANAGEMENT_DIR)/src
LIB_CONFIGURATION_MANAGEMENT_DIST_DIR=$(LIB_CONFIGURATION_MANAGEMENT_DIR)/dist
LIB_CONFIGURATION_MANAGEMENT_LIB_SHORT_NAME=ConfigurationManagement
LIB_CONFIGURATION_MANAGEMENT_RESOURCES_DIR=$(LIB_CONFIGURATION_MANAGEMENT_DIR)/resources

LIB_QWERK_DIR=../../libqwerk
LIB_QWERK_SRC_DIR=$(LIB_QWERK_DIR)
LIB_QWERK_LIB_SHORT_NAME=qwerk

LIB_PSTREAMS_DIR=../../pstreams
LIB_PSTREAMS_SRC_DIR=$(LIB_PSTREAMS_DIR)/src

CXX=arm-oe-linux-uclibcgnueabi-g++
AR=arm-oe-linux-uclibcgnueabi-ar

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_JSONCPP_SRC_DIR) \
       -I$(LIB_UTILITIES_SRC_DIR) \
       -I$(LIB_CONFIGURATION_MANAGEMENT_SRC_DIR) \
       -I$(LIB_QWERK_SRC_DIR) \
       -I$(LIB_PSTREAMS_SRC_DIR)

LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_JSONCPP_LIB_SHORT_NAME) \
        -l$(LIB_UTILITIES_LIB_SHORT_NAME) \
        -l$(LIB_CONFIGURATION_MANAGEMENT_LIB_SHORT_NAME) \
        -l$(LIB_QWERK_LIB_SHORT_NAME) \
        -lpthread

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: init \
       LibConfigurationManagementTest

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(EXE_DIR)

#-----------------------------------------------------------------------------------------------------------------------

LibConfigurationManagementTest.o: init $(SRC_DIR)/LibConfigurationManagementTest.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/LibConfigurationManagementTest.cpp -o $(OBJ_DIR)/LibConfigurationManagementTest.o
	@echo 'Finished building target: $@'
	@echo ' '

LibConfigurationManagementTest: libConfigurationManagement.so \
                                LibConfigurationManagementTest.o
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/LibConfigurationManagementTest $(OBJ_DIR)/LibConfigurationManagementTest.o $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libConfigurationManagement.so: init
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_CONFIGURATION_MANAGEMENT_DIR) CXX=$(CXX)
	cp $(LIB_CONFIGURATION_MANAGEMENT_DIST_DIR)/*.so $(LIB_DIR)/.
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
	cp $(LIB_CONFIGURATION_MANAGEMENT_RESOURCES_DIR)/*.default.json $(DIST_DIR)/.
	@echo 'Finished copying resources'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
