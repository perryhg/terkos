#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";

%CGI = ();

#===================================================================================================
sub isHttpRequest()
   {
   return (exists($ENV{SERVER_SOFTWARE}) && exists($ENV{REQUEST_METHOD}));
   }
#===================================================================================================
sub restartWebServer()
   {
   my $pathToBusyboxHttpd = &getPath('busybox-httpd');
   my $restartCommand = "$pathToBusyboxHttpd restart";

   # issue the restart
   open(WEB_SERVER_OUTPUT, "$restartCommand |") or die "Failed to call $restartCommand: $!\n";

   # Read the output into an array -- we won't actually do anything with the output but I
   # found that if I don't read it then the command doesn't actually execute.  That seems
   # weird, but, oh well.  There's also no point in parsing the output because even if it
   # reports some kind of error there's nothing we can do about it since we've broken the
   # connection to the user.
   my @lines = <WEB_SERVER_OUTPUT>;

   # close the filehandle
   close(WEB_SERVER_OUTPUT);
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
