/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ServoConfigManager.h"

const unsigned int ServoConfigManager::NUM_SERVOS = QES_DEFAULT_SERVOS;
const unsigned int ServoConfigManager::MIN_POSITION = 0;
const unsigned int ServoConfigManager::MAX_POSITION = 255;
const unsigned int ServoConfigManager::DEFAULT_MIN_BOUND = 10;
const unsigned int ServoConfigManager::DEFAULT_MAX_BOUND = 245;
const unsigned int ServoConfigManager::DEFAULT_INITIAL_POSITION = 127;

const string ServoConfigManager::CONFIG_FILENAME = "servo_config.json";
const string ServoConfigManager::DEFAULT_CONFIG_FILENAME = "servo_config.default.json";
const string ServoConfigManager::BOUNDS_MINIMUMS_PROPERTY = "servos.bounds.minimums";
const string ServoConfigManager::BOUNDS_MAXIMUMS_PROPERTY = "servos.bounds.maximums";
const string ServoConfigManager::INITIAL_POSITIONS_PROPERTY = "servos.initial-positions";

// TODO: add error logging to these methods once we have a real logging framework

const unsigned int ServoConfigManager::getMinBound(const unsigned int servoId) const
   {
   if (servoId < NUM_SERVOS)
      {
      return getIndexedUnsignedIntValue(BOUNDS_MINIMUMS_PROPERTY, servoId, DEFAULT_MIN_BOUND);
      }

   cout << "ServoConfigManager::getMinBound(): Invalid servo id [" << servoId << "], returning default instead" << endl;
   return DEFAULT_MIN_BOUND;
   }

const unsigned int ServoConfigManager::getMaxBound(const unsigned int servoId) const
   {
   if (servoId < NUM_SERVOS)
      {
      return getIndexedUnsignedIntValue(BOUNDS_MAXIMUMS_PROPERTY, servoId, DEFAULT_MAX_BOUND);
      }

   cout << "ServoConfigManager::getMaxBound(): Invalid servo id [" << servoId << "], returning default instead" << endl;
   return DEFAULT_MAX_BOUND;
   }

const unsigned int ServoConfigManager::getInitialPosition(const unsigned int servoId) const
   {
   if (servoId < NUM_SERVOS)
      {
      return getIndexedUnsignedIntValue(INITIAL_POSITIONS_PROPERTY, servoId, DEFAULT_INITIAL_POSITION);
      }

   cout << "ServoConfigManager::getInitialPosition(): Invalid servo id [" << servoId << "], returning default instead" << endl;
   return DEFAULT_INITIAL_POSITION;
   }

bool ServoConfigManager::setMinBound(const unsigned int servoId, const unsigned int position)
   {
   if (servoId < NUM_SERVOS)
      {
      return setIndexedUnsignedIntValue(BOUNDS_MINIMUMS_PROPERTY, servoId, std::max(position, MIN_POSITION));
      }

   cout << "ServoConfigManager::setMinBound(): Command ignored due to invalid servo id [" << servoId << "]" << endl;
   return false;
   }

bool ServoConfigManager::setMaxBound(const unsigned int servoId, const unsigned int position)
   {
   if (servoId < NUM_SERVOS)
      {
      return setIndexedUnsignedIntValue(BOUNDS_MAXIMUMS_PROPERTY, servoId, std::min(position, MAX_POSITION));
      }

   cout << "ServoConfigManager::setMaxBound(): Command ignored due to invalid servo id [" << servoId << "]" << endl;
   return false;
   }

bool ServoConfigManager::setInitialPosition(const unsigned int servoId, const unsigned int position)
   {
   if (servoId < NUM_SERVOS)
      {
      return setIndexedUnsignedIntValue(INITIAL_POSITIONS_PROPERTY, servoId, std::min(std::max(position, MIN_POSITION), MAX_POSITION));
      }

   cout << "ServoConfigManager::setInitialPosition(): Command ignored due to invalid servo id [" << servoId << "]" << endl;
   return false;
   }
