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

use strict;

require "/opt/scripts/fileUtils.pl";
require "/opt/scripts/pathUtils.pl";

#=======================================================================================================================
sub getNumericInput()
   {
   my $choice = <STDIN>;
   chomp ($choice);

   if ($choice =~ /^[0-9]+$/)
      {
      return $choice;
      }
   else
      {
      print "Invalid option [$choice]\n";
      }
   return 0;
   }
#=======================================================================================================================
sub printMainMenu()
   {
   print("\n" .
         "---------\n" .
         "MAIN MENU\n" .
         "---------\n" .
         "1) Configure for unencrypted network\n" .
         "2) Configure for WEP network\n" .
         "3) Configure for WPA/WPA2 network\n" .
         "4) Quit\n" .
         "Enter your choice: ");
   }
#=======================================================================================================================
sub saveConfiguration()
   {
   my ($profileJSON) = @_;

   my $json = '';
   $json .= "{\n";
   $json .= '"wireless-networking" : ' . "\n";
   $json .= "   {\n";
   $json .= '   "profiles" : [' . "\n";
   $json .= $profileJSON;
   $json .= "   ],\n";
   $json .= '   "will-start-on-bootup" : true' . "\n";
   $json .= "   }\n";
   $json .= "}\n";

   #print $json;

   my $pathToJsonConfigFile = &getPath('wireless_networking_config.json');
   my $wirelessNetworkingConfigTool = &getPath('WirelessNetworkingConfigTool');

   my $jsonBeforeUpdate = &readFileIntoString($pathToJsonConfigFile);

   # call the program and pipe the JSON input to it via its STDIN
   open(PROGRAM, "| $wirelessNetworkingConfigTool") or die "cannot open $wirelessNetworkingConfigTool: $!";
   print PROGRAM "$json\n";
   close(PROGRAM);

   my $jsonAfterUpdate = &readFileIntoString($pathToJsonConfigFile);

   if ($jsonBeforeUpdate eq $jsonAfterUpdate)
      {
      print "\n\nERROR: Wireless configuation not saved.  Either an error occurred or no save was necessary.\n\n";
      }
   else
      {
      print "\n\Wireless configuration saved.\n\n";
      }
   }
#=======================================================================================================================
sub handleUnencryptedConfiguration()
   {
   my $willLoop = 1;
   while($willLoop)
      {
      print("\n" .
            "---------------------------------\n" .
            "UNENCRYPTED NETWORK CONFIGURATION\n" .
            "---------------------------------\n" .
            "1) Enter network name (SSID)\n" .
            "2) Cancel, return to main menu\n" .
            "Enter your choice: ");

      my $choice = &getNumericInput();

      if ($choice == 1)
         {
         print "\nNetwork name (SSID): ";
         my $ssid = <STDIN>;
         chomp ($ssid);

         print("\nSave this configuration?  This will overwrite your current wireless configuration.\n" .
               "   Encryption: None\n" .
               "   SSID:       $ssid\n" .
               "Enter your choice (y/N): ");

         my $willSave = <STDIN>;
         chomp ($willSave);
         if ($willSave =~ /^y$/)
            {
            my $profileJSON = '{"encryption-type" : "none", "ssid" : "' . $ssid . '"}';
            &saveConfiguration($profileJSON);
            $willLoop = 0;
            }
         else
            {
            print "\nCancelled, no configuration changes were made.\n";
            }
         }
      else
         {
         $willLoop = 0;
         }
      }
   }
