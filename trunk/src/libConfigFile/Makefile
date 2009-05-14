SRC_DIR=src
BUILD_DIR=build
RESOURCES_DIR=resources
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
DIST_DIR=dist

LIB_CONFIG_DIR=../libconfig/dist
LIB_CONFIG_SRC_DIR=$(LIB_CONFIG_DIR)/include
LIB_CONFIG_DIST_DIR=$(LIB_CONFIG_DIR)/lib
LIB_CONFIG_LIB_SHORT_NAME=config++

#CXX=arm-oe-linux-uclibcgnueabi-g++

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_CONFIG_SRC_DIR)
LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_CONFIG_LIB_SHORT_NAME)

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: init \
       libconfig++.so \
       libConfigFile.so \
       libAudioConfig.so \
       libServoConfig.so

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)

#-----------------------------------------------------------------------------------------------------------------------

ConfigFile.o: init $(SRC_DIR)/ConfigFile.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/ConfigFile.cpp -o $(OBJ_DIR)/ConfigFile.o
	@echo 'Finished building target: $@'
	@echo ' '

libConfigFile.so: ConfigFile.o 
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libConfigFile.so -o $(LIB_DIR)/libConfigFile.so $(OBJ_DIR)/ConfigFile.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

AudioConfig.o: init $(SRC_DIR)/AudioConfig.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/AudioConfig.cpp -o $(OBJ_DIR)/AudioConfig.o
	@echo 'Finished building target: $@'
	@echo ' '

libAudioConfig.so: AudioConfig.o 
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libAudioConfig.so -o $(LIB_DIR)/libAudioConfig.so $(OBJ_DIR)/AudioConfig.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

ServoConfig.o: init $(SRC_DIR)/ServoConfig.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/ServoConfig.cpp -o $(OBJ_DIR)/ServoConfig.o
	@echo 'Finished building target: $@'
	@echo ' '

libServoConfig.so: ServoConfig.o 
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libServoConfig.so -o $(LIB_DIR)/libServoConfig.so $(OBJ_DIR)/ServoConfig.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libconfig++.so: init
	@echo 'Building target: $@'
	cp $(LIB_CONFIG_DIST_DIR)/lib$(LIB_CONFIG_LIB_SHORT_NAME).so $(LIB_DIR)/.
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
	@echo 'Copying resources to $(DIST_DIR)...'
	cp $(RESOURCES_DIR)/* $(DIST_DIR)/.
	@echo 'Finished copying resources'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
