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
   string delimiters(1, delimiter);
   tokenizeString(str, tokens, delimiters);
   }

const string StringUtilities::trim(const string& str)
   {
   // Code based on http://www.codeproject.com/KB/stl/stdstringtrim.aspx

   string s = str;

   const char* whitespaceChars = "\n\t\v\f\r ";
   string::size_type pos = s.find_last_not_of(whitespaceChars);
   if (pos != string::npos)
      {
      s.erase(pos + 1);
      pos = s.find_first_not_of(whitespaceChars);
      if (pos != string::npos)
         {
         s.erase(0, pos);
         }
      }
   else
      {
      s.erase(s.begin(), s.end());
      }

   return s;
   }
