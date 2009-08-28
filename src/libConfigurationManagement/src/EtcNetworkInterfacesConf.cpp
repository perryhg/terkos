/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "EtcNetworkInterfacesConf.h"

const string EtcNetworkInterfacesConf::ETC_NETWORK_INTERFACES_CONF_PATH = "/etc/network/interfaces";
const string EtcNetworkInterfacesConf::DEFAULT_ETC_NETWORK_INTERFACES_CONF_PATH = "/etc/network/interfaces.default";
const string EtcNetworkInterfacesConf::BEGIN_WLAN0_AUTOSTART = "# BEGIN_WLAN0_AUTOSTART";
const string EtcNetworkInterfacesConf::END_WLAN0_AUTOSTART = "# END_WLAN0_AUTOSTART";

void EtcNetworkInterfacesConf::save()
   {
   // Make sure the file exists.  If not, revert to default.
   if (!FileUtilities::isReadable(ETC_NETWORK_INTERFACES_CONF_PATH))
      {
      EtcNetworkInterfacesConf::revertToDefault();
      }
      
   // read in the existing conf line-by-line until we find the BEGIN_WLAN0_AUTOSTART line
   ostringstream theFile(ostringstream::out);
   bool foundAutoStartMarker = false;
   ifstream ifs(ETC_NETWORK_INTERFACES_CONF_PATH.c_str(), ifstream::in);
   string line;
   while (ifs.good())
      {
      getline(ifs, line);
      theFile << line << endl;
      if (line == BEGIN_WLAN0_AUTOSTART)
         {
         foundAutoStartMarker = true;
         break;
         }
      }

   if (foundAutoStartMarker)
      {
      // now write the auto start line appropriately (with or without the comment character)
      if (!willStartOnBootup)
         {
         theFile << "#";
         }
      theFile << "auto wlan0" << endl;

      // read and discard any lines until we find the END_WLAN0_AUTOSTART line
      while (ifs.good())
         {
         getline(ifs, line);

         if (line == END_WLAN0_AUTOSTART)
            {
            theFile << line << endl;
            break;
            }
         }
      }

   // now just read and copy the rest of the file
   while (ifs.good())
      {
      getline(ifs, line);
      theFile << line << endl;
      }

   // close the input
   ifs.close();

   // open the file for writing and write our new version
   ofstream ofs(ETC_NETWORK_INTERFACES_CONF_PATH.c_str());

   // trim the file to get rid of extra line breaks at the end
   ofs << StringUtilities::trim(theFile.str()) << endl;

   // close the interfaces file
   ofs.close();
   }
