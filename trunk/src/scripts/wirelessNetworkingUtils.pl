#!/usr/bin/perl

require "jsonUtils.pl";

#===================================================================================================
sub printWirelessNetworkingStatusAsJSON()
   {
   my ($interfaceName) = @_;

   # call ifconfig to get details about the wireless interface
   open(IFCONFIG_STATUS, "/sbin/ifconfig $interfaceName |") or die "Failed to call ifconfig $!\n";
   my @ifconfigOutput = <IFCONFIG_STATUS>;
   close(IFCONFIG_STATUS);

   my %wirelessInterface = {};
   my $isWirelessInstalled = 1;

   # Run through the output from ifconfig and look details about the wireless interface.  Start by
   # looking at the first line to 1) make sure that the device is working; and 2) get the wireless
   # interface's MAC address
   if (scalar(@ifconfigOutput) > 0)
      {
      my $line = $ifconfigOutput[0];
      chomp $line;                  # chop off the newline
      $line =~ s/^\s+//;            # chop off leading whitespace
      $line =~ s/\s+$//;            # chop off trailing whitespace
      if ($line =~ /^$interfaceName.+HWaddr\s+(([A-F0-9]{2}:){5}[A-F0-9]{2})$/)
         {
         $wirelessInterface{macAddress} = $1;

         # Now look for lines we care about:
         # 1) get the ip address from the "inet addr/Bcast/Mask"
         # 2) get whether the interface is up/down from the MTU/Metric line
         my $lineNumber = 1;
         while ($lineNumber < $#ifconfigOutput)
            {
            $line = $ifconfigOutput[$lineNumber];
            chomp $line;                  # chop off the newline
            $line =~ s/^\s+//;            # chop off leading whitespace
            $line =~ s/\s+$//;            # chop off trailing whitespace
            if ($line =~ /^inet addr:([0-9\.]+)\s+Bcast:.+Mask:.+$/)
               {
               $wirelessInterface{accessPoint}{ipAddress} = $1;
               }
            elsif ($line =~ /^.+MTU:.+Metric:.+$/)
               {
               $wirelessInterface{isEnabled} = ($line =~ /^UP BROADCAST RUNNING MULTICAST/);
               }
            $lineNumber++;
            }
         }

      # call iwconfig to get associated access point details
      open(IWCONFIG_STATUS, "/sbin/iwconfig $interfaceName |") or die "Failed to call iwconfig $!\n";
      my @iwconfigOutput = <IWCONFIG_STATUS>;
      close(IWCONFIG_STATUS);

      # Run through the output from iwconfig and look for the Mode/Frequency/Access Point line.  Start by
      # looking at the first line to 1) make sure that the device is working; and 2) get the SSID
      if (scalar(@iwconfigOutput) > 0)
         {
         my $line = $iwconfigOutput[0];
         chomp $line;                  # chop off the newline
         $line =~ s/^\s+//;            # chop off leading whitespace
         $line =~ s/\s+$//;            # chop off trailing whitespace
         if ($line =~ /^$interfaceName.+ESSID:"(.+)"\s*$/)
            {
            $wirelessInterface{accessPoint}{ssid} = $1;

            # Now look for the Mode/Frequency/Access Point line so we can determine whether it's associated
            # an access point and, if so, what the access point's MAC address is
            my $lineNumber = 1;
            while ($lineNumber < $#iwconfigOutput)
               {
               $line = $iwconfigOutput[$lineNumber];
               chomp $line;                  # chop off the newline
               $line =~ s/^\s+//;            # chop off leading whitespace
               $line =~ s/\s+$//;            # chop off trailing whitespace
               if ($line =~ /^Mode:.+Frequency:.+Access Point:\s+(.+)$/)
                  {
                  my $accessPoint = $1;
                  $wirelessInterface{accessPoint}{isAssociated} = ($accessPoint =~ /(([A-F0-9]{2}:){5}[A-F0-9]{2})/);
                  if ($wirelessInterface{accessPoint}{isAssociated})
                     {
                     $wirelessInterface{accessPoint}{macAddress} = $1;
                     }

                  break;
                  }
               $lineNumber++;
               }
            }
         }
      }
   else
      {
      $isWirelessInstalled = 0;
      }

   # create the JSON representation
   my $json = '';
   $json .= "{\n";
   $json .= '"wireless-networking-status" : ' . "\n";
   $json .= "   {\n";
   $json .= '   "is-installed" : ' . ($isWirelessInstalled ? 'true' : 'false') . ",\n";
   $json .= '   "wireless-interface" : ' . "\n";
   $json .= "      {\n";
   if ($isWirelessInstalled)
      {
      $json .= '      "is-enabled" : '   . ($wirelessInterface{isEnabled} ? 'true' : 'false') . ",\n";
      $json .= '      "mac-address" : "' . $wirelessInterface{macAddress} . "\",\n";
      $json .= '      "access-point" : ' . "\n";
      $json .= "         {\n";
      if ($wirelessInterface{isEnabled})
         {
         $json .= '         "ssid" : "'        . $wirelessInterface{accessPoint}{ssid} . "\",\n";
         $json .= '         "mac-address" : "' . $wirelessInterface{accessPoint}{macAddress} . "\",\n";
         $json .= '         "ip-address" : "'  . $wirelessInterface{accessPoint}{ipAddress} . "\"\n";
         }
      $json .= "         }\n";
      }
   $json .= "      }\n";
   $json .= "   }\n";
   $json .= "}\n";

   # write out the JSON
   outputJson($json);
   }
#===================================================================================================
return 1;
