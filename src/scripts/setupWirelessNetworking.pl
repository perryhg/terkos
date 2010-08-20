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
         "1) Configure for WEP network\n" .
         "2) Configure for WPA/WPA2 network\n" .
         "3) Quit\n" .
         "Enter your choice: ");
   }
#=======================================================================================================================
sub handleWEPConfiguration()
   {
   my $willLoop = 1;
   while($willLoop)
      {
      print("\n" .
            "-----------------\n" .
            "WEP CONFIGURATION\n" .
            "-----------------\n" .
            "1) 40/64-bit encryption\n" .
            "2) 128-bit encryption\n" .
            "3) Cancel, return to main menu\n" .
            "Enter your choice: ");

      my $choice = &getNumericInput();

      if ($choice == 1 || $choice == 2)
         {
         print "\nNetwork name (SSID): ";
         my $ssid = <STDIN>;
         chomp ($ssid);

         my $password = "";
         my $isValidPasswordLength = 0;
         while (!$isValidPasswordLength)
            {
            print "Password " . (($choice == 1) ? "(10 hex digits)" : "(26 hex digits)") . ": ";

            $password = <STDIN>;
            chomp ($password);

            $isValidPasswordLength = ($choice == 1 && ($password =~ /^[a-fA-F0-9]{10}$/)) ||  ($choice == 2 && ($password =~ /^[a-fA-F0-9]{26}$/));

            if (!$isValidPasswordLength)
               {
               print "\nInvalid password!  Please try again...\n";
               }
            }

         print("\nSave this configuration?\n" .
               "   SSID:     $ssid\n" .
               "   Password: $password\n" .
               "Enter your choice (y/N): ");

         my $willSave = <STDIN>;
         chomp ($willSave);
         if ($willSave =~ /^y$/)
            {
            print "\nSAVED!\n";
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
   print("\n" .
         "-----------------\n" .
         "WPA CONFIGURATION\n" .
         "-----------------\n");
   my $choice = &getNumericInput();
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
      &handleWEPConfiguration();
      }
   elsif ($choice == "2")
      {
      &handleWPAConfiguration();
      }
   elsif ($choice == "3")
      {
      $willLoop = 0;
      }
   }

print "\nGoodbye!\n\n"
#=======================================================================================================================
