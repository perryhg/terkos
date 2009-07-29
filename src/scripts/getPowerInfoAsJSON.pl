#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# make sure the LD_LIBRARY_PATH is set correctly since we're calling one of our C++ apps
if (&isHttpRequest())
   {
   &setUpLdLibraryPath();
   }

# get the JSON
my $programPath = &getPath('PowerInfoTool');
open(JSON_OUTPUT,"$programPath |") or die "Failed to call $programPath $!\n";

# read the output into a string
my $json = join("", <JSON_OUTPUT>);

# close the filehandle
close(JSON_OUTPUT);

# write out the JSON
outputJson($json);
