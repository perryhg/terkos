#!/usr/bin/perl

require "wirelessNetworkingUtils.pl";

my $interfaceName = "wlan0";

printWirelessNetworkingStatusAsJSON($interfaceName);
