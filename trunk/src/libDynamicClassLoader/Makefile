SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
DIST_DIR=dist
LIBRARY_NAME=libDynamicClassLoader.so

#CXX=arm-linux-g++
CFLAGS=-fPIC -c -Wall
LDFLAGS=

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: init \
       $(LIBRARY_NAME)

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)

#-----------------------------------------------------------------------------------------------------------------------

DynamicObject.o: init $(SRC_DIR)/DynamicObject.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/DynamicObject.cpp -o $(OBJ_DIR)/DynamicObject.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

DynamicLibrary.o: init $(SRC_DIR)/DynamicLibrary.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/DynamicLibrary.cpp -o $(OBJ_DIR)/DynamicLibrary.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

DynamicLoader.o: init $(SRC_DIR)/DynamicLoader.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/DynamicLoader.cpp -o $(OBJ_DIR)/DynamicLoader.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

$(LIBRARY_NAME): DynamicObject.o DynamicLibrary.o DynamicLoader.o 
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,$(LIBRARY_NAME) -o $(LIB_DIR)/$(LIBRARY_NAME) $(OBJ_DIR)/DynamicObject.o $(OBJ_DIR)/DynamicLibrary.o $(OBJ_DIR)/DynamicLoader.o -L$(LIB_DIR)
	@echo 'Finished building target: $@'
	@echo ' '

dist: 
	mkdir -p $(DIST_DIR)
	@echo 'Copying shared libraries to $(DIST_DIR)...'
	cp $(LIB_DIR)/*.so $(DIST_DIR)/.
	@echo 'Finished copying shared libraries'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
