/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef ROOTUSERPASSWORDMANAGER_H_
#define ROOTUSERPASSWORDMANAGER_H_

#include <string>
#include <iostream>
#include <pstream.h>
#include <json/json.h>

using namespace std;
using namespace redi;

class RootUserPasswordManager
   {
   public:

      /**
       * Resets the root user password to the default.
       */
      static const bool revertToDefault();

   private:

      RootUserPasswordManager()
         {
         // nothing to do
         }

      virtual ~RootUserPasswordManager()
         {
         // nothing to do
         }
   };

#endif /* ROOTUSERPASSWORDMANAGER_H_ */
