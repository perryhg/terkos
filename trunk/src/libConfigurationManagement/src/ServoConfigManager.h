/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef SERVOCONFIGMANAGER_H_
#define SERVOCONFIGMANAGER_H_

#include <algorithm>
#include "ConfigFile.h"
#include "rcservo.h"

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

      ServoConfigManager() :
         ConfigFile(CONFIG_FILENAME, DEFAULT_CONFIG_FILENAME)
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
