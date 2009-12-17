#!/usr/bin/perl

#
# This file is part of Terk and TerkOS.
#
# All Terk and TerkOS source code is provided under the terms of the
# GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
# Those wishing to use Terk and TerkOS source code, software and/or
# technologies under different licensing terms should contact us at
# telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
# all portions of the Terk and TerkOS codebase presented here.
#

require "/opt/scripts/pathUtils.pl";

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
