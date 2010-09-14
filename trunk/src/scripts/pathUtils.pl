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

my %paths = ();
$paths{'audio_config.json'} = '/opt/config/audio_config.json';
$paths{'AudioConfigTool'} = '/opt/bin/AudioConfigTool';
$paths{'busybox-httpd'} = '/etc/init.d/busybox-httpd';
$paths{'httpd'} = '/usr/sbin/httpd';
$paths{'httpd.conf'} = '/etc/httpd.conf';
$paths{'etc_passwd'} = '/etc/passwd';
$paths{'etc_network_wpa_supplicant_ad_hoc_conf'} = '/etc/network/wpa_supplicant_ad_hoc.conf';
$paths{'lcd_config.json'} = '/opt/config/lcd_config.json';
$paths{'LCDConfigTool'} = '/opt/bin/LCDConfigTool';
$paths{'PowerInfoTool'} = '/opt/bin/PowerInfoTool';
$paths{'TerkOSConfigTool'} = '/opt/bin/TerkOSConfigTool';
$paths{'user_programs_config.json'} = '/opt/config/user_programs_config.json';
$paths{'user_programs_dir'} = '/opt/usr/bin';
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
