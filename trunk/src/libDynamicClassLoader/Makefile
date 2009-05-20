SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
DIST_DIR=dist
LIBRARY_NAME=libDynamicClassLoader.so

CXX=arm-oe-linux-uclibcgnueabi-g++

CFLAGS=-fPIC -c -Wall

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(SRCS)))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(OBJECTS))

#-----------------------------------------------------------------------------------------------------------------------

default: all

build: $(LIBRARY_NAME)

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

$(OBJ_DIR):
	mkdir -p $@

$(LIB_DIR): 
	mkdir -p $@

#-----------------------------------------------------------------------------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

#-----------------------------------------------------------------------------------------------------------------------

$(LIBRARY_NAME):  $(OBJ_DIR) $(LIB_DIR) $(OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,$(LIBRARY_NAME) -o $(LIB_DIR)/$(LIBRARY_NAME) $(OBJECTS)
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
