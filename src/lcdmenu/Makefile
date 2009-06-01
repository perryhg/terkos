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

LIB_QWERK_DIR=../libqwerk
LIB_QWERK_SRC_DIR=$(LIB_QWERK_DIR)
LIB_QWERK_DIST_DIR=$(LIB_QWERK_DIR)/dist
LIB_QWERK_LIB_SHORT_NAME=qwerk

LIB_JSONCPP_DIR=../jsoncpp
LIB_JSONCPP_SRC_DIR=$(LIB_JSONCPP_DIR)/jsoncpp/include
LIB_JSONCPP_DIST_DIR=$(LIB_JSONCPP_DIR)/dist/arm-oe-linux-uclibcgnueabi
LIB_JSONCPP_LIB_SHORT_NAME=jsoncpp

LIB_UTILITIES_DIR=../libUtilities
LIB_UTILITIES_SRC_DIR=$(LIB_UTILITIES_DIR)/src
LIB_UTILITIES_DIST_DIR=$(LIB_UTILITIES_DIR)/dist
LIB_UTILITIES_LIB_SHORT_NAME=Utilities

LIB_CONFIGURATION_MANAGEMENT_DIR=../libConfigurationManagement
LIB_CONFIGURATION_MANAGEMENT_SRC_DIR=$(LIB_CONFIGURATION_MANAGEMENT_DIR)/src
LIB_CONFIGURATION_MANAGEMENT_DIST_DIR=$(LIB_CONFIGURATION_MANAGEMENT_DIR)/dist
LIB_CONFIGURATION_MANAGEMENT_LIB_SHORT_NAME=ConfigurationManagement

CXX=arm-oe-linux-uclibcgnueabi-g++
AR=arm-oe-linux-uclibcgnueabi-ar

CFLAGS=-fPIC -c -Wall \
       -I$(LIB_CHARACTER_DISPLAY_MENU_SRC_DIR) \
       -I$(LIB_DYNAMIC_CLASS_LOADER_SRC_DIR) \
       -I$(TINY_XML_SRC_DIR) \
       -D "TIXML_USE_TICPP" -D "UNICODE" -D "_UNICODE" -D "DEBUG" -D "_DEBUG" \
       -I$(LIB_QWERK_SRC_DIR) \
       -I$(LIB_JSONCPP_SRC_DIR) \
       -I$(LIB_UTILITIES_SRC_DIR) \
       -I$(LIB_CONFIGURATION_MANAGEMENT_SRC_DIR)
       
LDFLAGS=-L$(LIB_DIR) \
        -l$(LIB_CHARACTER_DISPLAY_MENU_LIB_SHORT_NAME) \
        -l$(LIB_DYNAMIC_CLASS_LOADER_LIB_SHORT_NAME) \
        -l$(TINY_XML_LIB_SHORT_NAME) \
        -l$(LIB_QWERK_LIB_SHORT_NAME) \
        -lpthread \
        -ldl \
        -l$(LIB_JSONCPP_LIB_SHORT_NAME) \
        -l$(LIB_UTILITIES_LIB_SHORT_NAME) \
        -l$(LIB_CONFIGURATION_MANAGEMENT_LIB_SHORT_NAME)

SYSTEM_INFO_MENU_ITEM_ACTIONS_SRCS=$(SRC_DIR)/FirmwareVersionMenuItemAction.cpp \
                                   $(SRC_DIR)/HardwareVersionMenuItemAction.cpp 
SYSTEM_INFO_MENU_ITEM_ACTIONS_OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(SYSTEM_INFO_MENU_ITEM_ACTIONS_SRCS)))
SYSTEM_INFO_MENU_ITEM_ACTIONS_OBJECTS := $(addprefix $(OBJ_DIR)/,$(SYSTEM_INFO_MENU_ITEM_ACTIONS_OBJECTS))

AUDIO_MENU_ITEM_ACTIONS_SRCS=$(SRC_DIR)/ToggleAudioAlertsMenuItemAction.cpp \
                             $(SRC_DIR)/SetVolumeMenuItemAction.cpp \
                             $(SRC_DIR)/ResetAudioMenuItemAction.cpp
AUDIO_MENU_ITEM_ACTIONS_OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(AUDIO_MENU_ITEM_ACTIONS_SRCS)))
AUDIO_MENU_ITEM_ACTIONS_OBJECTS := $(addprefix $(OBJ_DIR)/,$(AUDIO_MENU_ITEM_ACTIONS_OBJECTS))

