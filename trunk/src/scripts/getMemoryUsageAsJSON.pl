#!/usr/bin/perl

require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# Call free to get the memory usage
open(MEMORY_USAGE,"/usr/bin/free |") or die "Failed to call /usr/bin/free $!\n";

# read the output into an array
my @lines = <MEMORY_USAGE>;

# close the filehandle
close(MEMORY_USAGE);

# create the JSON representation
my $json = '';
$json .= "{\n";
$json .= '   "memory-usage" : ' . "\n";
$json .= "   {\n";
if ($#lines == 3)
   {
   my %memoryUsage = ();

   my $lineNumber = 0;
   while ($lineNumber++ < $#lines)
      {
      my $line = $lines[$lineNumber];
      chomp $line;                  # chop off the newline
      $line =~ s/^\s+//;            # chop off leading whitespace
      $line =~ s/\s+$//;            # chop off trailing whitespace
      next unless length($line);    # anything left?

      # test the line with regex to see if it's something we care about
      if ($line =~ /^\s*Mem:\s+(\d+)\s+(\d+)\s+(\d+)/)
         {
         $memoryUsage{'memory'}{'total'} = $1;
         $memoryUsage{'memory'}{'used'} = $2;
         $memoryUsage{'memory'}{'free'} = $3;
         }
      elsif ($line =~ /^\s*Swap:\s+(\d+)\s+(\d+)\s+(\d+)/)
         {
         $memoryUsage{'swap'}{'total'} = $1;
         $memoryUsage{'swap'}{'used'} = $2;
         $memoryUsage{'swap'}{'free'} = $3;
         }
      elsif ($line =~ /^\s*Total:\s+(\d+)\s+(\d+)\s+(\d+)/)
         {
         $memoryUsage{'total'}{'total'} = $1;
         $memoryUsage{'total'}{'used'} = $2;
         $memoryUsage{'total'}{'free'} = $3;
         }
      }

   $json .= '      "memory" :' . "\n";
   $json .= "      {\n";
   $json .= '         "total" : ' . $memoryUsage{'memory'}{'total'} . ",\n";
   $json .= '         "used" : '  . $memoryUsage{'memory'}{'used'}  . ",\n";
   $json .= '         "free" : '  . $memoryUsage{'memory'}{'free'}  . "\n";
   $json .= "      },\n";
   $json .= '      "swap" :' . "\n";
   $json .= "      {\n";
   $json .= '         "total" : ' . $memoryUsage{'swap'}{'total'} . ",\n";
   $json .= '         "used" : '  . $memoryUsage{'swap'}{'used'}  . ",\n";
   $json .= '         "free" : '  . $memoryUsage{'swap'}{'free'}  . "\n";
   $json .= "      },\n";
   $json .= '      "total" :' . "\n";
   $json .= "      {\n";
   $json .= '         "total" : ' . $memoryUsage{'total'}{'total'} . ",\n";
   $json .= '         "used" : '  . $memoryUsage{'total'}{'used'}  . ",\n";
   $json .= '         "free" : '  . $memoryUsage{'total'}{'free'}  . "\n";
   $json .= "      }\n";
   }
$json .= "   }\n";
$json .= "}\n";

# write out the JSON
outputJson($json);
