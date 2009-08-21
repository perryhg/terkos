#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/jsonUtils.pl";

my $pathToJsonConfig = &getPath('lcd_config.json');
outputJsonFile($pathToJsonConfig);
