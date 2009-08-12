/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef VERSIONINFOMANAGER_H_
#define VERSIONINFOMANAGER_H_

#include <string.h>
#include <json/json.h>
#include "ConfigFile.h"
#include <pstream.h>
#include "qwerkhw.h"
#include "StringUtilities.h"
#include "FirmwareVersionManager.h"

using namespace std;
using namespace redi;

class VersionInfoManager
   {
   public:

      VersionInfoManager()
         {
         // get the hardware version
         long val = 0;
         if (hardware.GetProperty(QHW_PROP_HARDWARE_VERSION, &val) == PROP_OK)
            {
            hardwareVersion = StringUtilities::convertIntToString((int) val);
            }
         else
            {
            hardwareVersion = UNKNOWN_VALUE;
            }

         // call uname to get system info
         ipstream uname1("/bin/uname -s");
         if (!getline(uname1, unameOperatingSystemName))
            {
            unameOperatingSystemName = UNKNOWN_VALUE;
            }
         ipstream uname2("/bin/uname -n");
         if (!getline(uname2, unameHostname))
            {
            unameHostname = UNKNOWN_VALUE;
            }
         ipstream uname3("/bin/uname -r");
         if (!getline(uname3, unameOperatingSystemRelease))
            {
            unameOperatingSystemRelease = UNKNOWN_VALUE;
            }
         ipstream uname4("/bin/uname -v");
         if (!getline(uname4, unameOperatingSystemVersion))
            {
            unameOperatingSystemVersion = UNKNOWN_VALUE;
            }
         ipstream uname5("/bin/uname -m");
         if (!getline(uname5, unameHardwareType))
            {
            unameHardwareType = UNKNOWN_VALUE;
            }
         ipstream uname6("/bin/uname -p");
         if (!getline(uname6, unameProcessorType))
            {
            unameProcessorType = UNKNOWN_VALUE;
            }
         }

      virtual ~VersionInfoManager()
         {
         // nothing to do
         }

      const string getHardwareVersion() const;
      const string getFirmwareVersion() const;
      const string getFirmwareMajorVersion() const;
      const string getFirmwareMinorVersion() const;
      const string getFirmwareRevision() const;
      const string getFirmwareTimestamp() const;

      const string getOperatingSystemName() const;
      const string getOperatingSystemVersion() const;
      const string getOperatingSystemRelease() const;
      const string getProcessorType() const;
      const string getMachineType() const;
      const string getHostname() const;

      Json::Value getJSON();

   private:

      static const string FIRMWARE_CONFIG_FILENAME;

      static const string UNKNOWN_VALUE;

      CQwerkHardware hardware;

      string hardwareVersion;
      FirmwareVersionManager firmwareVersionManager;

      string unameOperatingSystemName;
      string unameHostname;
      string unameOperatingSystemRelease;
      string unameOperatingSystemVersion;
      string unameHardwareType;
      string unameProcessorType;

   };

#endif /* VERSIONINFOMANAGER_H_ */
