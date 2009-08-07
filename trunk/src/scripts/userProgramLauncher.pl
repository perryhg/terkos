#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/httpUtils.pl";

# make sure the LD_LIBRARY_PATH is set correctly since we're calling one of our C++ apps
if (&isHttpRequest())
   {
   &setUpLdLibraryPath();
   }

# get the JSON
my $programPath = &getPath('UserProgramsConfigTool');
open(PROGRAM_OUTPUT,"$programPath --program |") or die "Failed to call $programPath $!\n";

# read the output into a string
my $pathToProgram = join("", <PROGRAM_OUTPUT>);
chomp $pathToProgram;

# close the filehandle
close(PROGRAM_OUTPUT);

# exec the program
if ($pathToProgram)
   {
   print "About to call [$pathToProgram]...\n";
   exec($pathToProgram);
   }
