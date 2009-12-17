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
