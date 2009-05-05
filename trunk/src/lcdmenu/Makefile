SRC_DIR=src
BUILD_DIR=build
RESOURCES_DIR=resources
OBJ_DIR=$(BUILD_DIR)/objects
LIB_DIR=$(BUILD_DIR)/libraries
EXE_DIR=$(BUILD_DIR)/executables
DIST_DIR=dist

LIB_CHARACTER_DISPLAY_MENU_DIR=../libCharacterDisplayMenu
LIB_CHARACTER_DISPLAY_MENU_SRC_DIR=$(LIB_CHARACTER_DISPLAY_MENU_DIR)/src
LIB_CHARACTER_DISPLAY_MENU_DIST_DIR=$(LIB_CHARACTER_DISPLAY_MENU_DIR)/dist
LIB_CHARACTER_DISPLAY_MENU_LIB_SHORT_NAME=CharacterDisplayMenu

LIB_DYNAMIC_CLASS_LOADER_DIR=../libDynamicClassLoader
LIB_DYNAMIC_CLASS_LOADER_SRC_DIR=$(LIB_DYNAMIC_CLASS_LOADER_DIR)/src
LIB_DYNAMIC_CLASS_LOADER_DIST_DIR=$(LIB_DYNAMIC_CLASS_LOADER_DIR)/dist
LIB_DYNAMIC_CLASS_LOADER_LIB_SHORT_NAME=DynamicClassLoader

TINY_XML_DIR=../tinyxml++
TINY_XML_SRC_DIR=$(TINY_XML_DIR)/ticpp
TINY_XML_DIST_DIR=$(TINY_XML_DIR)/dist
TINY_XML_LIB_SHORT_NAME=ticppd

#CXX=arm-linux-gnueabi-g++

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_CHARACTER_DISPLAY_MENU_SRC_DIR) \
       -I$(LIB_DYNAMIC_CLASS_LOADER_SRC_DIR) \
       -I$(TINY_XML_SRC_DIR) \
       -D "TIXML_USE_TICPP" -D "UNICODE" -D "_UNICODE" -D "DEBUG" -D "_DEBUG"
LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_CHARACTER_DISPLAY_MENU_LIB_SHORT_NAME) \
        -l$(LIB_DYNAMIC_CLASS_LOADER_LIB_SHORT_NAME) \
        -l$(TINY_XML_LIB_SHORT_NAME) \
        -ldl

#-----------------------------------------------------------------------------------------------------------------------

default: all

#-----------------------------------------------------------------------------------------------------------------------

build: init \
       LCDMenu

all: build dist

#-----------------------------------------------------------------------------------------------------------------------

init:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(LIB_DIR)
	mkdir -p $(EXE_DIR)

#-----------------------------------------------------------------------------------------------------------------------

FirmwareVersionMenuItemAction.o: init $(SRC_DIR)/FirmwareVersionMenuItemAction.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/FirmwareVersionMenuItemAction.cpp -o $(OBJ_DIR)/FirmwareVersionMenuItemAction.o
	@echo 'Finished building target: $@'
	@echo ' '
	
libFirmwareVersionMenuItemAction.so: libCharacterDisplayMenu.so FirmwareVersionMenuItemAction.o
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libFirmwareVersionMenuItemAction.so -o $(LIB_DIR)/libFirmwareVersionMenuItemAction.so $(OBJ_DIR)/FirmwareVersionMenuItemAction.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

HardwareVersionMenuItemAction.o: init $(SRC_DIR)/HardwareVersionMenuItemAction.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/HardwareVersionMenuItemAction.cpp -o $(OBJ_DIR)/HardwareVersionMenuItemAction.o
	@echo 'Finished building target: $@'
	@echo ' '
	
libHardwareVersionMenuItemAction.so: libCharacterDisplayMenu.so HardwareVersionMenuItemAction.o
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libHardwareVersionMenuItemAction.so -o $(LIB_DIR)/libHardwareVersionMenuItemAction.so $(OBJ_DIR)/HardwareVersionMenuItemAction.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

LCDCharacterDisplay.o: init $(SRC_DIR)/LCDCharacterDisplay.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/LCDCharacterDisplay.cpp -o $(OBJ_DIR)/LCDCharacterDisplay.o
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

LCDMenu.o: init $(SRC_DIR)/LCDMenu.cpp
	@echo 'Building target: $@'
	$(CXX) $(CFLAGS) $(SRC_DIR)/LCDMenu.cpp -o $(OBJ_DIR)/LCDMenu.o
	@echo 'Finished building target: $@'
	@echo ' '

LCDMenu: libCharacterDisplayMenu.so \
         libFirmwareVersionMenuItemAction.so \
         libHardwareVersionMenuItemAction.so \
         LCDCharacterDisplay.o \
         LCDMenu.o
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/LCDMenu $(OBJ_DIR)/LCDCharacterDisplay.o \
                                $(OBJ_DIR)/LCDMenu.o \
                                $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libCharacterDisplayMenu.so: init
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_CHARACTER_DISPLAY_MENU_DIR) CXX=$(CXX)
	cp $(LIB_CHARACTER_DISPLAY_MENU_DIST_DIR)/*.so $(LIB_DIR)/.
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
	cp $(RESOURCES_DIR)/menu.xml $(DIST_DIR)/.
	@echo 'Finished copying resources'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
