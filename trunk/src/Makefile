DIST_DIR=dist

LIB_CHARACTER_DISPLAY_MENU_DIR=libCharacterDisplayMenu
LIB_CONFIGURATION_MANAGEMENT_DIR=libConfigurationManagement
LIB_DYNAMIC_CLASS_LOADER_DIR=libDynamicClassLoader
LIB_QWERK_DIR=libqwerk
LIB_UTILITIES_DIR=libUtilities

LCD_MENU_DIR=lcdmenu
LIBMMAP_DIR=libmmap
FPGAC_DIR=util/fpgac
TEST_DIR=factory/test
LOGIC_DIR=logic/vexpro
SCRIPTS_DIR=scripts
WWW_DIR=www

DIST_SRV_DIR=$(DIST_DIR)/srv
DIST_CGI_DIR=$(DIST_SRV_DIR)/www/cgi-bin
DIST_BIN_DIR=$(DIST_DIR)/opt/bin
DIST_AUDIO_DIR=$(DIST_DIR)/opt/media/audio
DIST_CONFIG_DIR=$(DIST_DIR)/opt/config
DIST_LIB_DIR=$(DIST_DIR)/opt/lib
DIST_SCRIPTS_DIR=$(DIST_DIR)/opt/scripts
DIST_USR_BIN_DIR=$(DIST_DIR)/opt/usr/bin
#-----------------------------------------------------------------------------------------------------------------------

default: all

all: clean dist

#-----------------------------------------------------------------------------------------------------------------------

$(DIST_SRV_DIR):
	mkdir -p $@

$(DIST_CGI_DIR):
	mkdir -p $@

$(DIST_BIN_DIR):
	mkdir -p $@

$(DIST_AUDIO_DIR):
	mkdir -p $@

$(DIST_CONFIG_DIR):
	mkdir -p $@

$(DIST_LIB_DIR):
	mkdir -p $@

$(DIST_SCRIPTS_DIR):
	mkdir -p $@

$(DIST_USR_BIN_DIR):
	mkdir -p $@


#-----------------------------------------------------------------------------------------------------------------------

