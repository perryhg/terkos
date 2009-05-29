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
