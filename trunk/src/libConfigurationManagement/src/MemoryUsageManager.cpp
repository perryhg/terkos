/*
 * Chris Bartley (bartley@cmu.edu)
 */

#include "MemoryUsageManager.h"

Json::Value MemoryUsageManager::getJSON()
   {
   try
      {
      // execute the script and return the results as a stream
      redi::ipstream is("perl -I/opt/scripts /opt/scripts/getMemoryUsageAsJSON.pl");

      // parse the stream
      Json::Reader reader;
      Json::Value json;
      bool parsingSuccessful = reader.parse(is, json, true);
      is.close();

      if (parsingSuccessful)
         {
         return json;
         }
      else
         {
         cerr << "MemoryUsageManager(): failed to parse memory usage JSON:" << endl << reader.getFormatedErrorMessages();
         }
      }
   catch (...)
      {
      cerr << "MemoryUsageManager(): failed to parse memory usage JSON" << endl;
      }

   return Json::Value::null;
   }
