/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "SystemInfoManager.h"

const string SystemInfoManager::UNKNOWN_VALUE = "???";

const string SystemInfoManager::getHardwareVersion() const
   {
   return hardwareVersion;
   }

const string SystemInfoManager::getFirmwareVersion() const
   {
   return firmwareVersion;
   }

const string SystemInfoManager::getOperatingSystemName() const
   {
   return unameOperatingSystemName;
   }

const string SystemInfoManager::getOperatingSystemVersion() const
   {
   return unameOperatingSystemVersion;
   }

const string SystemInfoManager::getOperatingSystemRelease() const
   {
   return unameOperatingSystemRelease;
   }

const string SystemInfoManager::getProcessorType() const
   {
   return unameProcessorType;
   }

const string SystemInfoManager::getMachineType() const
   {
   return unameHardwareType;
   }

const string SystemInfoManager::getHostname() const
   {
   return unameHostname;
   }

const unsigned short SystemInfoManager::getBatteryVoltage()
   {
   return hardware.GetBattVoltage();
   }

const unsigned short SystemInfoManager::getMotorVoltage()
   {
   return hardware.GetMotorVoltage();
   }

const unsigned short SystemInfoManager::get5VVoltage()
   {
   return hardware.Get5VVoltage();
   }

const unsigned short SystemInfoManager::getTemperature()
   {
   return hardware.GetTemp();
   }

Json::Value SystemInfoManager::getJSON()
   {
   Json::Value hardwareElement;
   hardwareElement["version"] = getHardwareVersion();

   Json::Value firmwareElement;
   firmwareElement["version"] = getFirmwareVersion();

   Json::Value operatingSystemElement;
   operatingSystemElement["name"] = getOperatingSystemName();
   operatingSystemElement["release"] = getOperatingSystemRelease();
   operatingSystemElement["version"] = getOperatingSystemVersion();
   operatingSystemElement["machine-type"] = getMachineType();
   operatingSystemElement["processor-type"] = getProcessorType();
   operatingSystemElement["hostname"] = getHostname();

   Json::Value voltageElement;
   voltageElement["battery"] = getBatteryVoltage();
   voltageElement["motor"] = getMotorVoltage();
   voltageElement["five-volt"] = get5VVoltage();

   Json::Value systemInfoElement;
   systemInfoElement["hardware"] = hardwareElement;
   systemInfoElement["firmware"] = firmwareElement;
   systemInfoElement["operating-system"] = operatingSystemElement;
   systemInfoElement["voltage"] = voltageElement;
   systemInfoElement["temperature"] = getTemperature();

   Json::Value rootElement;
   rootElement["system-info"] = systemInfoElement;

   return rootElement;
   }
