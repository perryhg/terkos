#!/usr/bin/perl

require "/opt/scripts/wirelessNetworkingUtils.pl";

my $interfaceName = "wlan0";

# enable the wireless interface
enableWirelessNetworking($interfaceName);

# print the status
printWirelessNetworkingStatusAsJSON($interfaceName);
