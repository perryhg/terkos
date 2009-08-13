#!/usr/bin/perl

################################################################################
# Program for setting a the root user's password.  Can be # called either via
# the command line or as a CGI.
#
# If called from the command line, the usage is:
#
#       ./setRootUserPassword.pl --newPassword=NEW_PASSWORD
#
# If called as a CGI, it expects there to be a parameter named newPassword whose
# value is the new password.
#
# Regardless of how the script is called, it returns the result as JSON.
################################################################################

require "/opt/scripts/commandLineUtils.pl";
require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# parse command line switches or HTTP request params
my %arguments = ();
if (isHttpRequest())
   {
   %arguments = parseHttpRequestParameters();
   }
else
   {
   %arguments = parseCommandLineSwitches();
   }

# create the JSON output
my %response = ();
if (exists($arguments{'newPassword'}))
   {
   $response{'statusCode'} = '200';
   $response{'statusName'} = 'OK';
   $response{'ok'} = 'true';
   $response{'message'} = 'password changed';
   }
else
   {
   $response{'statusCode'} = '400';
   $response{'statusName'} = 'Bad Request';
   $response{'ok'} = 'false';
   $response{'message'} = 'missing parameter: newPassword';
   }

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
