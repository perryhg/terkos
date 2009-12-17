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

################################################################################
# Script for restarting the web server.  Prints "1" if the web server was
# restarted successfully, "0" otherwise.
################################################################################

require "/opt/scripts/httpUtils.pl";

my $success = &restartWebServer();
print "$success\n";