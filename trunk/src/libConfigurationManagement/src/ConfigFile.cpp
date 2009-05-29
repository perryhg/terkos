/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ConfigFile.h"

const char ConfigFile::CHAINED_PROPERTY_NAME_DELIMITER = '.';

void ConfigFile::revertToDefault()
   {
   // TODO: This method needs better error handling

   try
      {
      // load the default file
      Json::Value config;
      if (load(config, defaultConfigFilename))
         {
         // save it as the configuration
         if (!save(config))
            {
            cerr << "ConfigFile::revertToDefault(): failed to save the config file [" << configFilename << "]!" << endl;
            }
         }
      else
         {
         cerr << "ConfigFile::revertToDefault(): failed to load the default config file [" << defaultConfigFilename << "]!" << endl;
         }
      }
   catch (...)
      {
      cerr << "ConfigFile::revertToDefault(): failed to revert to defaults!" << endl;
      }
   }

const unsigned int ConfigFile::getUnsignedIntValue(const string& propertyName, const unsigned int defaultValue) const
   {
   Json::Value root;
   if (load(root))
      {
      Json::Value* property = findProperty(root, propertyName);
      if (property != NULL)
         {
         return property->asUInt();
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
   Json::Value root;
   if (load(root))
      {
      Json::Value* property = findProperty(root, propertyName);
      if (property != NULL)
         {
         return property->asInt();
         }
      }
   return defaultValue;
   }

const bool ConfigFile::setIntValue(const string& propertyName, const int value)
   {
   Json::Value root;
   if (load(root))
      {
      try
         {
         Json::Value* property = findProperty(root, propertyName);
         if (property != NULL)
            {
            (*property) = (int) value;
            return save(root);
            }
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
   Json::Value root;
   if (load(root))
      {
      Json::Value* property = findProperty(root, propertyName);
      if (property != NULL)
         {
         return property->asBool();
         }
      }
   return defaultValue;
   }

const bool ConfigFile::setBooleanValue(const string& propertyName, const bool value)
   {
   Json::Value root;
   if (load(root))
      {
      try
         {
         Json::Value* property = findProperty(root, propertyName);
         if (property != NULL)
            {
            (*property) = (bool) value;
            return save(root);
            }
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
   Json::Value root;
   if (load(root))
      {
      Json::Value* property = findProperty(root, propertyName);
      if (property != NULL && property->isValidIndex(index))
         {
         return (*property)[index].asUInt();
         }
      }
   return defaultValue;
   }

const bool ConfigFile::setIndexedUnsignedIntValue(const string& propertyName, const unsigned int index, const unsigned int value)
   {
   Json::Value root;
   if (load(root))
      {
      try
         {
         Json::Value* property = findProperty(root, propertyName);
         if (property != NULL)
            {
            (*property)[index] = (unsigned int) value;
            return save(root);
            }
         }
      catch (...)
         {
         cerr << "ConfigFile::setIndexedUnsignedIntValue(): failed to set index [" << index << "] on property [" << propertyName << "] to value ["
                  << value << "]!" << endl;
         }
      }
   return false;
   }

Json::Value* ConfigFile::findProperty(Json::Value& root, const string& propertyName, const char propertyNameDelimiter) const
   {
   try
      {
      Json::Value* currentProperty = &root;
      vector<string> propertyNames;
      StringUtilities::tokenizeString(propertyName, propertyNames, propertyNameDelimiter);

      if (propertyNames.size() > 0 && currentProperty->type() == Json::objectValue)
         {

         unsigned int i = 0;
         while (i < propertyNames.size())
            {
            string propertyNamePiece(propertyNames[i++]);

            if (currentProperty->type() == Json::objectValue && currentProperty->isMember(propertyNamePiece))
               {
               currentProperty = &((*currentProperty)[propertyNamePiece]);
               }
            else
               {
               return NULL;
               }
            }

         return currentProperty;
         }
      }
   catch (...)
      {
      cerr << "ConfigFile::findProperty(): failed to find property [" << propertyName << "]!" << endl;
      }

   return NULL;
   }

const bool ConfigFile::load(Json::Value& config) const
   {
   return load(config, configFilename);
   }

const bool ConfigFile::load(Json::Value& config, const string& filename) const
   {
   try
      {
      // try to load the file
      ifstream is(filename.c_str(), ios::in);
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(is, config, true);
      is.close();
      if (!parsingSuccessful)
         {
         // report to the user the failure and their locations in the document.
         cerr << "ConfigFile::loadFile(): failed to load or parse configuration file [" << filename << "]:" << endl
                  << reader.getFormatedErrorMessages();
         }

      return parsingSuccessful;
      }
   catch (...)
      {
      cerr << "ConfigFile::loadFile(): failed to load file [" << filename << "]" << endl;
      }
   return false;
   }

const bool ConfigFile::save(Json::Value& config) const
   {
   try
      {
      // open the file
      ofstream outfile(configFilename.c_str());

      // write to the file
      Json::StyledWriter writer;
      outfile << writer.write(config);

      outfile.close();

      return true;
      }
   catch (...)
      {
      cerr << "ConfigFile::save(): failed to save file [" << configFilename << "]" << endl;
      }
   return false;
   }
