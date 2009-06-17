#!/usr/bin/perl

require "commandLineUtils.pl";
require "httpUtils.pl";
require "jsonUtils.pl";

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

# convert "true" and "false" values to booleans
while (($key, $val) = each(%arguments))
   {
   if (lc($val) eq 'true')
      {
      $arguments{$key} = 1;
      }
   elsif (lc($val) eq 'false')
      {
      $arguments{$key} = 0;
      }
   }

# set flags, defaulting to true if the arguments weren't specified
my $includeEncryptedNetworks = (exists($arguments{'include-encrypted'})) ? $arguments{'include-encrypted'} : 1;
my $includeUnencryptedNetworks = (exists($arguments{'include-unencrypted'})) ? $arguments{'include-unencrypted'} : 1;

my $interfaceName = "wlan0";

# call iwlist
open(ACCESS_POINTS, "/sbin/iwlist $interfaceName scan |") or die "Failed to call iwlist $!\n";

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
   if ($line =~ /^Cell\s+\d+\s+-\s+Address:\s(([A-F0-9]{2}:){5}[A-F0-9]{2})$/)
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
      my $isEncrypted = ("on" eq $1);
      $accessPoints[$i]->{isEncrypted} = ($isEncrypted) ? 1 : 0;
      $accessPoints[$i]->{isEncryptedStr} = ($isEncrypted) ? "true" : "false";
      }

   $lineNumber++;
   }

# Now that we have an array of access points, we want to group them by SSID
my %wirelessNetworks = ();
for $j (0 .. $#accessPoints)
   {
   my $isEncrypted = $accessPoints[$j]{isEncrypted};
   if (($isEncrypted && $includeEncryptedNetworks) || (($isEncrypted == 0) && $includeUnencryptedNetworks))
      {
      my $ssid = $accessPoints[$j]{ssid};
      my $mac = $accessPoints[$j]{mac};
      $wirelessNetworks{$ssid}{isEncrypted} = $accessPoints[$j]{isEncryptedStr};
      $wirelessNetworks{$ssid}{access-points}{$mac}{channel} = $accessPoints[$j]{channel};
      $wirelessNetworks{$ssid}{access-points}{$mac}{frequency} = $accessPoints[$j]{frequency};
      $wirelessNetworks{$ssid}{access-points}{$mac}{quality} = $accessPoints[$j]{quality};
      $wirelessNetworks{$ssid}{access-points}{$mac}{signalLevel} = $accessPoints[$j]{signalLevel};
      }
   }

# iterate over the wireless networks and create the JSON representation
my $json = '';
$json .= '{"wireless-networks" : [' . "\n";
my @ssids = sort keys %wirelessNetworks;
for $networkCount (0 .. $#ssids)
   {
   my $ssid = $ssids[$networkCount];
   $json .= "   {\n";
   $json .= '      "ssid" : "'        . $ssid        . '"' . ",\n";
   $json .= '      "is-encrypted" : '  . $wirelessNetworks{$ssid}{isEncrypted} . ",\n";
   $json .= '      "access-points" : ['  .  "\n";
   my @macs = sort keys %{$wirelessNetworks{$ssid}{access-points}};
   for $macCount (0 .. $#macs)
      {
      $json .= "         {\n";
      $mac = $macs[$macCount];
      $json .= '            "mac-address" : "'. $mac                                                       . '"' . ",\n";
      $json .= '            "channel" : '     . $wirelessNetworks{$ssid}{access-points}{$mac}{channel}     . '' . ",\n";
      $json .= '            "frequency" : "'  . $wirelessNetworks{$ssid}{access-points}{$mac}{frequency}   . '"' . ",\n";
      $json .= '            "quality" : '     . $wirelessNetworks{$ssid}{access-points}{$mac}{quality}     . '' . ",\n";
      $json .= '            "signalLevel" : ' . $wirelessNetworks{$ssid}{access-points}{$mac}{signalLevel} . '' . "\n";
      $json .= "         }";
      if ($macCount < $#macs)
         {
         $json .= ",";
         }
      $json .= "\n";
      }
   $json .= "      ]\n";
   $json .= "   }";
   if ($networkCount < $#ssids)
      {
      $json .= ",";
      }
   $json .= "\n";
   }
$json .= "]}";
if ($callbackFunctionName)
   {
   $json .= ')';
   }
$json .= "\n";

# write out the JSON
outputJson($json);
