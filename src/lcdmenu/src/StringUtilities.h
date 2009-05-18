/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef STRINGUTILITIES_H_
#define STRINGUTILITIES_H_

#include <sstream>

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
