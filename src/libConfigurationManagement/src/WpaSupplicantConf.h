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

#ifndef WPASUPPLICANTCONF_H_
#define WPASUPPLICANTCONF_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <FileUtilities.h>
#include "WirelessEncryptionType.h"

using namespace std;

class WpaSupplicantConf
   {
   public:

      static const string WPA_SUPPLICANT_CONF_PATH;
      static const string DEFAULT_WPA_SUPPLICANT_CONF_PATH;

      static void revertToDefault()
         {
         FileUtilities::copyFile(DEFAULT_WPA_SUPPLICANT_CONF_PATH, WPA_SUPPLICANT_CONF_PATH);
         }

      WpaSupplicantConf(const bool willIncludeCatchAllNetwork = true) : willIncludeCatchAllNetwork(willIncludeCatchAllNetwork)
         {
         // nothing to do
         }

      virtual ~WpaSupplicantConf()
         {
         // nothing to do
         }

      void addNetwork(const string& ssid, const unsigned int priority=0)
         {
         Network network(ssid, priority, WirelessEncryptionType::NONE);
         networks.push_back(network);
         }

      void addEncryptedNetwork(const string& ssid,
                               const WirelessEncryptionType& encryptionType,
                               const string& password,
                               const bool isHexPassword,
                               const unsigned int priority=0 )
         {
         Network network(ssid, priority, encryptionType, password, isHexPassword);
         networks.push_back(network);
         }

      void setWillIncludeCatchAllNetwork(const bool willInclude)
         {
         willIncludeCatchAllNetwork = willInclude;
         }

      const bool getWillIncludeCatchAllNetwork()
         {
         return willIncludeCatchAllNetwork;
         }

      void save();
      
   private:

      class Network
         {
         public:

            Network(const string& ssid,
                    const unsigned int priority,
                    const WirelessEncryptionType& encryptionType,
                    const string& password="",
                    const bool isHexPassword=false) :
                   ssid(ssid),
                   priority(priority),
                   encryptionType(encryptionType),
                   password(password),
                   isHexPassword(isHexPassword)
               {
               // nothing to do
               }

            virtual ~Network()
               {
               // nothing to do
               }

            const string getSsid()
               {
               return ssid;
               }

            const unsigned int getPriority()
               {
               return priority;
               }

            const bool isUsingEncryption()
               {
               return encryptionType == WirelessEncryptionType::NONE;
               }

            const WirelessEncryptionType getEncryptionType()
               {
               return encryptionType;
               }

            const string getPassword()
               {
               return password;
               }

            const bool isUsingHexPassword()
               {
               return isHexPassword;
               }

         private:

            string ssid;
            unsigned int priority;
            WirelessEncryptionType encryptionType;
            string password;
            bool isHexPassword;
         };

      bool willIncludeCatchAllNetwork;
      vector<Network> networks;

   };

#endif /* WPASUPPLICANTCONF_H_ */
