SRC_DIR=src
BUILD_DIR=build
RESOURCES_DIR=resources
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
DIST_DIR=dist

LIB_JSONCPP_DIR=../../jsoncpp
LIB_JSONCPP_SRC_DIR=$(LIB_JSONCPP_DIR)/jsoncpp/include
LIB_JSONCPP_DIST_DIR=$(LIB_JSONCPP_DIR)/dist/arm-oe-linux-uclibcgnueabi
LIB_JSONCPP_LIB_SHORT_NAME=jsoncpp

CXX=arm-oe-linux-uclibcgnueabi-g++

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_JSONCPP_SRC_DIR)
       
LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_JSONCPP_LIB_SHORT_NAME) \
        -lpthread

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(SRCS)))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(OBJECTS))

#-----------------------------------------------------------------------------------------------------------------------

default: all

build: JsonCppTest

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

$(OBJ_DIR):
	mkdir -p $@

$(LIB_DIR):
	mkdir -p $@

$(EXE_DIR): 
	mkdir -p $@

#-----------------------------------------------------------------------------------------------------------------------

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

#-----------------------------------------------------------------------------------------------------------------------

JsonCppTest: $(OBJ_DIR) $(LIB_DIR) $(EXE_DIR) $(OBJECTS) libjsoncpp.so
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/JsonCppTest $(OBJ_DIR)/JsonCppTest.o $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libjsoncpp.so: $(LIB_DIR)
	@echo 'Building target: $@'
	cp $(LIB_JSONCPP_DIST_DIR)/*.so $(LIB_DIR)/.
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
	cp $(RESOURCES_DIR)/* $(DIST_DIR)/.
	@echo 'Finished copying resources'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
