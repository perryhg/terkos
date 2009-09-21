#!/usr/bin/perl

require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# get the JSON
my $programPath = &getPath('TerkOSConfigTool') . ' --json';
open(JSON_OUTPUT,"$programPath |") or die "Failed to call $programPath $!\n";

# read the output into a string
my $json = join("", <JSON_OUTPUT>);

# close the filehandle
close(JSON_OUTPUT);

if (&isHttpRequest())
   {
   &printContentDispositionHeader('terkos_config.json');
   }

# write out the JSON
outputJson($json);
