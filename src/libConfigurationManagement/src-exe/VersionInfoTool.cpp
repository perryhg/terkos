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
 * Simple program enabling access to the VersionInfoManager so that
 * the version info can be read via the command line.  The version info
 * is printed in JSON on stdout.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <VersionInfoManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   VersionInfoManager manager;

   cout << manager.getJSON() << endl;

   return 0;
   }
