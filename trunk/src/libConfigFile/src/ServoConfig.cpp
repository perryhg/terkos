/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "ServoConfig.h"

const unsigned int ServoConfig::MIN_POSITION = 0;
const unsigned int ServoConfig::MAX_POSITION = 255;
const unsigned int ServoConfig::DEFAULT_MIN_BOUND = 10;
const unsigned int ServoConfig::DEFAULT_MAX_BOUND = 245;
const unsigned int ServoConfig::DEFAULT_INITIAL_POSITION = 127;

const string ServoConfig::CONFIG_FILENAME = "servo.cfg";
const string ServoConfig::DEFAULT_CONFIG_FILENAME = "servo.default.cfg";
const string ServoConfig::BOUNDS_MINIMUMS_PROPERTY = "bounds.minimums";
const string ServoConfig::BOUNDS_MAXIMUMS_PROPERTY = "bounds.maximums";
const string ServoConfig::INITIAL_POSITIONS_PROPERTY = "initial-positions";

const unsigned int ServoConfig::getMinBound(const unsigned int servoId) const
   {
   return getIndexedUnsignedIntValue(BOUNDS_MINIMUMS_PROPERTY, servoId, DEFAULT_MIN_BOUND);
   }

const unsigned int ServoConfig::getMaxBound(const unsigned int servoId) const
   {
   return getIndexedUnsignedIntValue(BOUNDS_MAXIMUMS_PROPERTY, servoId, DEFAULT_MAX_BOUND);
   }

const unsigned int ServoConfig::getInitialPosition(const unsigned int servoId) const
   {
   return getIndexedUnsignedIntValue(INITIAL_POSITIONS_PROPERTY, servoId, DEFAULT_INITIAL_POSITION);
   }

bool ServoConfig::setMinBound(const unsigned int servoId, const unsigned int position)
   {
   return setIndexedUnsignedIntValue(BOUNDS_MINIMUMS_PROPERTY, servoId, std::max(position, MIN_POSITION));
   }

bool ServoConfig::setMaxBound(const unsigned int servoId, const unsigned int position)
   {
   return setIndexedUnsignedIntValue(BOUNDS_MAXIMUMS_PROPERTY, servoId, std::min(position, MAX_POSITION));
   }

bool ServoConfig::setInitialPosition(const unsigned int servoId, const unsigned int position)
   {
   return setIndexedUnsignedIntValue(INITIAL_POSITIONS_PROPERTY, servoId, std::min(std::max(position, MIN_POSITION), MAX_POSITION));
   }
