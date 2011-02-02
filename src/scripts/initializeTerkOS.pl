#!/usr/bin/perl -w

#
# This file is part of Terk and TerkOS.
#
# All Terk and TerkOS source code is provided under the terms of the
# GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
# Those wishing to use Terk and TerkOS source code, software and/or
# technologies under different licensing terms should contact us at
# telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
# all portions of the Terk and TerkOS codebase presented here.
#

################################################################################
# Script for initializing TeRKOS.  This script does the following:
# * Applies the audio configuration prefs to the system
# * Applies the wireless configuration prefs to the system
# * Makes sure the user programs directory exists and is writable
# * Generates the WPA Supplicant configuration file for ad-hoc wireless networking.
################################################################################

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/wirelessNetworkingUtils.pl";

# apply audio configuration
my $audioConfigTool = &getPath('AudioConfigTool');
`$audioConfigTool --apply-config`;

# apply wireless configuration
my $wirelessNetworkingConfigTool = &getPath('WirelessNetworkingConfigTool');
`$wirelessNetworkingConfigTool --apply-config`;

# make sure the user programs directory exists and is writable
my $userProgramDir = &getPath('user_programs_dir');
`mkdir -p $userProgramDir`;
`chmod 777 $userProgramDir`;

# generate the WPA Supplicant Ad Hoc conf file
&generateWPASupplicantConfForAdHoc();