SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
DIST_DIR=dist

LIB_DYNAMIC_CLASS_LOADER_DIR=../libDynamicClassLoader
LIB_DYNAMIC_CLASS_LOADER_SRC_DIR=$(LIB_DYNAMIC_CLASS_LOADER_DIR)/src
LIB_DYNAMIC_CLASS_LOADER_DIST_DIR=$(LIB_DYNAMIC_CLASS_LOADER_DIR)/dist
LIB_DYNAMIC_CLASS_LOADER_LIB_SHORT_NAME=DynamicClassLoader

#CXX=arm-linux-g++
CFLAGS=-fPIC -c -Wall -I$(LIB_DYNAMIC_CLASS_LOADER_SRC_DIR)
LDFLAGS=

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: init \
       AudioProcessor

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(EXE_DIR)

#-----------------------------------------------------------------------------------------------------------------------

Filter.o: init $(SRC_DIR)/Filter.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/Filter.cpp -o $(OBJ_DIR)/Filter.o
	@echo 'Finished building target: $@'
	@echo ' '

Filter.so: Filter.o
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libFilter.so -o $(LIB_DIR)/libFilter.so $(OBJ_DIR)/Filter.o -L$(LIB_DIR)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

HighPassFilter.o: init $(SRC_DIR)/HighPassFilter.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/HighPassFilter.cpp -o $(OBJ_DIR)/HighPassFilter.o
	@echo 'Finished building target: $@'
	@echo ' '

HighPassFilter.so: Filter.so HighPassFilter.o
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libHighPassFilter.so -o $(LIB_DIR)/libHighPassFilter.so $(OBJ_DIR)/HighPassFilter.o -L$(LIB_DIR)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

LowPassFilter.o: init $(SRC_DIR)/LowPassFilter.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/LowPassFilter.cpp -o $(OBJ_DIR)/LowPassFilter.o
	@echo 'Finished building target: $@'
	@echo ' '

LowPassFilter.so: Filter.so LowPassFilter.o
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libLowPassFilter.so -o $(LIB_DIR)/libLowPassFilter.so $(OBJ_DIR)/LowPassFilter.o -L$(LIB_DIR)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

AudioProcessor.o: init $(SRC_DIR)/AudioProcessor.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/AudioProcessor.cpp -o $(OBJ_DIR)/AudioProcessor.o
	@echo 'Finished building target: $@'
	@echo ' '

AudioProcessor: libDynamicClassLoader.so Filter.so HighPassFilter.so LowPassFilter.so AudioProcessor.o
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/AudioProcessor $(OBJ_DIR)/AudioProcessor.o -L$(LIB_DIR) -l$(LIB_DYNAMIC_CLASS_LOADER_LIB_SHORT_NAME) -lFilter -ldl
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libDynamicClassLoader.so: init
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_DYNAMIC_CLASS_LOADER_DIR)
	cp $(LIB_DYNAMIC_CLASS_LOADER_DIST_DIR)/*.so $(LIB_DIR)/.
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

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
