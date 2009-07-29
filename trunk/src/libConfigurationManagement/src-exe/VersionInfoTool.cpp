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
