/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef USERPROGRAMSCONFIGMANAGER_H_
#define USERPROGRAMSCONFIGMANAGER_H_

#include <algorithm>
#include <string>
#include <vector>
#include <FileUtilities.h>
#include <json/value.h>
#include "ConfigFile.h"

using namespace std;

class UserProgramsConfigManager : public ConfigFile
   {
      static const string USER_PROGRAMS_DIRECTORY_PATH;
      static const string EMPTY_PROGRAM_NAME;

   public:

      UserProgramsConfigManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~UserProgramsConfigManager()
         {
         // nothing to do
         }

      /**
       * Returns the program to run on boot.  Note that this method will compare the value
       * in the config file with the list of programs actually existing in the user program
       * directory to make sure it's a valid program before returning.  If the name is not
       * valid, the config file is corrected (setting the program name to EMPTY_PROGRAM_NAME)
       * and EMPTY_PROGRAM_NAME is returned.
       */
      const string getProgramToRunOnBoot();

      /**
       * Sets the name of the program to run on boot.  Note that this method verifies that
       * the given programName actually specifies a program existing in the user program
       * directory.  If the program is valid, the name is saved to the config file and the
       * result of the save operation is returned.  If the program is not valid, the
       * EMPTY_PROGRAM_NAME is saved to the config file and the result of the save operation
       * is returned.
       */
      bool setProgramToRunOnBoot(const string programName);

      /**
       * Returns a vector containing the names of the programs in the user programs directory.
       */
      vector<string> getUserProgramNames() const;

      Json::Value getJSON();

      const bool setJson(Json::Value& config);

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string PROGRAM_TO_RUN_ON_BOOTUP_PROPERTY;
      static const string PROGRAMS_PROPERTY;
      static const string USER_PROGRAMS_ELEMENT_NAME;
      static const string PROGRAM_TO_RUN_ON_BOOTUP_ELEMENT_NAME;
      static const string PROGRAMS_ELEMENT_NAME;

      /**
       * Returns true if the given programName is EMPTY_PROGRAM_NAME or if it matches
       * one of the programs in the user program directory.
       */
      const bool isValidProgramName(const string& programName) const;

   };

#endif /* USERPROGRAMSCONFIGMANAGER_H_ */
