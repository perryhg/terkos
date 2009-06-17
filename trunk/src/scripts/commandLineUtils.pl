#!/usr/bin/perl

#===================================================================================================
sub parseCommandLineSwitches()
   {
   my %switches = ();

   foreach $arg (@ARGV)
      {
      if ($arg =~ /^--(.+)$/)
         {
         my $command = $1;
         if ($command =~ /^(.+)=(.*)$/)
            {
            $switches{$1} = $2;
            }
         else
            {
            $switches{$command} = '';
            }
         }
      }

   return %switches;
   }
#===================================================================================================
return 1;
