/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ServoConfigManager.h"

const unsigned int ServoConfigManager::MIN_POSITION = 0;
const unsigned int ServoConfigManager::MAX_POSITION = 255;
const unsigned int ServoConfigManager::DEFAULT_MIN_BOUND = 10;
const unsigned int ServoConfigManager::DEFAULT_MAX_BOUND = 245;
const unsigned int ServoConfigManager::DEFAULT_INITIAL_POSITION = 127;

const string ServoConfigManager::CONFIG_FILENAME = "servo.cfg";
const string ServoConfigManager::DEFAULT_CONFIG_FILENAME = "servo.default.cfg";
const string ServoConfigManager::BOUNDS_MINIMUMS_PROPERTY = "bounds.minimums";
const string ServoConfigManager::BOUNDS_MAXIMUMS_PROPERTY = "bounds.maximums";
const string ServoConfigManager::INITIAL_POSITIONS_PROPERTY = "initial-positions";

const unsigned int ServoConfigManager::getMinBound(const unsigned int servoId) const
   {
   return getIndexedUnsignedIntValue(BOUNDS_MINIMUMS_PROPERTY, servoId, DEFAULT_MIN_BOUND);
   }

const unsigned int ServoConfigManager::getMaxBound(const unsigned int servoId) const
   {
   return getIndexedUnsignedIntValue(BOUNDS_MAXIMUMS_PROPERTY, servoId, DEFAULT_MAX_BOUND);
   }

const unsigned int ServoConfigManager::getInitialPosition(const unsigned int servoId) const
   {
   return getIndexedUnsignedIntValue(INITIAL_POSITIONS_PROPERTY, servoId, DEFAULT_INITIAL_POSITION);
   }

bool ServoConfigManager::setMinBound(const unsigned int servoId, const unsigned int position)
   {
   return setIndexedUnsignedIntValue(BOUNDS_MINIMUMS_PROPERTY, servoId, std::max(position, MIN_POSITION));
   }

bool ServoConfigManager::setMaxBound(const unsigned int servoId, const unsigned int position)
   {
   return setIndexedUnsignedIntValue(BOUNDS_MAXIMUMS_PROPERTY, servoId, std::min(position, MAX_POSITION));
   }

bool ServoConfigManager::setInitialPosition(const unsigned int servoId, const unsigned int position)
   {
   return setIndexedUnsignedIntValue(INITIAL_POSITIONS_PROPERTY, servoId, std::min(std::max(position, MIN_POSITION), MAX_POSITION));
   }
