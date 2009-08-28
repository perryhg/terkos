#!/usr/bin/perl

################################################################################
# Script for initializing TeRKOS.  This script does the following:
# * Applies the audio configuration prefs to the system
# * Applies the LCD configuration prefs to the system
# * Applies the wireless configuration prefs to the system
# * Makes sure the user programs directory exists and is writable
################################################################################

require "/opt/scripts/pathUtils.pl";

# apply audio configuration
my $audioConfigTool = &getPath('AudioConfigTool');
`$audioConfigTool --apply-config`;

# apply LCD configuration
my $lcdConfigTool = &getPath('LCDConfigTool');
`$lcdConfigTool --apply-config`;

# apply wireless configuration
my $wirelessNetworkingConfigTool = &getPath('WirelessNetworkingConfigTool');
`$wirelessNetworkingConfigTool --apply-config`;

# make sure the user programs directory exists and is writable
my $userProgramDir = &getPath('user_programs_dir');
`mkdir -p $userProgramDir`;
`chmod 777 $userProgramDir`;