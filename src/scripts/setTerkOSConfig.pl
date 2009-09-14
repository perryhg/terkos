#!/usr/bin/perl

require "/opt/scripts/cgi-lib.pl";
require "/opt/scripts/fileUtils.pl";
require "/opt/scripts/pathUtils.pl";
require "/opt/scripts/httpUtils.pl";
require "/opt/scripts/jsonUtils.pl";

use JSON;

# Save the files to the temp directory
$cgi_lib::writefiles = '/tmp';

# Limit upload size to 1 meg
$cgi_lib::maxdata = 1000000;

my $ret = &ReadParse();

my %response = ();
my $jsonResult = '{ "ok" : false, "updated-components" : [] }';
if (defined $ret && $ret == 1 && defined $in{'configFile'} && -e $in{'configFile'})
   {
   # build the command
   my $command = 'cat ' . $in{'configFile'} . ' | ' . &getPath('TerkOSConfigTool');

   # Call the command
   open(OUTPUT,"$command |") or die "Failed to call $command $!\n";

   # read the output into a string
   $jsonResult = join("", <OUTPUT>);

   # close the filehandle
   close(OUTPUT);

   if ($jsonResult)
      {
      $jsonObj = from_json($jsonResult);

      if (defined $jsonObj->{'ok'} && $jsonObj->{'ok'} eq 'true')
         {
         $response{'statusCode'} = '200';
         $response{'statusName'} = 'OK';
         $response{'ok'} = 'true';
         $response{'message'} = (defined $jsonObj->{'message'}) ? $jsonObj->{'message'} : 'configuration updated';
         }
      else
         {
         $response{'statusCode'} = '400';
         $response{'statusName'} = 'Bad Request';
         $response{'ok'} = 'false';
         $response{'message'} = (defined $jsonObj->{'message'}) ? $jsonObj->{'message'} : 'update failed';
         }
      }
   else
      {
      $response{'statusCode'} = '400';
      $response{'statusName'} = 'Bad Request';
      $response{'ok'} = 'false';
      $response{'message'} = 'update failed';
      }
   }
else
   {
   $response{'statusCode'} = '400';
   $response{'statusName'} = 'Bad Request';
   $response{'ok'} = 'false';
   $response{'message'} = 'missing parameter: configFile';
   }

my $jsonOutput = '';
$jsonOutput .= "{\n";
$jsonOutput .= '"ok" : '.$response{'ok'}.',' . "\n";
$jsonOutput .= '"status_code" : '.$response{'statusCode'}.',' . "\n";
$jsonOutput .= '"status_detail" : '.$jsonResult.',' . "\n";
$jsonOutput .= '"message" : "'.$response{'message'}.'"' . "\n";
$jsonOutput .= "}\n";

# print "Content-Type: application/json\n";
# write out the JSON
if (&isHttpRequest())
   {
   print "HTTP/1.0 $response{'statusCode'} $response{'statusName'}\n";
   }
print "Content-Type: text/html\n";
print "Expires: Wed, 25 Apr 2007 14:00:00 GMT\n";
print "Cache-Control: no-cache\n\n";
print "$jsonOutput\n";