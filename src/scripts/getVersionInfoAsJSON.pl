#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# get the JSON
my $programPath = &getPath('VersionInfoTool');
open(JSON_OUTPUT,"$programPath |") or die "Failed to call $programPath $!\n";

# read the output into a string
my $json = join("", <JSON_OUTPUT>);

# close the filehandle
close(JSON_OUTPUT);

# write out the JSON
outputJson($json);
