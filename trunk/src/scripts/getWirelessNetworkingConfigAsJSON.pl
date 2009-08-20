#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/jsonUtils.pl";

my $pathToJsonConfig = &getPath('wireless_networking_config.json');
outputJsonFile($pathToJsonConfig);
