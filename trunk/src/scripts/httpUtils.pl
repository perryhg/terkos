#!/usr/bin/perl

%CGI = ();

#===================================================================================================
sub isHttpRequest()
   {
   return (exists($ENV{SERVER_SOFTWARE}) && exists($ENV{REQUEST_METHOD}));
   }
#===================================================================================================
# Code taken from Matt's Scripts: http://www.scriptarchive.com/
sub parseHttpRequestParameters()
   {
   # Determine the form's REQUEST_METHOD (GET or POST) and split the form
   # fields up into their name-value pairs.  If the REQUEST_METHOD was
   # not GET or POST, simply return and empty hash.
   my @pairs = ();
   if ($ENV{'REQUEST_METHOD'} eq 'GET')
      {
      # Split the name-value pairs
      @pairs = split(/&/, $ENV{'QUERY_STRING'});
      }
   elsif ($ENV{'REQUEST_METHOD'} eq 'POST')
      {
      # Get the input
      read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});

      # Split the name-value pairs
      @pairs = split(/&/, $buffer);
      }
   else
      {
      return %CGI;
      }

   foreach $pair (@pairs)
      {
      # Split the pair up into individual variables.
      local($key, $value) = split(/=/, $pair);

      # Decode the form encoding on the name and value variables.
      $key =~ tr/+/ /;
      $key =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
      $key =~ tr/\0//d;

      $value =~ tr/+/ /;
      $value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
      $value =~ tr/\0//d;

      $CGI{$key}= $value;
      }

   return %CGI;
   }
#===================================================================================================
return 1;
