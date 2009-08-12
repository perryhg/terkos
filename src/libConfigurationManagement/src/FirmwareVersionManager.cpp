/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "FirmwareVersionManager.h"

const string FirmwareVersionManager::CONFIG_FILENAME = "firmware.json";
const string FirmwareVersionManager::DEFAULT_CONFIG_FILENAME = "firmware.json";

const string FirmwareVersionManager::FIRMWARE_ELEMENT_NAME = "firmware";
const string FirmwareVersionManager::VERSION_ELEMENT_NAME = "version";
const string FirmwareVersionManager::MAJOR_ELEMENT_NAME = "major";
const string FirmwareVersionManager::MINOR_ELEMENT_NAME = "minor";
const string FirmwareVersionManager::REVISION_ELEMENT_NAME = "revision";
const string FirmwareVersionManager::TIMESTAMP_ELEMENT_NAME = "timestamp";
const string FirmwareVersionManager::MAJOR_VERSION_PROPERTY = FIRMWARE_ELEMENT_NAME + "." + VERSION_ELEMENT_NAME + "." + MAJOR_ELEMENT_NAME;
const string FirmwareVersionManager::MINOR_VERSION_PROPERTY = FIRMWARE_ELEMENT_NAME + "." + VERSION_ELEMENT_NAME + "." + MINOR_ELEMENT_NAME;
const string FirmwareVersionManager::REVISION_PROPERTY = FIRMWARE_ELEMENT_NAME + "." + VERSION_ELEMENT_NAME + "." + REVISION_ELEMENT_NAME;
const string FirmwareVersionManager::TIMESTAMP_PROPERTY = FIRMWARE_ELEMENT_NAME + "." + TIMESTAMP_ELEMENT_NAME;

const string FirmwareVersionManager::getVersion() const
   {
   return getMajorVersion() + "." + getMinorVersion() + "." + getRevision();
   }

const string FirmwareVersionManager::getMajorVersion() const
   {
   return getStringValue(MAJOR_VERSION_PROPERTY);
   }

const string FirmwareVersionManager::getMinorVersion() const
   {
   return getStringValue(MINOR_VERSION_PROPERTY);
   }

const string FirmwareVersionManager::getRevision() const
   {
   string str = getStringValue(REVISION_PROPERTY);

   // The revision number is inserted by SVN by keyword replacement, so we need to parse
   // a string that looks like this:
   //
   //    "$LastChangedRevision: 866 $"
   //
   // to pick out just the revision number.
   size_t pos = str.find_first_of(" ");
   if (pos != string::npos && pos + 1 != string::npos)
      {
      str = str.substr(pos + 1);
      pos = str.find_first_of(" ");
      if (pos != string::npos && pos + 1 != string::npos)
         {
         return str.substr(0, pos);
         }
      }

   return str;
   }

const string FirmwareVersionManager::getTimestamp() const
   {
   string str = getStringValue(TIMESTAMP_PROPERTY);

   // The timestamp is inserted by SVN by keyword replacement, so we need to parse
   // a string that looks like this:
   //
   //    "$LastChangedDate: 2009-08-12 12:25:36 -0400 (Wed, 12 Aug 2009) $"
   //
   // to pick out just the revision number.
   size_t pos = str.find_first_of(" ");
   if (pos != string::npos && pos + 1 != string::npos)
      {
      str = str.substr(pos + 1);
      pos = str.find_last_of(" ");
      if (pos != string::npos && pos + 1 != string::npos)
         {
         return str.substr(0, pos);
         }
      }

   return str;
   }

Json::Value FirmwareVersionManager::getJSON()
   {
   // build the version element
   Json::Value versionElement;
   versionElement[MAJOR_ELEMENT_NAME] = getMajorVersion();
   versionElement[MINOR_ELEMENT_NAME] = getMinorVersion();
   versionElement[REVISION_ELEMENT_NAME] = getRevision();

   // build the firmware element
   Json::Value firmwareElement;
   firmwareElement[VERSION_ELEMENT_NAME] = versionElement;
   firmwareElement[TIMESTAMP_ELEMENT_NAME] = getTimestamp();

   Json::Value rootElement;
   rootElement[FIRMWARE_ELEMENT_NAME] = firmwareElement;

   return rootElement;
   }
