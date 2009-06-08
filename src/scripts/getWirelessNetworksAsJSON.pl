#!/usr/bin/perl

# call iwlist
open(ACCESS_POINTS, "/sbin/iwlist wlan0 scan |") or die "Failed to call iwlist $!\n";

# read the output into an array
my @lines = <ACCESS_POINTS>;

# close the filehandle
close(ACCESS_POINTS);

# run through the output from iwlist and store it in an array of hashes
my @accessPoints = ();
my $lineNumber = 0;
my $i = -1;
while ($lineNumber < $#lines)
   {
   my $line = $lines[$lineNumber];
   chomp $line;                  # chop off the newline
   $line =~ s/^\s+//;            # chop off leading whitespace
   $line =~ s/\s+$//;            # chop off trailing whitespace

   next unless length($line);    # anything left?

   # test the line with regex to see if it's something we care about
   if ($line =~ /^Cell\s+\d+\s+-\s+Address:\s([A-F0-9:]+)$/)
      {
      # We found a new Cell line, so increment the index in the accessPoints array
      $i++;

      # record the MAC address
      $accessPoints[$i]->{mac} = $1;
      }
   elsif ($line =~ /^ESSID:"(.+)"/)
      {
      $accessPoints[$i]->{ssid} = $1;
      }
   elsif ($line =~ /^Mode:(.+)/)
      {
      $accessPoints[$i]->{mode} = $1;
      }
   elsif ($line =~ /^Channel:(.+)/)
      {
      $accessPoints[$i]->{channel} = $1;
      }
   elsif ($line =~ /^Frequency:(.+)\s+\(Channel \d+\)/)
      {
      $accessPoints[$i]->{frequency} = $1;
      }
   elsif ($line =~ /^Quality=(\d+)\/100\s+Signal level:(.+)\s+dBm$/)
      {
      $accessPoints[$i]->{quality} = $1;
      $accessPoints[$i]->{signalLevel} = $2;
      }
   elsif ($line =~ /^Encryption key:(.+)/)
      {
      $accessPoints[$i]->{isEncrypted} = ("on" eq $1 ? "true" : "false");
      }

   $lineNumber++;
   }

# Now that we have an array of access points, we want to group them by SSID
my %wirelessNetworks = ();
for $j (0 .. $#accessPoints)
   {
   my $ssid = $accessPoints[$j]{ssid};
   my $mac = $accessPoints[$j]{mac};
   $wirelessNetworks{$ssid}{isEncrypted} = $accessPoints[$j]{isEncrypted};
   $wirelessNetworks{$ssid}{access-points}{$mac}{channel} = $accessPoints[$j]{channel};
   $wirelessNetworks{$ssid}{access-points}{$mac}{frequency} = $accessPoints[$j]{frequency};
   $wirelessNetworks{$ssid}{access-points}{$mac}{quality} = $accessPoints[$j]{quality};
   $wirelessNetworks{$ssid}{access-points}{$mac}{signalLevel} = $accessPoints[$j]{signalLevel};
   }

# See whether this script is being called as a CGI.  If so, look for the JSON "callback" query
# string parameter and then output appropriate headers before printing out the JSON.
my $callbackFunctionName = '';
if (exists($ENV{SERVER_SOFTWARE}) && exists($ENV{REQUEST_METHOD}))
   {
   # parse the query string (if any) and look for the "callback" parameter for the JSON
   if (exists($ENV{QUERY_STRING}) && ((uc($ENV{REQUEST_METHOD}) eq "GET") || (uc($ENV{REQUEST_METHOD}) eq "POST")))
      {
      %queryParams = parseQueryParameters();

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
print '{"wireless-networks" : [' . "\n";
my @ssids = sort keys %wirelessNetworks;
for $networkCount (0 .. $#ssids)
   {
   my $ssid = $ssids[$networkCount];
   print "   {\n";
   print '      "ssid" : "'        . $ssid        . '"' . ",\n";
   print '      "is-encrypted" : '  . $wirelessNetworks{$ssid}{isEncrypted} . ",\n";
   print '      "access-points" : ['  .  "\n";
   my @macs = sort keys %{$wirelessNetworks{$ssid}{access-points}};
   for $macCount (0 .. $#macs)
      {
      print "         {\n";
      $mac = $macs[$macCount];
      print '            "mac" : "'         . $mac                                                       . '"' . ",\n";
      print '            "channel" : '     . $wirelessNetworks{$ssid}{access-points}{$mac}{channel}     . '' . ",\n";
      print '            "frequency" : "'   . $wirelessNetworks{$ssid}{access-points}{$mac}{frequency}   . '"' . ",\n";
      print '            "quality" : '     . $wirelessNetworks{$ssid}{access-points}{$mac}{quality}     . '' . ",\n";
      print '            "signalLevel" : ' . $wirelessNetworks{$ssid}{access-points}{$mac}{signalLevel} . '' . "\n";
      print "         }";
      if ($macCount < $#macs)
         {
         print ",";
         }
      print "\n";
      }
   print "      ]\n";
   print "   }";
   if ($networkCount < $#ssids)
      {
      print ",";
      }
   print "\n";
   }
print "]}";
if ($callbackFunctionName)
   {
   print ')';
   }
print "\n";

#===================================================================================================
# Code taken from Matt's Scripts: http://www.scriptarchive.com/
sub parseQueryParameters()
   {
   my %queryParameters = ();

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
      return %queryParameters;
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

      $queryParameters{$key}= $value;
      }

   return %queryParameters;
   }
#===================================================================================================
