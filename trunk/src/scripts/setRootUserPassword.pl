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
# Script for setting a the root user's password.  Can be called either via
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
