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

#ifndef WIRELESSENCRYPTIONTYPE_H_
#define WIRELESSENCRYPTIONTYPE_H_

#include <string>

using namespace std;

class WirelessEncryptionType
   {

   public:
      static const WirelessEncryptionType NONE;
      static const WirelessEncryptionType WEP;
      static const WirelessEncryptionType WPA;
      static const WirelessEncryptionType UNKNOWN;

      static const WirelessEncryptionType findByName(const string& name)
         {
         // if we had more, then a hashmap would be better, but if-else is fine for now...
         if (name.compare(NONE_) == 0)
            {
            return NONE;
            }
         else if (name.compare(WEP_) == 0)
            {
            return WEP;
            }
         else if (name.compare(WPA_) == 0)
            {
            return WPA;
            }
         else
            {
            return UNKNOWN;
            }
         }

      const string getName() const
         {
         return name;
         }

      bool operator== (const WirelessEncryptionType& rhs) const
         {
         return name.compare(rhs.getName()) == 0;
         }

      bool operator!= (const WirelessEncryptionType& rhs) const
         {
         return name.compare(rhs.getName()) != 0;
         }

      const WirelessEncryptionType& operator=(const WirelessEncryptionType& rhs)
         {
         if (this == &rhs)
            {
            return *this;
            }
         else if (&rhs == &NONE)
            {
            return NONE;
            }
         else if (&rhs == &WEP)
            {
            return WEP;
            }
         else if (&rhs == &WPA)
            {
            return WPA;
            }

         return UNKNOWN;
         }

   private:

      static const string NONE_;
      static const string WEP_;
      static const string WPA_;
      static const string UNKNOWN_;

      const string name;

      WirelessEncryptionType(const string& name) : name(name)
         {
         // nothing to do
         }

   };

#endif /* WIRELESSENCRYPTIONTYPE_H_ */
