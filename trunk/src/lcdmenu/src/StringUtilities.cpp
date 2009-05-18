/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "StringUtilities.h"

const string StringUtilities::convertIntToString(const int val, const unsigned int desiredPaddedLength, const char paddingChar)
   {
   std::stringstream out;
   out << val;
   string s = out.str();

   if (desiredPaddedLength > s.length())
      {
      const unsigned int numPaddedChars = desiredPaddedLength - s.length();
      string padding(numPaddedChars, paddingChar);
      return padding.append(s);
      }

   return s;
   }
