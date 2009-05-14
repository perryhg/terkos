/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ConfigFile.h"

void ConfigFile::revertToDefault()
   {
   try
      {
      Config config;

      // load the default file
      config.readFile(defaultConfigFilename.c_str());

      // save it as the configuration
      config.writeFile(configFilename.c_str());
      }
   catch (...)
      {
      // TODO: I should do something better here
      cerr << "ConfigFile::revertToDefault(): failed to revert to defaults!" << endl;
      }
   }

const unsigned int ConfigFile::getUnsignedIntValue(const string& propertyName, const unsigned int defaultValue) const
   {
   Config config;
   if (load(config))
      {
      unsigned int value;
      if (config.lookupValue(propertyName, value))
         {
         return value;
         }
      }
   return defaultValue;
   }

const bool ConfigFile::setUnsignedIntValue(const string& propertyName, const unsigned int value)
   {
   return setIntValue(propertyName, value);
   }

const int ConfigFile::getIntValue(const string& propertyName, const int defaultValue) const
   {
   Config config;
   if (load(config))
      {
      int value;
      if (config.lookupValue(propertyName, value))
         {
         return value;
         }
      }
   return defaultValue;
   }

const bool ConfigFile::setIntValue(const string& propertyName, const int value)
   {
   Config config;
   if (load(config))
      {
      try
         {
         Setting& setting = config.lookup(propertyName);
         setting = (int) value;
         return save(config);
         }
      catch (...)
         {
         cerr << "ConfigFile::setIntValue(): failed to set property [" << propertyName << "] to value [" << value << "]!" << endl;
         }
      }

   return false;
   }

const bool ConfigFile::getBooleanValue(const string& propertyName, const bool defaultValue) const
   {
   Config config;
   if (load(config))
      {
      bool value;
      if (config.lookupValue(propertyName, value))
         {
         return value;
         }
      }
   return defaultValue;
   }

const bool ConfigFile::setBooleanValue(const string& propertyName, const bool value)
   {
   Config config;
   if (load(config))
      {
      try
         {
         Setting& setting = config.lookup(propertyName);
         setting = (bool) value;
         return save(config);
         }
      catch (...)
         {
         cerr << "ConfigFile::setBooleanValue(): failed to set property [" << propertyName << "] to value [" << value << "]!" << endl;
         }
      }

   return false;
   }

const unsigned int ConfigFile::getIndexedUnsignedIntValue(const string& propertyName, const unsigned int index, const unsigned int defaultValue) const
   {
   Config config;
   if (load(config))
      {
      try
         {
         Setting& setting = config.lookup(propertyName);
         return setting[index];
         }
      catch (...)
         {
         cerr << "ConfigFile::getIndexedUnsignedIntValue(): failed to find property [" << propertyName << "]!" << endl;
         }
      }
   return defaultValue;
   }

const bool ConfigFile::setIndexedUnsignedIntValue(const string& propertyName, const unsigned int index, const unsigned int value)
   {
   Config config;
   if (load(config))
      {
      try
         {
         Setting& setting = config.lookup(propertyName);
         setting[index] = (int) value;
         return save(config);
         }
      catch (...)
         {
         cerr << "ConfigFile::setIndexedUnsignedIntValue(): failed to set index [" << index << "] on property [" << propertyName << "] to value [" << value << "]!" << endl;
         }
      }

   return false;
   }

const bool ConfigFile::load(Config& config) const
   {
   try
      {
      // try to load the file
      config.readFile(configFilename.c_str());
      return true;
      }
   catch (...)
      {
      cerr << "ConfigFile::loadFile(): failed to load file [" << configFilename << "]" << endl;
      }
   return false;
   }

const bool ConfigFile::save(Config& config) const
   {
   try
      {
      // try to save the file
      config.writeFile(configFilename.c_str());
      return true;
      }
   catch (...)
      {
      cerr << "ConfigFile::save(): failed to save file [" << configFilename << "]" << endl;
      }
   return false;
   }
