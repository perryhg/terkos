/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "WpaSupplicantConf.h"

const string WpaSupplicantConf::WPA_SUPPLICANT_CONF_PATH = "/etc/network/wpa_supplicant.conf";
const string WpaSupplicantConf::DEFAULT_WPA_SUPPLICANT_CONF_PATH = "/etc/network/wpa_supplicant.default.conf";

void WpaSupplicantConf::save()
   {
   // open the output stream to write to the wpa_supplicant.conf file
   ofstream wpa_supplicant_conf(WPA_SUPPLICANT_CONF_PATH.c_str());

   // write the header stuff
   wpa_supplicant_conf << "ctrl_interface=/var/run/wpa_supplicant" << endl;
   wpa_supplicant_conf << "ctrl_interface_group=0" << endl;

   // write the networks
   for (unsigned int i=0; i<networks.size(); i++)
      {
      wpa_supplicant_conf << endl;
      wpa_supplicant_conf << "network={" << endl;
      wpa_supplicant_conf << "        ssid=\"" << networks[i].getSsid() << "\"" << endl;
      wpa_supplicant_conf << "        key_mgmt=NONE" << endl;
      wpa_supplicant_conf << "        priority=" << networks[i].getPriority() << endl;
      wpa_supplicant_conf << "}" << endl;
      }

   // if desired, write the catch-all network which will cause it to connect to any network
   if (willIncludeCatchAllNetwork)
      {
      wpa_supplicant_conf << endl;
      wpa_supplicant_conf << "network={" << endl;
      wpa_supplicant_conf << "        key_mgmt=NONE" << endl;
      wpa_supplicant_conf << "        priority=0" << endl;
      wpa_supplicant_conf << "}" << endl;
      }

   // close the wpa_supplicant.conf file
   wpa_supplicant_conf.close();
   }
