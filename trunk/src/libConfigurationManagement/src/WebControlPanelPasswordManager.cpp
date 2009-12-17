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

#include "WebControlPanelPasswordManager.h"

const string WebControlPanelPasswordManager::HTTPD_CONF_PATH = "/etc/httpd.conf";
const string WebControlPanelPasswordManager::DEFAULT_HTTPD_CONF_PATH = "/etc/httpd.conf.default";

const bool WebControlPanelPasswordManager::revertToDefault()
   {
   // revert the conf file
   string copyCommand = "cp " + DEFAULT_HTTPD_CONF_PATH + " " + HTTPD_CONF_PATH;
   system(copyCommand.c_str());

   return true;
   }

const bool WebControlPanelPasswordManager::restartWebServer()
   {
   // call the Perl script which restarts the web server and return the result as a stream
   redi::ipstream is("perl -I/opt/scripts /opt/scripts/restartWebServer.pl");
   string line;
   getline(is, line);
   bool success = (line == "1");
   if (success)
      {
      cout << "WebControlPanelPasswordManager::restartWebServer(): web server restarted successfully." << endl;
      }
   else
      {
      cerr << "WebControlPanelPasswordManager::restartWebServer(): failed to restart web server." << endl;
      }

   return success;
   }
