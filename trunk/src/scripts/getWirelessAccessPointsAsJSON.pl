#!/usr/bin/perl

# call iwlist
open(ACCESS_POINTS, "/sbin/iwlist wlan0 scan |") or die "Failed to call iwlist $!\n";

# read the output into an array
@lines = <ACCESS_POINTS>;

# close the filehandle
close(ACCESS_POINTS);

# run through the output from iwlist and store it in an array of hashes
@accessPoints = ();
my $lineNumber = 0;
my $i = -1;
while ($lineNumber < $#lines)
	{
	$line = $lines[$lineNumber];
   chomp $line;						# chop off the newline
   $line =~ s/^\s+//;				# chop off leading whitespace
   $line =~ s/\s+$//;				# chop off trailing whitespace

   next unless length($line); 	# anything left?

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

# iterate over the access points and print them out as JSON
print '{"access-points" : [' . "\n";
for $j (0 .. $#accessPoints)
	{
	print "   {\n";
	print '      "ssid" : "'        . $accessPoints[$j]{ssid}        . '"' . ",\n";
	print '      "mac" : "'         . $accessPoints[$j]{mac}         . '"' . ",\n";
	print '      "mode" : "'        . $accessPoints[$j]{mode}        . '"' . ",\n";
	print '      "channel" : '      . $accessPoints[$j]{channel}     . ",\n";
	print '      "frequency" : "'   . $accessPoints[$j]{frequency}   . '"' . ",\n";
	print '      "quality" : '      . $accessPoints[$j]{quality}     . ",\n";
	print '      "signalLevel" : '  . $accessPoints[$j]{signalLevel} . ",\n";
	print '      "isEncrypted" : '  . $accessPoints[$j]{isEncrypted} . "\n";
	print "   }";
	if ($j < $#accessPoints)
		{
		print ",";
		}
	print "\n";
	}
print "]}\n";