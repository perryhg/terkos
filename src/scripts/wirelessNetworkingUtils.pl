#!/usr/bin/perl -w

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

require "/opt/scripts/jsonUtils.pl";
require "/opt/scripts/pathUtils.pl";

#===================================================================================================
sub enableWirelessNetworking()
   {
   my ($interfaceName) = @_;

   # make sure the wireless interface is down
   &disableWirelessNetworking($interfaceName);

   # Call ifup to bring up the wireless interface.  Note: I'm using backticks here because
   # using open() didn't work because wpa_supplicant would fail with this error message:
   # "run-parts: /etc/network/if-post-down.d/wpasupplicant exited with code 1013"
   return `/sbin/ifup $interfaceName`;
   }
#===================================================================================================
sub disableWirelessNetworking()
   {
   my ($interfaceName) = @_;

   # Call ifdown to shut down the wireless interface.  Note: I'm using backticks here because
   # using open() didn't work because wpa_supplicant would fail with this error message:
   # "run-parts: /etc/network/if-post-down.d/wpasupplicant exited with code 1013"
   return `/sbin/ifdown $interfaceName`;
   }
#===================================================================================================
sub enableAdHocWirelessNetworking()
   {
   my ($interfaceName) = @_;

   # make sure the wireless interface is down
   &disableAdHocWirelessNetworking($interfaceName);

   my $wpaSupplicantAdHocConfFile = &getPath('etc_network_wpa_supplicant_ad_hoc_conf');

   return `/usr/sbin/wpa_supplicant -c$wpaSupplicantAdHocConfFile -i$interfaceName -Dwext -B && /sbin/ifconfig $interfaceName 169.254.54.42 netmask 255.255.255.0`;
   }
#===================================================================================================
sub disableAdHocWirelessNetworking()
   {
   my ($interfaceName) = @_;

   `/sbin/ifdown $interfaceName`;
   `/sbin/ifconfig $interfaceName down`;
   `/usr/bin/killall wpa_supplicant`;
   }
#===================================================================================================
sub printWirelessNetworkingStatusAsJSON()
   {
   my ($interfaceName) = @_;

   # call ifconfig to get details about the wireless interface
   open(IFCONFIG_STATUS, "/sbin/ifconfig $interfaceName |") or die "Failed to call ifconfig $!\n";
   my @ifconfigOutput = <IFCONFIG_STATUS>;
   close(IFCONFIG_STATUS);

   my %wirelessInterface = ();
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

                  last;
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
sub getNetworkInterfaceMACAddress()
   {
   my ($interfaceName) = @_;

   # call ifconfig to get details about the interface
   open(IFCONFIG_STATUS, "/sbin/ifconfig $interfaceName |") or die "Failed to call ifconfig $!\n";
   my @ifconfigOutput = <IFCONFIG_STATUS>;
   close(IFCONFIG_STATUS);

   # Parse the output from ifconfig and pick out the MAC address.
   if (scalar(@ifconfigOutput) > 0)
      {
      my $line = $ifconfigOutput[0];
      chomp $line;                  # chop off the newline
      $line =~ s/^\s+//;            # chop off leading whitespace
      $line =~ s/\s+$//;            # chop off trailing whitespace
      if ($line =~ /^$interfaceName.+HWaddr\s+(([A-F0-9]{2}:){5}[A-F0-9]{2})$/)
         {
         return $1;
         }
      }
   return '';
   }
#===================================================================================================
sub generateWPASupplicantConfForAdHoc()
   {
   # get the path to the conf file
   my $confFilePath = &getPath('etc_network_wpa_supplicant_ad_hoc_conf');

   # don't overwrite it if it already exists
   unless (-e $confFilePath)
      {
      # The SSID will use the last 4 digits of the eth0 MAC address (we use eth0 instead of wlan0 because
      # we want to SSID to stay with the device, not the wifi adapter).
      my $eth0MacAddress = &getNetworkInterfaceMACAddress("eth0");
      $eth0MacAddress =~ s/://g;
      my $ssid = "VEXPro" . substr($eth0MacAddress, 8, 4);

      # open the conf file
      open (CONF_FILE, ">$confFilePath") or die "Failed to open $confFilePath for writing $!\n";

      # write the conf file
      print CONF_FILE "ap_scan=2\n" .
                      "network={\n" .
                      "        ssid=\"" .$ssid. "\"\n" .
                      "        mode=1\n" .
                      "        frequency=2412\n" .
                      "        key_mgmt=NONE\n" .
                      "        pairwise=NONE\n" .
                      "}\n";

      # close the conf file
      close(CONF_FILE);
      }
   }
#===================================================================================================
return 1;
