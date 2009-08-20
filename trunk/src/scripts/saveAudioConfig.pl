#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/saveJSONHelper.pl";

my $pathToJsonConfig = &getPath('audio_config.json');
my $program = &getPath('AudioConfigTool');
&saveJSON($pathToJsonConfig, $program);
