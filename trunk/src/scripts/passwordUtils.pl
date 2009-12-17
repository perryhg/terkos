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

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/fileUtils.pl";

#===================================================================================================
sub verifyPassword()
   {
   my ($username, $passwordPlainText) = @_;

   my @userInfo = getpwnam($username);
   my $oldPassword = $userInfo[1];
   my $oldPasswordVerified = crypt($passwordPlainText, substr($oldPassword,0,2));

   return ($oldPassword eq $oldPasswordVerified);
   }
#===================================================================================================
sub encryptPassword()
   {
   my ($passwordPlainText) = @_;

   # generate a random, 2 character salt
   my @chars = ('a'..'z');
   my $salt = $chars[int rand @chars] . $chars[int rand @chars];

   # encrypt the new password
   return crypt($passwordPlainText, $salt);
   }
#===================================================================================================
sub setUserPassword()
   {
   my ($username, $newPasswordPlainText) = @_;

   my $newPassword = &encryptPassword($newPasswordPlainText);

   my $passwdFilePath = &getPath('etc_passwd');
   my @lines = &readFileIntoArray($passwdFilePath);

   my @newLines = ();
   for my $line (@lines)
      {
      chomp $line;                  # chop off the newline
      next unless length($line);    # anything left?

      # test the line with regex to see if it's something we care about
      if ($line =~ /^$username:(.+)$/)
         {
         my @parts = split(/:/, $1);

         # set the new password
         $parts[0] = $newPassword;

         # re-build the line
         $line = $username . ":" . join(':',@parts);
         }
         
      push @newLines, $line;
      }
   my $newPasswdFile = join("\n", @newLines) . "\n";
   &writeStringToFile($newPasswdFile, $passwdFilePath);
   }
#===================================================================================================
return 1;
