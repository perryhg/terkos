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

      static const string convertIntToString(const int val);

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
