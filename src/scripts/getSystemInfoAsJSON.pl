#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# make sure the LD_LIBRARY_PATH is set correctly since we're calling one of our C++ apps
if (&isHttpRequest())
   {
   &setUpLdLibraryPath();
   }

# get the system info as JSON
my $pathToSystemInfoTool = &getPath('SystemInfoTool');
open(SYSTEM_INFO,"$pathToSystemInfoTool |") or die "Failed to call $pathToSystemInfoTool $!\n";

# read the output into a string
my $json = join("", <SYSTEM_INFO>);

# close the filehandle
close(SYSTEM_INFO);

# write out the JSON
outputJson($json);
