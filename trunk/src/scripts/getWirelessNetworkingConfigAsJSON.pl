#!/usr/bin/perl

require "pathUtils.pl";
require "jsonUtils.pl";

my $pathToJsonConfig = &getPath('wireless_networking_config.json');
outputJsonFile($pathToJsonConfig);