NETWORK_MENU_ITEM_ACTIONS_SRCS=$(SRC_DIR)/ToggleWirelessStartupUponBootMenuItemAction.cpp \
                               $(SRC_DIR)/ResetWirelessNetworkingMenuItemAction.cpp
NETWORK_MENU_ITEM_ACTIONS_OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(NETWORK_MENU_ITEM_ACTIONS_SRCS)))
NETWORK_MENU_ITEM_ACTIONS_OBJECTS := $(addprefix $(OBJ_DIR)/,$(NETWORK_MENU_ITEM_ACTIONS_OBJECTS))

SERVO_MENU_ITEM_ACTIONS_SRCS=$(SRC_DIR)/ViewAndEditServoConfigsMenuItemAction.cpp
SERVO_MENU_ITEM_ACTIONS_OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(SERVO_MENU_ITEM_ACTIONS_SRCS)))
SERVO_MENU_ITEM_ACTIONS_OBJECTS := $(addprefix $(OBJ_DIR)/,$(SERVO_MENU_ITEM_ACTIONS_OBJECTS))

LCD_MENU_SRCS=$(SRC_DIR)/LCDCharacterDisplay.cpp \
              $(SRC_DIR)/LCDMenu.cpp
LCD_MENU_OBJECTS=$(patsubst %.cpp,%.o,$(notdir $(LCD_MENU_SRCS)))
LCD_MENU_OBJECTS := $(addprefix $(OBJ_DIR)/,$(LCD_MENU_OBJECTS))

#-----------------------------------------------------------------------------------------------------------------------

default: all

build: LCDMenu

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

libSystemInfoMenuItemActions.so: $(OBJ_DIR) $(LIB_DIR) $(SYSTEM_INFO_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libSystemInfoMenuItemActions.so -o $(LIB_DIR)/libSystemInfoMenuItemActions.so \
	                                                              $(SYSTEM_INFO_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libAudioMenuItemActions.so: $(OBJ_DIR) $(LIB_DIR) $(AUDIO_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libAudioMenuItemActions.so -o $(LIB_DIR)/libAudioMenuItemActions.so \
	                                                         $(AUDIO_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libServoMenuItemActions.so: $(OBJ_DIR) $(LIB_DIR) $(SERVO_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libServoMenuItemActions.so -o $(LIB_DIR)/libServoMenuItemActions.so \
	                                                         $(SERVO_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libNetworkMenuItemActions.so: $(OBJ_DIR) $(LIB_DIR) $(NETWORK_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -shared -Wl,-soname,libNetworkMenuItemActions.so -o $(LIB_DIR)/libNetworkMenuItemActions.so \
	                                                         $(NETWORK_MENU_ITEM_ACTIONS_OBJECTS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

LCDMenu: $(EXE_DIR) \
         libConfigurationManagement.so \
         libCharacterDisplayMenu.so \
         libqwerk.so \
         libSystemInfoMenuItemActions.so \
         libAudioMenuItemActions.so \
         libNetworkMenuItemActions.so \
         libServoMenuItemActions.so \
         $(LCD_MENU_OBJECTS)
	@echo 'Building target: $@'
	$(CXX) -o $(EXE_DIR)/LCDMenu $(LCD_MENU_OBJECTS) $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

libCharacterDisplayMenu.so: $(LIB_DIR)
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_CHARACTER_DISPLAY_MENU_DIR) CXX=$(CXX)
	cp $(LIB_CHARACTER_DISPLAY_MENU_DIST_DIR)/*.so $(LIB_DIR)/.
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

libConfigurationManagement.so: $(LIB_DIR)
	@echo 'Building target: $@'
	$(MAKE) -C $(LIB_CONFIGURATION_MANAGEMENT_DIR) CXX=$(CXX)
	cp $(LIB_CONFIGURATION_MANAGEMENT_DIST_DIR)/*.so $(LIB_DIR)/.
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
	cp $(LIB_CONFIGURATION_MANAGEMENT_DIST_DIR)/*.json $(DIST_DIR)/.
	@echo 'Finished copying resources'
	@echo ' '

#-----------------------------------------------------------------------------------------------------------------------

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
