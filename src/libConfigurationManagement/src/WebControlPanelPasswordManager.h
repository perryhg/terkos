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
