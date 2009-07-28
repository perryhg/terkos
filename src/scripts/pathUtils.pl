#!/usr/bin/perl

my %paths = ();
$paths{'audio_config.json'} = '/opt/config/audio_config.json';
$paths{'AudioConfigTool'} = '/opt/bin/AudioConfigTool';
$paths{'SystemInfoTool'} = '/opt/bin/SystemInfoTool';
$paths{'wireless_networking_config.json'} = '/opt/config/wireless_networking_config.json';
$paths{'WirelessNetworkingConfigTool'} = '/opt/bin/WirelessNetworkingConfigTool';

#===================================================================================================
sub getPath()
   {
   my ($pathName) = @_;

   return $paths{$pathName};
   }
#===================================================================================================
sub setUpLdLibraryPath()
   {
   # We need to set the LD_LIBRARY_PATH environment variable when we're executing as a CGI and we
   # want to call binaries which depend on shared libraries.
   $ENV{'LD_LIBRARY_PATH'} = "/opt/lib:.";
   }
#===================================================================================================
return 1;
