#!/usr/bin/perl

################################################################################
# Program for resetting the configuration for the various systems.
#
# If called from the command line, the usage is:
#
#       ./resetPreferences.pl --systemsToReset=system1[,system2[,system3...]]
#
# If called as a CGI, it expects there to be a parameter named systemsToReset
# whose value is a comma delimited list of the system(s) to reset.  Valid system
# names are:
#
#    Audio
#    LCD
#    Wireless Networking
#    User Programs
#    Passwords
#
# Regardless of how the script is called, it returns the result as JSON.
################################################################################

require "/opt/scripts/commandLineUtils.pl";
require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

# parse command line switches or HTTP request params
my %arguments = ();
if (isHttpRequest())
   {
   %arguments = parseHttpRequestParameters();
   }
else
   {
   %arguments = parseCommandLineSwitches();
   }

# create the JSON output
my %response = ();
my $webServerNeedsRestart = 0;
if (exists($arguments{'systemsToReset'}))
   {
   my %systemsToReset = ();
   foreach $systemToReset (split(/,/, $arguments{'systemsToReset'}))
      {
      $systemsToReset{$systemToReset} = 1;
      }

   my $success = 1;
   if (exists($systemsToReset{'Audio'}))
      {
      $success = $success && &resetSystem('AudioConfigTool');
      }
   if (exists($systemsToReset{'LCD'}))
      {
      $success = $success && &resetSystem('LCDConfigTool');
      }
   if (exists($systemsToReset{'Wireless Networking'}))
      {
      $success = $success && &resetSystem('WirelessNetworkingConfigTool');
      }
   if (exists($systemsToReset{'User Programs'}))
      {
      $success = $success && &resetSystem('UserProgramsConfigTool');
      }
   if (exists($systemsToReset{'Passwords'}))
      {
      $webServerNeedsRestart = &resetSystem('WebControlPanelPasswordTool');
      $success = $success && $webServerNeedsRestart;
      }
   $response{'statusCode'} = '200';
   $response{'statusName'} = 'OK';
   if ($success)
      {
      $response{'ok'} = 'true';
      $response{'message'} = 'reset successful';
      }
   else
      {
      $response{'ok'} = 'false';
      $response{'message'} = 'reset failed: one or more systems could not be reset';
      }
   }
else
   {
   $response{'statusCode'} = '400';
   $response{'statusName'} = 'Bad Request';
   $response{'ok'} = 'false';
   $response{'message'} = 'missing parameter: systemsToReset';
   }

my $jsonOutput = '';
$jsonOutput .= "{\n";
$jsonOutput .= '"ok" : '.$response{'ok'}.',' . "\n";
$jsonOutput .= '"status_code" : '.$response{'statusCode'}.',' . "\n";
$jsonOutput .= '"message" : "'.$response{'message'}.'"' . "\n";
$jsonOutput .= "}\n";

# write out the JSON
if (&isHttpRequest())
   {
   print "HTTP/1.0 $response{'statusCode'} $response{'statusName'}\n";
   }
&outputJson($jsonOutput);

# now restart the web server so that the changes take effect
if ($webServerNeedsRestart)
   {
   &restartWebServer();
   }

#===================================================================================================
sub resetSystem()
   {
   my ($programName) = @_;
   
   my $programPath = &getPath($programName);
   my $command = "$programPath --reset";
   open(PROGRAM_OUTPUT,"$command |") or die "Failed to call $command $!\n";

   # read the output into a string
   my $output = join("", <PROGRAM_OUTPUT>);
   chomp $output;
   
   # close the filehandle
   close(PROGRAM_OUTPUT);

   return ($output eq '1');
   }
#===================================================================================================

