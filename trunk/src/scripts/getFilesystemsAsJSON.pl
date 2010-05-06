#!/usr/bin/perl

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

require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# Call mount to get the filesystems
open(DISK_USAGE,"/bin/mount |") or die "Failed to call /bin/mount $!\n";

# read the output into an array
my @lines = <DISK_USAGE>;

# close the filehandle
close(DISK_USAGE);

# create the JSON representation
my $json = '';
$json .= "{\n";
$json .= '   "mount-points" : ' . "\n";
$json .= "   {\n";
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
      if ($line =~ /^(\S+)\s+on\s+(\S+)\s+type\s+(\S+)\s+\((\S+)\)$/)
         {
         $i++;
         $filesystems[$i]->{'filesystem'} = $1;
         $filesystems[$i]->{'mounted-on'} = $2;
         $filesystems[$i]->{'type'} = $3;
         $filesystems[$i]->{'attributes'} = $4;
         }
      }

   for $j (0 .. $#filesystems)
      {
      $json .= '      "' . $filesystems[$j]->{'mounted-on'} . '" : ' . "\n";
      $json .= "      {\n";
      $json .= '         "filesystem" : "' . $filesystems[$j]->{'filesystem'} . '",' . "\n";
      $json .= '         "type" : "'       . $filesystems[$j]->{'type'}       . '",' . "\n";
      $json .= '         "attributes" : "' . $filesystems[$j]->{'attributes'} . '"' . "\n";
      $json .= "      }";
      if ($j < $#filesystems)
         {
         $json .= ",";
         }
      $json .= "\n";
      }

   }
$json .= "   }\n";
$json .= "}\n";

# write out the JSON
outputJson($json);
