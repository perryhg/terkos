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
