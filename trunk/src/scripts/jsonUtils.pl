#!/usr/bin/perl

require "httpUtils.pl";

#===================================================================================================
sub outputJson($json)
   {
   # See whether this script is being called as a CGI.  If so, look for the JSON "callback" query
   # string parameter and then output appropriate headers before printing out the JSON.
   my $callbackFunctionName = '';
   if (exists($ENV{SERVER_SOFTWARE}) && exists($ENV{REQUEST_METHOD}))
      {
      # parse the query string (if any) and look for the "callback" parameter for the JSON
      if (exists($ENV{QUERY_STRING}) && ((uc($ENV{REQUEST_METHOD}) eq "GET") || (uc($ENV{REQUEST_METHOD}) eq "POST")))
         {
         %queryParams = parseHttpRequestParameters();

         if (exists($queryParams{callback}))
            {
            $callbackFunctionName = $queryParams{callback};
            }
         }

      # print headers
      print "Content-Type: application/json\n";
      print "Expires: Wed, 25 Apr 2007 14:00:00 GMT\n";
      print "Cache-Control: no-cache\n\n";
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
return 1;