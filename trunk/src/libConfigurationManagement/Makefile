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

LIB_QWERK_DIR=../libqwerk
LIB_QWERK_SRC_DIR=$(LIB_QWERK_DIR)
LIB_QWERK_DIST_DIR=$(LIB_QWERK_DIR)
LIB_QWERK_LIB_SHORT_NAME=qwerk

CXX=arm-oe-linux-uclibcgnueabi-g++

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(SRCS)))

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_CONFIG_SRC_DIR) \
       -I$(LIB_QWERK_SRC_DIR)

LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_CONFIG_LIB_SHORT_NAME) \
        -l$(LIB_QWERK_LIB_SHORT_NAME) \
        -lpthread

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: libConfigurationManagement.so

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)

#-----------------------------------------------------------------------------------------------------------------------

$(OBJECTS): %.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $(OBJ_DIR)/$@

#-----------------------------------------------------------------------------------------------------------------------

# TODO: I can (probably?) get rid of these lib flags once libqwerk is a shared object
libConfigurationManagement.so: init libconfig++.so libqwerk.so $(OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libConfigurationManagement.so -o $(LIB_DIR)/libConfigurationManagement.so $(OBJ_DIR)/*.o -L$(LIB_DIR) -l$(LIB_QWERK_LIB_SHORT_NAME) -lpthread
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libconfig++.so: init
	@echo 'Building target: $@'
	cp $(LIB_CONFIG_DIST_DIR)/lib$(LIB_CONFIG_LIB_SHORT_NAME).so $(LIB_DIR)/.
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

# TODO: switch to .so
libqwerk.so: init
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_QWERK_DIR) CXX=$(CXX) AR=$(AR)
	cp $(LIB_QWERK_DIST_DIR)/*.a $(LIB_DIR)/.
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
