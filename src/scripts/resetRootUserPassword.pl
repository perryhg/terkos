#!/usr/bin/perl -w

use strict;

################################################################################
# Script for resetting the root user's password.  Can be called either via
# the command line or as a CGI.
#
# If called from the command line, the usage is:
#
#       ./resetRootUserPassword.pl
#
# Regardless of how the script is called, it returns the result as JSON.
################################################################################

require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";
require "/opt/scripts/passwordUtils.pl";

# set the password
my $username = 'root';
my $newPasswordPlainText = 'qwerk';
&setUserPassword($username, $newPasswordPlainText);

# prepare the response
my %response = ();
$response{'statusCode'} = '200';
$response{'statusName'} = 'OK';
$response{'ok'} = 'true';
$response{'message'} = 'password changed';

# create the JSON output
my $jsonOutput = '';
$jsonOutput .= "{\n";
$jsonOutput .= '"ok" : '.$response{'ok'}.',' . "\n";
$jsonOutput .= '"status_code" : '.$response{'statusCode'}.',' . "\n";
$jsonOutput .= '"message" : "'.$response{'message'}.'"' . "\n";
$jsonOutput .= "}\n";

# write out the JSON
if (&isHttpRequest())
   {
   print "HTTP/1.0 $response{'statusCode'} $response{'statusName'}\n";
   }
&outputJson($jsonOutput);
