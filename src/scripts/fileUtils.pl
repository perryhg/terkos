#!/usr/bin/perl

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
return 1;
