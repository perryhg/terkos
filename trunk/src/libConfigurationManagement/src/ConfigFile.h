/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <fstream>
#include <iostream>
#include <string>
#include <json/json.h>
#include <StringUtilities.h>

using namespace std;

class ConfigFile
   {
   public:

      static const string DEFAULT_CONFIG_FILE_DIRECTORY;
      static const char CHAINED_PROPERTY_NAME_DELIMITER;

      static Json::Value* findProperty(Json::Value& root, const string& propertyName, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER);

      ConfigFile(const string& configFilename, const string& defaultConfigFilename, const string& configFileDirectory = DEFAULT_CONFIG_FILE_DIRECTORY) :
         configFilename(configFilename), defaultConfigFilename(defaultConfigFilename), configFileDirectory(configFileDirectory)
         {
         Json::Value config;
         if (!load(config))
            {
            cerr << "ConfigFile::ConfigFile(): failed to load config file [" << configFilename << "], reverting to default file ["
                     << defaultConfigFilename << "]" << endl;
            revertToDefault();
            if (!load(config))
               {
               // TODO: I should really throw an exception here instead
               cerr << "ConfigFile::ConfigFile(): failed to load config file [" << configFilename << "]!" << endl;
               }
            }
         }

      virtual ~ConfigFile()
         {
         // nothing to do
         }

      virtual const bool revertToDefault();

   protected:

      const unsigned int getUnsignedIntValue(const string& propertyName, const unsigned int defaultValue = 0, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER) const;
      const bool setUnsignedIntValue(const string& propertyName, const unsigned int value, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER);

      const int getIntValue(const string& propertyName, const int defaultValue = 0, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER) const;
      const bool setIntValue(const string& propertyName, const int value, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER);

      const bool getBooleanValue(const string& propertyName, const bool defaultValue = false, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER) const;
      const bool setBooleanValue(const string& propertyName, const bool value, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER);

      const string getStringValue(const string& propertyName, const string defaultValue = "", const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER) const;
      const bool setStringValue(const string& propertyName, const string value, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER);

      const unsigned int getIndexedUnsignedIntValue(const string& propertyName, const unsigned int index, const unsigned int defaultValue = 0, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER) const;
      const bool setIndexedUnsignedIntValue(const string& propertyName, const unsigned int index, const unsigned int value, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER);

      const bool appendObjectToArray(const string& propertyName, const Json::Value& obj, const char chainedPropertyNameDelimiter = CHAINED_PROPERTY_NAME_DELIMITER);

      const bool load(Json::Value& config) const;

   private:

      const string configFilename;
      const string defaultConfigFilename;
      const string configFileDirectory;

      const bool load(Json::Value& config, const string& filename) const;
      const bool save(Json::Value& config) const;
   };

#endif /* CONFIGFILE_H_ */