#=======================================================================================================================
sub handleWEPConfiguration()
   {
   my $willLoop = 1;
   while($willLoop)
      {
      print("\n" .
            "-------------------------\n" .
            "WEP NETWORK CONFIGURATION\n" .
            "-------------------------\n" .
            "1) 40/64-bit encryption (10 hex digits)\n" .
            "2) 128-bit encryption (26 hex digits)\n" .
            "3) Cancel, return to main menu\n" .
            "Enter your choice: ");

      my $choice = &getNumericInput();

      if ($choice == 1 || $choice == 2)
         {
         print "\nNetwork name (SSID): ";
         my $ssid = <STDIN>;
         chomp ($ssid);

         my $password = "";
         my $isValidPassword = 0;
         while (!$isValidPassword)
            {
            print "Password " . (($choice == 1) ? "(10 hex digits)" : "(26 hex digits)") . ": ";

            $password = <STDIN>;
            chomp ($password);

            $isValidPassword = ($choice == 1 && ($password =~ /^[a-fA-F0-9]{10}$/)) ||  ($choice == 2 && ($password =~ /^[a-fA-F0-9]{26}$/));

            if (!$isValidPassword)
               {
               print "\nInvalid password!  Please try again...\n";
               }
            }

         print("\nSave this configuration?  This will overwrite your current wireless configuration.\n" .
               "   Encryption: WEP, " . (($choice == 1) ? "40/64-bit" : "128-bit") . "\n" .
               "   SSID:       $ssid\n" .
               "   Password:   $password\n" .
               "Enter your choice (y/N): ");

         my $willSave = <STDIN>;
         chomp ($willSave);
         if ($willSave =~ /^y$/)
            {
            my $profileJSON = '{"encryption-type" : "wep", "ssid" : "' . $ssid . '", "is-hex-password" : true, "password" : "' . $password . '"}';
            &saveConfiguration($profileJSON);
            $willLoop = 0;
            }
         else
            {
            print "\nCancelled, no configuration changes were made.\n";
            }
         }
      elsif ($choice == "3")
         {
         $willLoop = 0;
         }
      }
   }
#=======================================================================================================================
sub handleWPAConfiguration()
   {
   my $willLoop = 1;
   while($willLoop)
      {
      print("\n" .
            "-------------------------\n" .
            "WPA NETWORK CONFIGURATION\n" .
            "-------------------------\n" .
            "1) Plain-text password (8-63 characters)\n" .
            "2) Hex password (64 hex digits)\n" .
            "3) Cancel, return to main menu\n" .
            "Enter your choice: ");

      my $choice = &getNumericInput();

      if ($choice == 1 || $choice == 2)
         {
         print "\nNetwork name (SSID): ";
         my $ssid = <STDIN>;
         chomp ($ssid);

         my $password = "";
         my $isValidPassword = 0;
         while (!$isValidPassword)
            {
            print "Password " . (($choice == 1) ? "(8-63 characters)" : "(64 hex digits)") . ": ";

            $password = <STDIN>;
            chomp ($password);

            $isValidPassword = ($choice == 1 && ($password =~ /^.{8,63}$/)) ||  ($choice == 2 && ($password =~ /^[a-fA-F0-9]{64}$/));

            if (!$isValidPassword)
               {
               print "\nInvalid password!  Please try again...\n";
               }
            }

         print("\nSave this configuration?  This will overwrite your current wireless configuration.\n" .
               "   Encryption: WPA, " . (($choice == 1) ? "plain-text password" : "hex password") . "\n" .
               "   SSID:       $ssid\n" .
               "   Password:   $password\n" .
               "Enter your choice (y/N): ");

         my $willSave = <STDIN>;
         chomp ($willSave);
         if ($willSave =~ /^y$/)
            {
            my $profileJSON = '{"encryption-type" : "wpa", "ssid" : "' . $ssid . '", "is-hex-password" : ' . ($choice == 1 ? "false" : "true") . ', "password" : "' . $password . '"}';
            &saveConfiguration($profileJSON);
            $willLoop = 0;
            }
         else
            {
            print "\nCancelled, no configuration changes were made.\n";
            }
         }
      elsif ($choice == "3")
         {
         $willLoop = 0;
         }
      }
   }
#=======================================================================================================================
print "\n";
print "==============================================\n";
print "   VEXPro Wireless Networking Setup Utility\n";
print "==============================================\n";
print "\n";
print "This program helps configure your VEXPro for\n";
print "encrypted wireless networking.  It currently\n";
print "only supports WEP, WPA, and WPA2.\n";
print "\n";
print "WARNING: Using this program will overwrite any\n";
print "existing networking configuration defined on\n";
print "this VEXPro.\n";
print "\n";
print "==============================================\n";

my $willLoop = 1;
while($willLoop)
   {
   &printMainMenu();

   my $choice = &getNumericInput();

   if ($choice == "1")
      {
      &handleUnencryptedConfiguration();
      }
   elsif ($choice == "2")
      {
      &handleWEPConfiguration();
      }
   elsif ($choice == "3")
      {
      &handleWPAConfiguration();
      }
   elsif ($choice == "4")
      {
      $willLoop = 0;
      }
   }

print "\nGoodbye!\n\n"
#=======================================================================================================================
