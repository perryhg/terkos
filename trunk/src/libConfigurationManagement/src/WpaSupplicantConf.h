/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WPASUPPLICANTCONF_H_
#define WPASUPPLICANTCONF_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <FileUtilities.h>

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
         Network network(ssid, priority);
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

            Network(const string& ssid, const unsigned int priority=0) : ssid(ssid), priority(priority)
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

         private:

            string ssid;
            unsigned int priority;
            
         };

      bool willIncludeCatchAllNetwork;
      vector<Network> networks;

   };

#endif /* WPASUPPLICANTCONF_H_ */