dist: $(DIST_CGI_DIR) \
      $(DIST_BIN_DIR) \
      $(DIST_AUDIO_DIR) \
      $(DIST_CONFIG_DIR) \
      $(DIST_LIB_DIR) \
      $(DIST_SCRIPTS_DIR) \
      $(DIST_USR_BIN_DIR)
	@echo 'Making lcdmenu...'
	$(MAKE) -C $(LCD_MENU_DIR) all

	@echo 'Making fpgac...'
	$(MAKE) -C $(LIBMMAP_DIR) all
	$(MAKE) -C $(FPGAC_DIR) all

	@echo 'Making test...'
	$(MAKE) -C $(TEST_DIR) all

	@echo 'Copying binaries...'
	cp $(LCD_MENU_DIR)/dist/LCDMenu $(DIST_BIN_DIR)
	cp $(FPGAC_DIR)/fpgac $(DIST_BIN_DIR)
	cp $(TEST_DIR)/test $(DIST_BIN_DIR)
	cp $(LIB_CONFIGURATION_MANAGEMENT_DIR)/dist/*Tool $(DIST_BIN_DIR)

	@echo 'Copying configs...'
	cp $(LCD_MENU_DIR)/dist/*.xml $(DIST_CONFIG_DIR)
	cp $(LIB_CONFIGURATION_MANAGEMENT_DIR)/dist/*.json $(DIST_CONFIG_DIR)

	@echo 'Copying FPGA logic...'
	cp $(LOGIC_DIR)/vexpro.bit $(DIST_LIB_DIR)
	cp $(LOGIC_DIR)/../vexprotest/vexpro.bit $(DIST_LIB_DIR)/vexprotest.bit

	@echo 'Copying libraries...'
	cp $(LCD_MENU_DIR)/dist/*.so $(DIST_LIB_DIR)

	@echo 'Copying media...'
	cp $(LIB_CONFIGURATION_MANAGEMENT_DIR)/dist/*.wav $(DIST_AUDIO_DIR)/
	
	@echo 'Copying scripts...'
	cp $(SCRIPTS_DIR)/*.pl $(DIST_SCRIPTS_DIR)

	@echo 'Copying www files...'
	cp -R $(WWW_DIR) $(DIST_SRV_DIR)

	@echo 'Symlinking CGIs...'
	ln -s /opt/scripts/getAudioConfigAsJSON.pl                $(DIST_CGI_DIR)/getAudioConfigAsJSON.pl
	ln -s /opt/scripts/getLCDConfigAsJSON.pl                  $(DIST_CGI_DIR)/getLCDConfigAsJSON.pl
	ln -s /opt/scripts/getDiskUsageAsJSON.pl                  $(DIST_CGI_DIR)/getDiskUsageAsJSON.pl
	ln -s /opt/scripts/getMemoryUsageAsJSON.pl                $(DIST_CGI_DIR)/getMemoryUsageAsJSON.pl
	ln -s /opt/scripts/getPowerInfoAsJSON.pl                  $(DIST_CGI_DIR)/getPowerInfoAsJSON.pl
	ln -s /opt/scripts/getTerkOSConfig.pl                     $(DIST_CGI_DIR)/getTerkOSConfig.pl
	ln -s /opt/scripts/getUserProgramsConfigAsJSON.pl         $(DIST_CGI_DIR)/getUserProgramsConfigAsJSON.pl
	ln -s /opt/scripts/getVersionInfoAsJSON.pl                $(DIST_CGI_DIR)/getVersionInfoAsJSON.pl
	ln -s /opt/scripts/getWirelessNetworkingConfigAsJSON.pl   $(DIST_CGI_DIR)/getWirelessNetworkingConfigAsJSON.pl
	ln -s /opt/scripts/getWirelessNetworkingStatusAsJSON.pl   $(DIST_CGI_DIR)/getWirelessNetworkingStatusAsJSON.pl
	ln -s /opt/scripts/getWirelessNetworksAsJSON.pl           $(DIST_CGI_DIR)/getWirelessNetworksAsJSON.pl
	ln -s /opt/scripts/playSampleSound.pl                     $(DIST_CGI_DIR)/playSampleSound.pl
	ln -s /opt/scripts/resetPreferences.pl                    $(DIST_CGI_DIR)/resetPreferences.pl
	ln -s /opt/scripts/saveAudioConfig.pl                     $(DIST_CGI_DIR)/saveAudioConfig.pl
	ln -s /opt/scripts/saveLCDConfig.pl                       $(DIST_CGI_DIR)/saveLCDConfig.pl
	ln -s /opt/scripts/saveUserProgramsConfig.pl              $(DIST_CGI_DIR)/saveUserProgramsConfig.pl
	ln -s /opt/scripts/saveWirelessNetworkingConfig.pl        $(DIST_CGI_DIR)/saveWirelessNetworkingConfig.pl
	ln -s /opt/scripts/setHttpBasicAuthPassword.pl            $(DIST_CGI_DIR)/setHttpBasicAuthPassword.pl
	ln -s /opt/scripts/setRootUserPassword.pl                 $(DIST_CGI_DIR)/setRootUserPassword.pl
	ln -s /opt/scripts/setTerkOSConfig.pl                     $(DIST_CGI_DIR)/setTerkOSConfig.pl

	@echo 'Removing .svn directories...'
	rm -rf `find $(DIST_DIR) -type d -name .svn`

#-----------------------------------------------------------------------------------------------------------------------

clean:
	$(MAKE) -C $(LIB_CHARACTER_DISPLAY_MENU_DIR) clean
	$(MAKE) -C $(LIB_CONFIGURATION_MANAGEMENT_DIR) clean
	$(MAKE) -C $(LIB_DYNAMIC_CLASS_LOADER_DIR) clean
	$(MAKE) -C $(LIB_QWERK_DIR) clean
	$(MAKE) -C $(LIB_UTILITIES_DIR) clean
	$(MAKE) -C $(LCD_MENU_DIR) clean
	-rm -rf $(DIST_DIR)

#-----------------------------------------------------------------------------------------------------------------------
