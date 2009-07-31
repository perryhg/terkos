#!/usr/bin/perl

################################################################################
# Program for setting the HTTP Basic Auth password for the admin user.  Can be
# called either via the command line or as a CGI.
#
# If called from the command line, the usage is:
#
#       ./setHttpBasicAuthPassword.pl --newPassword=PASSWORD
#
# If called as a CGI, it expects the to be a parameter named newPassword whose
# value is the new password.
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
if (exists($arguments{'newPassword'}))
   {
   # fork so we can safely call the httpd program and give it the user's password
   $pid = open(HTTPD_OUTPUT, "-|");
   if ($pid)      # parent
      {
      # read the output into a string
      my $encryptedPassword = join("", <HTTPD_OUTPUT>);
      chomp $encryptedPassword;

      # close the handle
      close(HTTPD_OUTPUT) || warn "kid exited $?";

      # now write the httpd.conf file--the format is PATH:USER:PASSWORD, so since we always want to protect
      # everything under the web root for the admin user, the line will be /:admin:PASSWORD
      my $pathToHttpdConf = &getPath('httpd.conf');
      open(HTTP_CONF_FILE, ">$pathToHttpdConf") || die "failed to open $pathToHttpdConf: $!";
      print HTTP_CONF_FILE "/:admin:$encryptedPassword\n";
      close(HTTP_CONF_FILE);

      # set the flag so the web server gets restarted
      $webServerNeedsRestart = 1;

      $response{'statusCode'} = '200';
      $response{'statusName'} = 'OK';
      $response{'ok'} = 'true';
      $response{'message'} = 'password changed';
      }
   else           # child
      {
      my $newPassword = $arguments{'newPassword'};
      my $pathToHttpd = &getPath('httpd');
      exec($pathToHttpd, "-m", $newPassword) || die "can't exec $pathToHttpd: $!";

      # exec exits this script, so this is never reached
      }
   }
else
   {
   $response{'statusCode'} = '400';
   $response{'statusName'} = 'Bad Request';
   $response{'ok'} = 'false';
   $response{'message'} = 'missing parameter: newPassword';
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
   my $pathToBusyboxHttpd = &getPath('busybox-httpd');
   my $restartCommand = "$pathToBusyboxHttpd restart";

   # issue the restart
   open(WEB_SERVER_OUTPUT, "$restartCommand |") or die "Failed to call $restartCommand: $!\n";

   # Read the output into an array -- we won't actually do anything with the output but I
   # found that if I don't read it then the command doesn't actually execute.  That seems
   # weird, but, oh well.  There's also no point in parsing the output because even if it
   # reports some kind of error there's nothing we can do about it since we've already sent
   # the response back to the user.
   my @lines = <WEB_SERVER_OUTPUT>;

   # close the filehandle
   close(WEB_SERVER_OUTPUT);
   }
