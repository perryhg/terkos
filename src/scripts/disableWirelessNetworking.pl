#!/usr/bin/perl

require "/opt/scripts/wirelessNetworkingUtils.pl";

my $interfaceName = "wlan0";

# call ifdown shut down the wireless interface
open(IFDOWN_STATUS, "/sbin/ifdown $interfaceName |") or die "Failed to call ifdown $!\n";
close(IFDOWN_STATUS);

# print the status
printWirelessNetworkingStatusAsJSON($interfaceName);
