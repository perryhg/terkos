#!/usr/bin/perl

#===================================================================================================
sub readFileIntoArray()
   {
   my ($filename) = @_;

   open FILE, $filename or die "Couldn't open file: $!";
   my @lines = <FILE>;
   close FILE;

   return @lines;
   }
#===================================================================================================
sub readFileIntoString()
   {
   my ($filename) = @_;

   open FILE, $filename or die "Couldn't open file: $!";
   my $str = join("", <FILE>);
   close FILE;

   return $str;
   }
#===================================================================================================
sub writeStringToFile()
   {
   my ($str, $filename) = @_;

   open FILE, ">$filename" or die "Couldn't open file: $!";
   print FILE $str;
   close FILE;
   }
#===================================================================================================
return 1;
