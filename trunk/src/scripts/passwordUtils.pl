#!/usr/bin/perl -w

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
