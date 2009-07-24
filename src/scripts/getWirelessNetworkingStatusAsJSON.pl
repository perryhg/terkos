#!/usr/bin/perl

require "/opt/scripts/wirelessNetworkingUtils.pl";

my $interfaceName = "wlan0";

printWirelessNetworkingStatusAsJSON($interfaceName);
