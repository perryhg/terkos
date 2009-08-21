#!/usr/bin/perl

my %paths = ();
$paths{'audio_config.json'} = '/opt/config/audio_config.json';
$paths{'AudioConfigTool'} = '/opt/bin/AudioConfigTool';
$paths{'busybox-httpd'} = '/etc/init.d/busybox-httpd';
$paths{'httpd'} = '/usr/sbin/httpd';
$paths{'httpd.conf'} = '/etc/httpd.conf';
$paths{'etc_passwd'} = '/etc/passwd';
$paths{'lcd_config.json'} = '/opt/config/lcd_config.json';
$paths{'LCDConfigTool'} = '/opt/bin/LCDConfigTool';
$paths{'PowerInfoTool'} = '/opt/bin/PowerInfoTool';
$paths{'user_programs_config.json'} = '/opt/config/user_programs_config.json';
$paths{'UserProgramsConfigTool'} = '/opt/bin/UserProgramsConfigTool';
$paths{'VersionInfoTool'} = '/opt/bin/VersionInfoTool';
$paths{'WebControlPanelPasswordTool'} = '/opt/bin/WebControlPanelPasswordTool';
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
