/*
 * Simple program enabling access to the SystemInfoManager so that
 * the system info can be read via the command line.  The system info
 * is printed in JSON on stdout.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <SystemInfoManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   SystemInfoManager manager;

   cout << manager.getJSON() << endl;

   return 0;
   }
