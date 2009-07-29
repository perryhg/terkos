SRC_DIR=src
BUILD_DIR=build
RESOURCES_DIR=resources
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
EXE_SRC_DIR=src-exe
EXE_OBJ_DIR=$(BUILD_DIR)/objects
DIST_DIR=dist
LIBRARY_NAME=libConfigurationManagement.so
AUDIO_CONFIG_TOOL_NAME=AudioConfigTool
POWER_INFO_TOOL_NAME=PowerInfoTool
VERSION_INFO_TOOL_NAME=VersionInfoTool
WIRELESS_NETWORKING_CONFIG_TOOL_NAME=WirelessNetworkingConfigTool

LIB_JSONCPP_DIR=../jsoncpp
LIB_JSONCPP_SRC_DIR=$(LIB_JSONCPP_DIR)/jsoncpp/include
LIB_JSONCPP_DIST_DIR=$(LIB_JSONCPP_DIR)/dist/arm-oe-linux-uclibcgnueabi
LIB_JSONCPP_LIB_SHORT_NAME=jsoncpp

LIB_UTILITIES_DIR=../libUtilities
LIB_UTILITIES_SRC_DIR=$(LIB_UTILITIES_DIR)/src
LIB_UTILITIES_DIST_DIR=$(LIB_UTILITIES_DIR)/dist
LIB_UTILITIES_LIB_SHORT_NAME=Utilities

LIB_QWERK_DIR=../libqwerk
LIB_QWERK_SRC_DIR=$(LIB_QWERK_DIR)
LIB_QWERK_DIST_DIR=$(LIB_QWERK_DIR)/dist
LIB_QWERK_LIB_SHORT_NAME=qwerk

LIB_PSTREAMS_DIR=../pstreams
LIB_PSTREAMS_SRC_DIR=$(LIB_PSTREAMS_DIR)/src

CXX=arm-oe-linux-uclibcgnueabi-g++
AR=arm-oe-linux-uclibcgnueabi-ar

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_JSONCPP_SRC_DIR) \
       -I$(LIB_UTILITIES_SRC_DIR) \
       -I$(LIB_QWERK_SRC_DIR) \
       -I$(LIB_PSTREAMS_SRC_DIR)

LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_JSONCPP_LIB_SHORT_NAME) \
        -l$(LIB_UTILITIES_LIB_SHORT_NAME) \
        -l$(LIB_QWERK_LIB_SHORT_NAME) \
        -lpthread

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(SRCS)))
OBJECTS := $(addprefix $(OBJ_DIR)/,$(OBJECTS))

EXE_SRCS=$(wildcard $(EXE_SRC_DIR)/*.cpp)
EXE_OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(EXE_SRCS)))
EXE_OBJECTS := $(addprefix $(EXE_OBJ_DIR)/,$(EXE_OBJECTS))
#-----------------------------------------------------------------------------------------------------------------------

default: all

build: $(LIBRARY_NAME) \
       $(AUDIO_CONFIG_TOOL_NAME) \
       $(POWER_INFO_TOOL_NAME) \
       $(VERSION_INFO_TOOL_NAME) \
       $(WIRELESS_NETWORKING_CONFIG_TOOL_NAME)

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

$(EXE_OBJ_DIR)/%.o: $(EXE_SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@
	
#-----------------------------------------------------------------------------------------------------------------------

$(AUDIO_CONFIG_TOOL_NAME): $(LIBRARY_NAME) $(EXE_DIR) $(EXE_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/$@ $(EXE_OBJ_DIR)/$@.o $(LDFLAGS) -lConfigurationManagement
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

$(POWER_INFO_TOOL_NAME): $(LIBRARY_NAME) $(EXE_DIR) $(EXE_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/$@ $(EXE_OBJ_DIR)/$@.o $(LDFLAGS) -lConfigurationManagement
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

$(VERSION_INFO_TOOL_NAME): $(LIBRARY_NAME) $(EXE_DIR) $(EXE_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/$@ $(EXE_OBJ_DIR)/$@.o $(LDFLAGS) -lConfigurationManagement
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

$(WIRELESS_NETWORKING_CONFIG_TOOL_NAME): $(LIBRARY_NAME) $(EXE_DIR) $(EXE_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/$@ $(EXE_OBJ_DIR)/$@.o $(LDFLAGS) -lConfigurationManagement
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

$(LIBRARY_NAME): $(OBJ_DIR) $(LIB_DIR) $(OBJECTS) libjsoncpp.so libUtilities.so libqwerk.so
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,$(LIBRARY_NAME) -o $(LIB_DIR)/$(LIBRARY_NAME) $(OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libjsoncpp.so: $(LIB_DIR)
	@echo 'Building target: $@'
	cp $(LIB_JSONCPP_DIST_DIR)/lib$(LIB_JSONCPP_LIB_SHORT_NAME).so $(LIB_DIR)/.
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libUtilities.so: $(LIB_DIR)
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_UTILITIES_DIR) CXX=$(CXX)
	cp $(LIB_UTILITIES_DIST_DIR)/lib$(LIB_UTILITIES_LIB_SHORT_NAME).so $(LIB_DIR)/.
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libqwerk.so: $(LIB_DIR)
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_QWERK_DIR) CXX=$(CXX) AR=$(AR)
	cp $(LIB_QWERK_DIST_DIR)/*.so $(LIB_DIR)/.
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
