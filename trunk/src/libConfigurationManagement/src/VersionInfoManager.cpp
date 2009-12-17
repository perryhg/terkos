//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "VersionInfoManager.h"

const string VersionInfoManager::UNKNOWN_VALUE = "???";

const string VersionInfoManager::getHardwareVersion() const
   {
   return hardwareVersion;
   }

const string VersionInfoManager::getFirmwareVersion() const
   {
   return firmwareVersionManager.getVersion();
   }

const string VersionInfoManager::getFirmwareMajorVersion() const
   {
   return firmwareVersionManager.getMajorVersion();
   }

const string VersionInfoManager::getFirmwareMinorVersion() const
   {
   return firmwareVersionManager.getMinorVersion();
   }

const string VersionInfoManager::getFirmwareRevision() const
   {
   return firmwareVersionManager.getRevision();
   }

const string VersionInfoManager::getFirmwareTimestamp() const
   {
   return firmwareVersionManager.getTimestamp();
   }

const string VersionInfoManager::getOperatingSystemName() const
   {
   return unameOperatingSystemName;
   }

const string VersionInfoManager::getOperatingSystemVersion() const
   {
   return unameOperatingSystemVersion;
   }

const string VersionInfoManager::getOperatingSystemRelease() const
   {
   return unameOperatingSystemRelease;
   }

const string VersionInfoManager::getProcessorType() const
   {
   return unameProcessorType;
   }

const string VersionInfoManager::getMachineType() const
   {
   return unameHardwareType;
   }

const string VersionInfoManager::getHostname() const
   {
   return unameHostname;
   }

Json::Value VersionInfoManager::getJSON()
   {
   Json::Value operatingSystemElement;
   operatingSystemElement["name"] = getOperatingSystemName();
   operatingSystemElement["release"] = getOperatingSystemRelease();
   operatingSystemElement["version"] = getOperatingSystemVersion();
   operatingSystemElement["machine-type"] = getMachineType();
   operatingSystemElement["processor-type"] = getProcessorType();
   operatingSystemElement["hostname"] = getHostname();

   Json::Value systemInfoElement;
   systemInfoElement["hardware"] = getHardwareVersion();

   Json::Value firmwareElement = firmwareVersionManager.getJSON();
   systemInfoElement["firmware"] = firmwareElement["firmware"];
   systemInfoElement["operating-system"] = operatingSystemElement;

   Json::Value rootElement;
   rootElement["version-info"] = systemInfoElement;

   return rootElement;
   }
