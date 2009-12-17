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

#include "UserProgramsConfigManager.h"

const string UserProgramsConfigManager::USER_PROGRAMS_DIRECTORY_PATH = "/opt/usr/bin/";
const string UserProgramsConfigManager::EMPTY_PROGRAM_NAME = "";

const string UserProgramsConfigManager::CONFIG_FILENAME = "user_programs_config.json";
const string UserProgramsConfigManager::DEFAULT_CONFIG_FILENAME = "user_programs_config.default.json";
const string UserProgramsConfigManager::USER_PROGRAMS_ELEMENT_NAME = "user-programs";
const string UserProgramsConfigManager::PROGRAM_TO_RUN_ON_BOOTUP_ELEMENT_NAME = "program-to-run-on-bootup";
const string UserProgramsConfigManager::PROGRAMS_ELEMENT_NAME = "programs";
const string UserProgramsConfigManager::PROGRAM_TO_RUN_ON_BOOTUP_PROPERTY = USER_PROGRAMS_ELEMENT_NAME + "." + PROGRAM_TO_RUN_ON_BOOTUP_ELEMENT_NAME;
const string UserProgramsConfigManager::PROGRAMS_PROPERTY = USER_PROGRAMS_ELEMENT_NAME + "." + PROGRAMS_ELEMENT_NAME;

const string UserProgramsConfigManager::getProgramToRunOnBoot()
   {
   string programName = getStringValue(PROGRAM_TO_RUN_ON_BOOTUP_PROPERTY);

   if (!isValidProgramName(programName))
      {
      programName = EMPTY_PROGRAM_NAME;
      setProgramToRunOnBoot(programName);
      }

   return programName;
   }

bool UserProgramsConfigManager::setProgramToRunOnBoot(const string programName)
   {
   if (isValidProgramName(programName))
      {
      return setStringValue(PROGRAM_TO_RUN_ON_BOOTUP_PROPERTY, programName);
      }
   else
      {
      return setStringValue(PROGRAM_TO_RUN_ON_BOOTUP_PROPERTY, EMPTY_PROGRAM_NAME);
      }
   }

const string UserProgramsConfigManager::getAbsolutePathToProgramToRunOnBoot()
   {
   string programName = getProgramToRunOnBoot();

   if (!programName.empty())
      {
      return USER_PROGRAMS_DIRECTORY_PATH + programName;
      }

   return EMPTY_PROGRAM_NAME;
   }

vector<string> UserProgramsConfigManager::getUserProgramNames() const
   {
   return FileUtilities::getFilenamesInDirectory(USER_PROGRAMS_DIRECTORY_PATH);
   }

Json::Value UserProgramsConfigManager::getJSON()
   {
   // get the vector of user program file names
   vector<string> programs = getUserProgramNames();
   vector<int>::size_type numPrograms = programs.size();

   // build the programsElement
   Json::Value programsElement(Json::arrayValue);
   for (unsigned int i = 0; i < numPrograms; i++)
      {
      programsElement[i] = programs[i];
      }

   // build the other elements
   Json::Value userProgramsElement;
   userProgramsElement[PROGRAM_TO_RUN_ON_BOOTUP_ELEMENT_NAME] = getProgramToRunOnBoot();
   userProgramsElement[PROGRAMS_ELEMENT_NAME] = programsElement;

   Json::Value rootElement;
   rootElement[USER_PROGRAMS_ELEMENT_NAME] = userProgramsElement;

   return rootElement;
   }

const bool UserProgramsConfigManager::setJson(Json::Value& config)
   {
   if (config != Json::Value::null)
      {
      // now do some simple tests to verify it
      Json::Value* programNameProperty = ConfigFile::findProperty(config, PROGRAM_TO_RUN_ON_BOOTUP_PROPERTY);
      if (programNameProperty != NULL)
         {
         // revert to default
         revertToDefault();

         // set the PROGRAM_TO_RUN_ON_BOOTUP_PROPERTY
         setProgramToRunOnBoot(programNameProperty->asString());

         // NOTE: we don't set the PROGRAMS_PROPERTY because we don't ever write that to disk--it's always
         // fetched on demand so that it's never stale.

         return true;
         }
      }
   return false;
   }

const bool UserProgramsConfigManager::isValidProgramName(const string& programName) const
   {
   if (programName == EMPTY_PROGRAM_NAME)
      {
      return true;
      }

   vector<string> programs = getUserProgramNames();
   vector<int>::size_type numPrograms = programs.size();

   for (unsigned int i = 0; i < numPrograms; i++)
      {
      if (programs[i] == programName)
         {
         return true;
         }
      }

   return false;
   }
