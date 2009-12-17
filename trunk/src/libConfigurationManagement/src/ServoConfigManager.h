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

#ifndef SERVOCONFIGMANAGER_H_
#define SERVOCONFIGMANAGER_H_

#include <algorithm>
#include "ConfigFile.h"
#include "qeservo.h"

using namespace std;

class ServoConfigManager : public ConfigFile
   {
   public:

      static const unsigned int NUM_SERVOS;
      static const unsigned int MIN_POSITION;
      static const unsigned int MAX_POSITION;
      static const unsigned int DEFAULT_MIN_BOUND;
      static const unsigned int DEFAULT_MAX_BOUND;
      static const unsigned int DEFAULT_INITIAL_POSITION;

      ServoConfigManager(const string& configFileDirectory = ConfigFile::DEFAULT_CONFIG_FILE_DIRECTORY) :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME, configFileDirectory)
         {
         // nothing to do
         }

      virtual ~ServoConfigManager()
         {
         // nothing to do
         }

      const unsigned int getMinBound(const unsigned int servoId) const;
      const unsigned int getMaxBound(const unsigned int servoId) const;
      const unsigned int getInitialPosition(const unsigned int servoId) const;

      bool setMinBound(const unsigned int servoId, const unsigned int position);
      bool setMaxBound(const unsigned int servoId, const unsigned int position);
      bool setInitialPosition(const unsigned int servoId, const unsigned int position);

   private:

      static const string CONFIG_FILENAME;
      static const string DEFAULT_CONFIG_FILENAME;
      static const string BOUNDS_MINIMUMS_PROPERTY;
      static const string BOUNDS_MAXIMUMS_PROPERTY;
      static const string INITIAL_POSITIONS_PROPERTY;

   };

#endif /* SERVOCONFIGMANAGER_H_ */
