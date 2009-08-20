#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/saveJSONHelper.pl";

my $pathToJsonConfig = &getPath('user_programs_config.json');
my $program = &getPath('UserProgramsConfigTool');
&saveJSON($pathToJsonConfig, $program);
