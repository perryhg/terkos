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

require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/fileUtils.pl";

#===================================================================================================
sub outputJson()
   {
   my ($json) = @_;
   # See whether this script is being called as a CGI.  If so, look for the JSON "callback" query
   # string parameter and then output appropriate headers before printing out the JSON.
   my $callbackFunctionName = '';
   if (isHttpRequest())
      {
      # parse the request parameters (if any)
      my %queryParams = parseHttpRequestParameters();

      # look for the "callback" parameter for the JSON
      if (exists($queryParams{callback}))
         {
         $callbackFunctionName = $queryParams{callback};
         }

      # print HTTP response headers
      printJsonHttpResponseHeaders();
      }

   # print them out as JSON
   if ($callbackFunctionName)
      {
      print $callbackFunctionName . '(';
      }
   print $json;
   if ($callbackFunctionName)
      {
      print ')';
      }
   print "\n";
   }
#===================================================================================================
sub outputJsonFile()
   {
   my ($jsonFilename) = @_;

   my $fileContents = &readFileIntoString($jsonFilename);

   &outputJson($fileContents);
   }
#===================================================================================================
sub printJsonHttpResponseHeaders()
   {
   # print HTTP response headers
   print "Content-Type: application/json\n";
   print "Expires: Wed, 25 Apr 2007 14:00:00 GMT\n";
   print "Cache-Control: no-cache\n\n";
   }
#===================================================================================================
return 1;
