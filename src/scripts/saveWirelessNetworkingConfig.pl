#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/saveJSONHelper.pl";

my $pathToJsonConfig = &getPath('wireless_networking_config.json');
my $program = &getPath('WirelessNetworkingConfigTool');
&saveJSON($pathToJsonConfig, $program);
