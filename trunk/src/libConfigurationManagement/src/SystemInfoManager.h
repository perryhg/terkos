/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef SYSTEMINFOMANAGER_H_
#define SYSTEMINFOMANAGER_H_

#include <string.h>
#include <json/json.h>
#include <pstream.h>
#include "qwerkhw.h"
#include "StringUtilities.h"

using namespace std;
using namespace redi;

class SystemInfoManager
   {
   public:

      SystemInfoManager()
         {
         hardwareVersion = StringUtilities::convertIntToString(hardware.GetVersion());
         firmwareVersion = "1.0.0"; // TODO: get this version number from the proper place

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

      virtual ~SystemInfoManager()
         {
         // nothing to do
         }

      const string getHardwareVersion() const;
      const string getFirmwareVersion() const;

      const string getOperatingSystemName() const;
      const string getOperatingSystemVersion() const;
      const string getOperatingSystemRelease() const;
      const string getProcessorType() const;
      const string getMachineType() const;
      const string getHostname() const;

      const unsigned short getBatteryVoltage();
      const unsigned short getMotorVoltage();
      const unsigned short get5VVoltage();
      const unsigned short getTemperature();

      Json::Value getJSON();

   private:

      static const string UNKNOWN_VALUE;

      CQwerkHardware hardware;

      string hardwareVersion;
      string firmwareVersion;

      string unameOperatingSystemName;
      string unameHostname;
      string unameOperatingSystemRelease;
      string unameOperatingSystemVersion;
      string unameHardwareType;
      string unameProcessorType;

   };

#endif /* SYSTEMINFOMANAGER_H_ */
