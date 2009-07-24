#!/usr/bin/perl

my %paths = ();
$paths{'audio_config.json'} = '/opt/config/audio_config.json';
$paths{'AudioConfigTool'} = '/opt/bin/AudioConfigTool';
$paths{'wireless_networking_config.json'} = '/opt/config/wireless_networking_config.json';
$paths{'WirelessNetworkingConfigTool'} = '/opt/bin/WirelessNetworkingConfigTool';

#===================================================================================================
sub getPath()
   {
   my ($pathName) = @_;

   return $paths{$pathName};
   }
#===================================================================================================
return 1;
