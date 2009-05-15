/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include <iostream>
#include <libconfig.h++>
#include <string>

using namespace std;
using namespace libconfig;

class ConfigFile
   {
   public:

      ConfigFile(const string& configFilename, const string& defaultConfigFilename) :
         configFilename(configFilename), defaultConfigFilename(defaultConfigFilename)
         {
         Config config;
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

      void revertToDefault();

   protected:

      const unsigned int getUnsignedIntValue(const string& propertyName, const unsigned int defaultValue = 0) const;
      const bool setUnsignedIntValue(const string& propertyName, const unsigned int value);

      const int getIntValue(const string& propertyName, const int defaultValue = 0) const;
      const bool setIntValue(const string& propertyName, const int value);

      const bool getBooleanValue(const string& propertyName, const bool defaultValue = false) const;
      const bool setBooleanValue(const string& propertyName, const bool value);

      const unsigned int getIndexedUnsignedIntValue(const string& propertyName, const unsigned int index, const unsigned int defaultValue = 0) const;
      const bool setIndexedUnsignedIntValue(const string& propertyName, const unsigned int index, const unsigned int value);


   private:

      const string configFilename;
      const string defaultConfigFilename;

      // TODO: For safety, load and save should default to loading from and saving to a particular directory such as /opt/config
      const bool load(Config& config) const;
      const bool save(Config& config) const;

   };

#endif /* CONFIGFILE_H_ */
