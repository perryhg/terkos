#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/saveJSONHelper.pl";

my $pathToJsonConfig = &getPath('lcd_config.json');
my $program = &getPath('LCDConfigTool');
&saveJSON($pathToJsonConfig, $program);
