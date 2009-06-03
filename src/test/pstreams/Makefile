SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
DIST_DIR=dist

LIB_PSTREAMS_DIR=../../pstreams
LIB_PSTREAMS_SRC_DIR=$(LIB_PSTREAMS_DIR)/src

CXX=arm-oe-linux-uclibcgnueabi-g++
AR=arm-oe-linux-uclibcgnueabi-ar

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_PSTREAMS_SRC_DIR)
       
LDFLAGS=-L$(LIB_DIR)

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(SRCS)))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(OBJECTS))

#-----------------------------------------------------------------------------------------------------------------------

default: all

build: PStreamsTest

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

$(OBJ_DIR):
	mkdir -p $@

$(EXE_DIR):
	mkdir -p $@

#-----------------------------------------------------------------------------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

#-----------------------------------------------------------------------------------------------------------------------

PStreamsTest: $(OBJ_DIR) $(EXE_DIR) $(OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/PStreamsTest $(OBJECTS) $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

dist: 
	mkdir -p $(DIST_DIR)
	@echo 'Copying executables to $(DIST_DIR)...'
	cp $(EXE_DIR)/* $(DIST_DIR)/.
	@echo 'Finished copying executables'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
