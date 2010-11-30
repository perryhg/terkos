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

require "/opt/scripts/wirelessNetworkingUtils.pl";

my $interfaceName = "wlan0";

# enable ad hoc
enableAdHocWirelessNetworking($interfaceName);

# sleep a bit to let it take effect
sleep 7;

# print the status
printWirelessNetworkingStatusAsJSON($interfaceName);

