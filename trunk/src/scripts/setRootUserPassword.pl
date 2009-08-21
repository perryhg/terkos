#!/usr/bin/perl

################################################################################
# Program for setting a the root user's password.  Can be called either via
# the command line or as a CGI.
#
# If called from the command line, the usage is:
#
#       ./setRootUserPassword.pl --oldPassword=OLD_PASSWORD --newPassword=NEW_PASSWORD
#
# If called as a CGI, it expects there to be parameters named oldPassword and
# newPassword whose values are the old and new passwords.
#
# Regardless of how the script is called, it returns the result as JSON.
################################################################################

require "/opt/scripts/commandLineUtils.pl";
require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";
require "/opt/scripts/passwordUtils.pl";

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
my $username = 'root';
if (exists($arguments{'oldPassword'}) && exists($arguments{'newPassword'}))
   {
   my $oldPasswordPlainText = $arguments{'oldPassword'};
   my $newPasswordPlainText = $arguments{'newPassword'};

   # first check that the old password is correct
   my $isOldPasswordVerified = &verifyPassword($username, $oldPasswordPlainText);
   if ($isOldPasswordVerified)
      {
      &setUserPassword($username, $newPasswordPlainText);

      $response{'ok'} = 'true';
      $response{'message'} = 'password changed';
      }
   else
      {
      $response{'ok'} = 'false';
      $response{'message'} = 'old password is incorrect';
      }
   $response{'statusCode'} = '200';
   $response{'statusName'} = 'OK';
   }
else
   {
   $response{'statusCode'} = '400';
   $response{'statusName'} = 'Bad Request';
   $response{'ok'} = 'false';
   $response{'message'} = 'missing parameter(s): oldPassword and newPassword';
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
