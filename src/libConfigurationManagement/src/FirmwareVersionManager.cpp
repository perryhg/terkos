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
   return getStringValue(REVISION_PROPERTY);
   }

const string FirmwareVersionManager::getTimestamp() const
   {
   return getStringValue(TIMESTAMP_PROPERTY);
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
