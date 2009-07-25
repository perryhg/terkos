#!/usr/bin/perl

require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# Call free to get the disk usage
open(DISK_USAGE,"/bin/df |") or die "Failed to call /bin/df $!\n";

# read the output into an array
my @lines = <DISK_USAGE>;

# close the filehandle
close(DISK_USAGE);

# create the JSON representation
my $json = '';
$json .= "{\n";
$json .= '   "disk-usage" : ' . "\n";
$json .= "   {\n";
$json .= '      "units" : "K",' . "\n";
$json .= '      "filesystems" : ' . "\n";
$json .= "      [\n";
if ($#lines >= 1)
   {
   my @filesystems = ();

   my $lineNumber = 0;
   my $i = -1;
   while ($lineNumber++ < $#lines)
      {
      my $line = $lines[$lineNumber];
      chomp $line;                  # chop off the newline
      $line =~ s/^\s+//;            # chop off leading whitespace
      $line =~ s/\s+$//;            # chop off trailing whitespace
      next unless length($line);    # anything left?

      # test the line with regex to see if it's something we care about
      if ($line =~ /^(\S+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\%\s+(\S+)$/)
         {
         $i++;
         $filesystems[$i]->{'filesystem'} = $1;
         $filesystems[$i]->{'total'} = $2;
         $filesystems[$i]->{'used'} = $3;
         $filesystems[$i]->{'available'} = $4;
         $filesystems[$i]->{'use-percentage'} = $5;
         $filesystems[$i]->{'mounted-on'} = $6;
         }
      }

   for $j (0 .. $#filesystems)
      {
      $json .= "         {\n";
      $json .= '            "filesystem" : "'     . $filesystems[$j]->{'filesystem'}     . '",' . "\n";
      $json .= '            "total" : "'          . $filesystems[$j]->{'total'}     . '",' . "\n";
      $json .= '            "used" : "'           . $filesystems[$j]->{'used'}     . '",' . "\n";
      $json .= '            "available" : "'      . $filesystems[$j]->{'available'}      . '",' . "\n";
      $json .= '            "use-percentage" : "' . $filesystems[$j]->{'use-percentage'} . '",' . "\n";
      $json .= '            "mounted-on" : "'     . $filesystems[$j]->{'mounted-on'}     . '"' . "\n";
      $json .= "         }";
      if ($networkCount < $#filesystems)
         {
         $json .= ",";
         }
      $json .= "\n";
      }

   }
$json .= "      ]\n";
$json .= "   }\n";
$json .= "}\n";

# write out the JSON
outputJson($json);
