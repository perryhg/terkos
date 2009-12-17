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

require "/opt/scripts/pathUtils.pl";

%CGI = ();

#===================================================================================================
sub isHttpRequest()
   {
   return (exists($ENV{SERVER_SOFTWARE}) && exists($ENV{REQUEST_METHOD}));
   }
#===================================================================================================
sub printContentDispositionHeader()
   {
   my ($filename) = @_;

   print "Content-Disposition: attachment; filename=$filename\n";
   }
#===================================================================================================
sub restartWebServer()
   {
   my $pathToBusyboxHttpd = &getPath('busybox-httpd');
   my $stopCommand = "$pathToBusyboxHttpd stop";
   my $startCommand = "$pathToBusyboxHttpd start";

   # We do a stop and a start here instead of a restart since the restart fails if the server isn't
   # already running.  Doing a stop first is safe even if the server is already stopped, so we can
   # simply follow it up with a start.

   # issue the stop command
   open(STOP_WEB_SERVER_OUTPUT, "$stopCommand |") or die "Failed to call $stopCommand: $!\n";

   # Read the output into an array -- we won't actually do anything with the output but I
   # found that if I don't read it then the command doesn't actually execute.  That seems
   # weird, but, oh well.
   my @stopWebServerOuput = <STOP_WEB_SERVER_OUTPUT>;

   # close the filehandle
   close(STOP_WEB_SERVER_OUTPUT);

   # issue the start command
   open(START_WEB_SERVER_OUTPUT, "$startCommand |") or die "Failed to call $startCommand: $!\n";

   # Read the output into an array
   my @startWebServerOuput = <START_WEB_SERVER_OUTPUT>;

   # close the filehandle
   close(START_WEB_SERVER_OUTPUT);

   if (scalar(@startWebServerOuput) >= 1)
      {
      my $line = $startWebServerOuput[0];
      if ($line =~ /^starting Busybox HTTP Daemon: httpd...\s*done/)
         {
         return 1;
         }
      elsif ($line =~ /httpd is already running/)
         {
         return 1;
         }
      }
   return 0;
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
