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
