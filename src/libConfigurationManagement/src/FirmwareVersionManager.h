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

#ifndef FIRMWAREVERSIONMANAGER_H_
#define FIRMWAREVERSIONMANAGER_H_

#include <algorithm>
#include <string>
#include "ConfigFile.h"

using namespace std;

class FirmwareVersionManager : public ConfigFile
   {
   public:

      FirmwareVersionManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~FirmwareVersionManager()
         {
         // nothing to do
         }

      const string getVersion() const;
      const string getMajorVersion() const;
      const string getMinorVersion() const;
      const string getRevision() const;
      const string getTimestamp() const;

      Json::Value getJSON();

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;

      static const string FIRMWARE_ELEMENT_NAME;
      static const string VERSION_ELEMENT_NAME;
      static const string MAJOR_ELEMENT_NAME;
      static const string MINOR_ELEMENT_NAME;
      static const string REVISION_ELEMENT_NAME;
      static const string TIMESTAMP_ELEMENT_NAME;

      static const string MAJOR_VERSION_PROPERTY;
      static const string MINOR_VERSION_PROPERTY;
      static const string REVISION_PROPERTY;
      static const string TIMESTAMP_PROPERTY;

   };

#endif /* FIRMWAREVERSIONMANAGER_H_ */
