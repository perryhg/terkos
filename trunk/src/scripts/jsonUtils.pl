#!/usr/bin/perl

require "httpUtils.pl";
require "fileUtils.pl";

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

   # iterate over the wireless networks and print them out as JSON
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

   if (isHttpRequest())
      {
      printJsonHttpResponseHeaders();
      }
   readFileAndPrintToStdOut($jsonFilename);
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
