SRC_DIR=src
BUILD_DIR=build
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
DIST_DIR=dist
RESOURCES_DIR=resources
LIBRARY_NAME=libCharacterDisplayMenu.so

LIB_DYNAMIC_CLASS_LOADER_DIR=../libDynamicClassLoader
LIB_DYNAMIC_CLASS_LOADER_SRC_DIR=$(LIB_DYNAMIC_CLASS_LOADER_DIR)/src
LIB_DYNAMIC_CLASS_LOADER_DIST_DIR=$(LIB_DYNAMIC_CLASS_LOADER_DIR)/dist
LIB_DYNAMIC_CLASS_LOADER_LIB_SHORT_NAME=DynamicClassLoader

TINY_XML_DIR=../tinyxml++
TINY_XML_SRC_DIR=$(TINY_XML_DIR)/ticpp
TINY_XML_DIST_DIR=$(TINY_XML_DIR)/dist
TINY_XML_LIB_SHORT_NAME=ticppd

#CXX=arm-linux-g++
CFLAGS=-fPIC -c -Wall -I$(LIB_DYNAMIC_CLASS_LOADER_SRC_DIR) -I$(TINY_XML_SRC_DIR) -D "TIXML_USE_TICPP" -D "UNICODE" -D "_UNICODE" -D "DEBUG" -D "_DEBUG"
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

DefaultMenuItemAction.o: init $(SRC_DIR)/DefaultMenuItemAction.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/DefaultMenuItemAction.cpp -o $(OBJ_DIR)/DefaultMenuItemAction.o
	@echo 'Finished building target: $@'
	@echo ' '

DefaultMenuStatusManager.o: init $(SRC_DIR)/DefaultMenuStatusManager.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/DefaultMenuStatusManager.cpp -o $(OBJ_DIR)/DefaultMenuStatusManager.o
	@echo 'Finished building target: $@'
	@echo ' '

CharacterDisplayMenuItemAction.o: init $(SRC_DIR)/CharacterDisplayMenuItemAction.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/CharacterDisplayMenuItemAction.cpp -o $(OBJ_DIR)/CharacterDisplayMenuItemAction.o -L$(LIB_DIR)
	@echo 'Finished building target: $@'
	@echo ' '

TwoOptionMenuItemAction.o: init $(SRC_DIR)/TwoOptionMenuItemAction.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/TwoOptionMenuItemAction.cpp -o $(OBJ_DIR)/TwoOptionMenuItemAction.o -L$(LIB_DIR)
	@echo 'Finished building target: $@'
	@echo ' '

CharacterDisplayMenu.o: init $(SRC_DIR)/CharacterDisplayMenu.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/CharacterDisplayMenu.cpp -o $(OBJ_DIR)/CharacterDisplayMenu.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

$(LIBRARY_NAME): libDynamicClassLoader.so \
      			  libticpp.so \
         		  DefaultMenuItemAction.o \
         		  DefaultMenuStatusManager.o \
         		  CharacterDisplayMenuItemAction.o \
         		  TwoOptionMenuItemAction.o \
         		  CharacterDisplayMenu.o
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,$(LIBRARY_NAME) -o $(LIB_DIR)/$(LIBRARY_NAME) \
                           						    $(OBJ_DIR)/DefaultMenuItemAction.o \
                                                 $(OBJ_DIR)/DefaultMenuStatusManager.o \
                                                 $(OBJ_DIR)/CharacterDisplayMenuItemAction.o \
                                                 $(OBJ_DIR)/TwoOptionMenuItemAction.o \
                                                 $(OBJ_DIR)/CharacterDisplayMenu.o \
                                                 -L$(LIB_DIR) \
                                                 -l$(LIB_DYNAMIC_CLASS_LOADER_LIB_SHORT_NAME) \
                                                 -l$(TINY_XML_LIB_SHORT_NAME) \
                                                 -ldl
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libDynamicClassLoader.so: init
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_DYNAMIC_CLASS_LOADER_DIR) CXX=$(CXX)
	cp $(LIB_DYNAMIC_CLASS_LOADER_DIST_DIR)/*.so $(LIB_DIR)/.
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libticpp.so: init
	@echo 'Building target: $@'
	$(MAKE) -C $(TINY_XML_SRC_DIR) CXX=$(CXX)
	cp $(TINY_XML_DIST_DIR)/*.so $(LIB_DIR)/.
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

dist: 
	mkdir -p $(DIST_DIR)
	@echo 'Copying shared libraries to $(DIST_DIR)...'
	cp $(LIB_DIR)/*.so $(DIST_DIR)/.
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
