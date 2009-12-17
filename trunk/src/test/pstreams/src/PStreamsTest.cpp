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

#include <iostream>
#include <pstream.h>

using namespace std;
using namespace redi;

int main(int argc, char** argv)
   {
   cout << "----------------------------------------------------------------" << endl;

   std::vector<std::string> lines;
   std::string line;
   redi::ipstream in("/sbin/iwlist wlan0 scan");
   while (std::getline(in, line))
      {
      cout << "**** " << line << endl;
      }

   cout << "----------------------------------------------------------------" << endl;

   return 0;
   }
