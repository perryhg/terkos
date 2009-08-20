#!/usr/bin/perl

require "/opt/scripts/wirelessNetworkingUtils.pl";

my $interfaceName = "wlan0";

# disable the wireless interface
disableWirelessNetworking($interfaceName);

# print the status
printWirelessNetworkingStatusAsJSON($interfaceName);
