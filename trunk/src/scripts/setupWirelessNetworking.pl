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
   print("\n" .
         "---------\n" .
         "MAIN MENU\n" .
         "---------\n" .
         "1) Configure for WEP network\n" .
         "2) Configure for WPA/WPA2 network\n" .
         "3) Quit\n" .
         "Enter your choice: ");

   my $choice = <STDIN>;
   chomp ($choice);

   if ($choice == "1")
      {
      print("\n" .
            "-----------------\n" .
            "WEP CONFIGURATION\n" .
            "-----------------\n");

      $choice = <STDIN>;
      chomp ($choice);
      }
   elsif ($choice == "2")
      {
      print "WPA!\n";
      }
   elsif ($choice == "3")
      {
      $willLoop = 0;
      }
   else
      {
      print "Invalid option [$choice]\n";
      }
   }

print "\nGoodbye!\n\n"
#===================================================================================================