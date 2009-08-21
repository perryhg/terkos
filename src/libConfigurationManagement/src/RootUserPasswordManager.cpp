/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "RootUserPasswordManager.h"

const bool RootUserPasswordManager::revertToDefault()
   {
   Json::Value response;
   try
      {
      // call the Perl script which resets the root user password
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/resetRootUserPassword.pl");

      // parse the stream
      Json::Reader reader;
      bool parsingSuccessful = reader.parse(is, response, true);

      if (!parsingSuccessful)
         {
         // report to the user the failure and their locations in the document.
         cerr << "RootUserPasswordManager::revertToDefault(): failed to parse JSON response:" << endl
                  << reader.getFormatedErrorMessages();
         response = Json::Value::null;
         }
      }
   catch (...)
      {
      cerr << "RootUserPasswordManager::revertToDefault(): failed to parse JSON response" << endl;
      response = Json::Value::null;
      }

   if (response != Json::Value::null)
      {
      Json::Value okStatus = response["ok"];
      if (okStatus != Json::Value::null)
         {
         return okStatus.asBool();
         }
      }

   return false;
   }
