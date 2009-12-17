#!/usr/bin/perl -w

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
# Script for resetting the root user's password.  Can be called either via
# the command line or as a CGI.
#
# If called from the command line, the usage is:
#
#       ./resetRootUserPassword.pl
#
# Regardless of how the script is called, it returns the result as JSON.
################################################################################

use strict;
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
