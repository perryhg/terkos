/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef STRINGUTILITIES_H_
#define STRINGUTILITIES_H_

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class StringUtilities
   {
   public:

      /**
       * Converts the given int value to a string, optionally left-padding the created string to the given
       * desiredPaddedLength with the given paddingChar.  Padding is only performed if the desiredPaddedLength
       * is greater than the length of the string representation of the int.
       */
      static const string convertIntToString(const int val, const unsigned int desiredPaddedLength=0, const char paddingChar=' ');

      /**
       * Tokenizes the given string, breaking it wherever it finds one of the given delimiters (defaults to a space
       * if not specified) and returns the tokens in the given vector.
       *
       * Code taken from: http://www.linuxselfhelp.com/HOWTO/C++Programming-HOWTO-7.html#ss7.3
       */
      static const void tokenizeString(const string& str, vector<string>& tokens, const string& delimiters = " ");

      /**
       * Tokenizes the given string, breaking it where it finds the given delimiter (defaults to a space if not
       * specified) and returns the tokens in the given vector.
       */
      static const void tokenizeString(const string& str, vector<string>& tokens, const char delimiter = ' ');

      /**
       * Trims leading and trailing whitespace off the given string, and returns the result.  The given string is not
       * modified.
       */
      static const string trim(const string& str);

      /**
       * Pads or trims the given string and returns a copy.  If the given string is less than the desired length, it is
       * right-padded with the paddingChar.  If the given string is longer than the desired length the end of the string
       * is chopped so that the resulting string is the desired length.
       */
      static const string rightPadOrTrimRight(const string& str, unsigned int desiredLength, const char paddingChar=' ');

      /**
       * Creates a new string by concatenating str1 and str2 and either padding or trimming so that the resulting string
       * is the desired length.  If the concatenation of the two strings would be shorter than the desired length, some
       * number of paddingChars is inserted between the two strings so that the result is the desired length.  If the
       * concatenation of the two strings would be shorter than the desired length, Then the strings are simply
       * concatenated and then the right end is trimmed so that the resulting string is the desired length.
       */
      static const string middlePadOrTrimRight(const string& str1, const string& str2, unsigned int desiredLength, const char paddingChar=' ');

   private:

      StringUtilities()
         {
         // nothing to do
         }

      virtual ~StringUtilities()
         {
         // nothing to do
         }
   };

#endif /* STRINGUTILITIES_H_ */
