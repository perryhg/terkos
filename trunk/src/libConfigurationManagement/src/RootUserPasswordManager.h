//
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//

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
