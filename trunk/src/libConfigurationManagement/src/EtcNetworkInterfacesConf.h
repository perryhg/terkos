/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef ETCNETWORKINTERFACESCONF_H_
#define ETCNETWORKINTERFACESCONF_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <StringUtilities.h>
#include <FileUtilities.h>

using namespace std;

class EtcNetworkInterfacesConf
   {
   public:

      static const string ETC_NETWORK_INTERFACES_CONF_PATH;
      static const string DEFAULT_ETC_NETWORK_INTERFACES_CONF_PATH;

      static void revertToDefault()
         {
         FileUtilities::copyFile(DEFAULT_ETC_NETWORK_INTERFACES_CONF_PATH, ETC_NETWORK_INTERFACES_CONF_PATH);
         }

      EtcNetworkInterfacesConf(const bool willStartOnBootup = true) : willStartOnBootup(willStartOnBootup)
         {
         // nothing to do
         }

      virtual ~EtcNetworkInterfacesConf()
         {
         // nothing to do
         }

      void setWillStartOnBootup(const bool willStart)
         {
         willStartOnBootup = willStart;
         }

      const bool getWillStartOnBootup()
         {
         return willStartOnBootup;
         }

      void save();
      
   private:

      static const string BEGIN_WLAN0_AUTOSTART;
      static const string END_WLAN0_AUTOSTART;

      bool willStartOnBootup;

   };

#endif /* ETCNETWORKINTERFACESCONF_H_ */
