#!/usr/bin/perl

################################################################################
# Program for restarting the web server.  Prints "1" if the web server was
# restarted successfully, "0" otherwise.
################################################################################

require "/opt/scripts/httpUtils.pl";

my $success = &restartWebServer();
print "$success\n";