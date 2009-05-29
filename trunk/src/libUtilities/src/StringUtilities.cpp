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

const void StringUtilities::tokenizeString(const string& str, vector<string>& tokens, const string& delimiters)
   {
   // Skip delimiters at beginning.
   string::size_type lastPos = str.find_first_not_of(delimiters, 0);
   // Find first "non-delimiter".
   string::size_type pos = str.find_first_of(delimiters, lastPos);

   while (string::npos != pos || string::npos != lastPos)
      {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
      }
   }

const void StringUtilities::tokenizeString(const string& str, vector<string>& tokens, const char delimiter)
   {
   string delimiters(1,delimiter);
   tokenizeString(str, tokens, delimiters);
   }
