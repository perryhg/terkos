/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "StringUtilities.h"

const string StringUtilities::convertIntToString(const int val)
   {
   std::stringstream out;
   out << val;
   return out.str();
   }
