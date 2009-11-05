#!/usr/bin/perl

require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/commandLineUtils.pl";
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

# look for the "volume" parameter
my %response = ();
if (exists($arguments{volume}))
   {
   my $volume = $arguments{volume};

   # for security, make sure the volume is a number
   if ($volume =~ /^\d+$/)
      {
      my $program = &getPath('AudioConfigTool');
      my $command = "$program --play-sample $volume";
      open(PROGRAM_OUTPUT,"$command |") or die "Failed to call $command $!\n";

      $response{'statusCode'} = '200';
      $response{'statusName'} = 'OK';
      $response{'ok'} = 'true';
      $response{'message'} = 'OK';
      }
   else
      {
      $response{'statusCode'} = '400';
      $response{'statusName'} = 'Bad Request';
      $response{'ok'} = 'false';
      $response{'message'} = 'Volume must be an integer';
      }
   }
else
   {
   $response{'statusCode'} = '400';
   $response{'statusName'} = 'Bad Request';
   $response{'ok'} = 'false';
   $response{'message'} = 'missing parameter: volume';
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

