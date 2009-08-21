/*
 * Chris Bartley (bartley@cmu.edu)
 */

#ifndef WEBCONTROLPANELPASSWORDMANAGER_H_
#define WEBCONTROLPANELPASSWORDMANAGER_H_

#include <string>
#include <iostream>
#include <pstream.h>

using namespace std;
using namespace redi;

class WebControlPanelPasswordManager
   {
   public:

      /**
       * Resets the web control panel password to its default value by reverting /etc/httpd.conf to its default.
       * Note that the web server is not restarted, so changes won't take effect until it is restarted.
       */
      static const bool revertToDefault();

      static const bool restartWebServer();

   private:

      static const string HTTPD_CONF_PATH;
      static const string DEFAULT_HTTPD_CONF_PATH;

      WebControlPanelPasswordManager()
         {
         // nothing to do
         }

      virtual ~WebControlPanelPasswordManager()
         {
         // nothing to do
         }

   };

#endif /* WEBCONTROLPANELPASSWORDMANAGER_H_ */
