SRC_DIR=src
BUILD_DIR=build
RESOURCES_DIR=resources
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
DIST_DIR=dist

LIB_CONFIG_DIR=../../libconfig/dist
LIB_CONFIG_SRC_DIR=$(LIB_CONFIG_DIR)/include
LIB_CONFIG_DIST_DIR=$(LIB_CONFIG_DIR)/lib
LIB_CONFIG_LIB_SHORT_NAME=config++

LIB_CONFIG_FILE_DIR=../../libConfigFile
LIB_CONFIG_FILE_SRC_DIR=$(LIB_CONFIG_FILE_DIR)/src
LIB_CONFIG_FILE_DIST_DIR=$(LIB_CONFIG_FILE_DIR)/dist
LIB_CONFIG_FILE_LIB_SHORT_NAME=ConfigFile

LIB_AUDIO_CONFIG_DIR=../../libConfigFile
LIB_AUDIO_CONFIG_SRC_DIR=$(LIB_AUDIO_CONFIG_DIR)/src
LIB_AUDIO_CONFIG_DIST_DIR=$(LIB_AUDIO_CONFIG_DIR)/dist
LIB_AUDIO_CONFIG_LIB_SHORT_NAME=AudioConfig

#CXX=arm-oe-linux-uclibcgnueabi-g++

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_CONFIG_SRC_DIR) \
       -I$(LIB_CONFIG_FILE_SRC_DIR) \
       -I$(LIB_AUDIO_CONFIG_SRC_DIR)
       
LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_CONFIG_LIB_SHORT_NAME) \
        -l$(LIB_CONFIG_FILE_LIB_SHORT_NAME) \
        -l$(LIB_AUDIO_CONFIG_LIB_SHORT_NAME)
        

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: init \
       LibConfigFileTest

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(EXE_DIR)

#-----------------------------------------------------------------------------------------------------------------------

LibConfigFileTest.o: init $(SRC_DIR)/LibConfigFileTest.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/LibConfigFileTest.cpp -o $(OBJ_DIR)/LibConfigFileTest.o
	@echo 'Finished building target: $@'
	@echo ' '

LibConfigFileTest: LibConfigFile LibConfigFileTest.o
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/LibConfigFileTest $(OBJ_DIR)/LibConfigFileTest.o $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

LibConfigFile: init
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_CONFIG_FILE_DIR) CXX=$(CXX)
	cp $(LIB_CONFIG_FILE_DIST_DIR)/*.so $(LIB_DIR)/.
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

dist: 
	mkdir -p $(DIST_DIR)
	@echo 'Copying shared libraries to $(DIST_DIR)...'
	cp $(LIB_DIR)/*.so $(DIST_DIR)/.
	ln -s lib$(LIB_CONFIG_LIB_SHORT_NAME).so $(DIST_DIR)/lib$(LIB_CONFIG_LIB_SHORT_NAME).so.8
	ln -s lib$(LIB_CONFIG_LIB_SHORT_NAME).so $(DIST_DIR)/lib$(LIB_CONFIG_LIB_SHORT_NAME).so.8.0.0
	@echo 'Finished copying shared libraries'
	@echo ' '
	@echo 'Copying executables to $(DIST_DIR)...'
	cp $(EXE_DIR)/* $(DIST_DIR)/.
	@echo 'Finished copying executables'
	@echo ' '
	@echo 'Copying resources to $(DIST_DIR)...'
	cp $(RESOURCES_DIR)/* $(DIST_DIR)/.
	@echo 'Finished copying resources'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
