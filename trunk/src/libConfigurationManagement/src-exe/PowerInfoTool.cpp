/*
 * Simple program enabling access to the PowerInfoManager so that
 * the power info can be read via the command line.  The power info
 * is printed in JSON on stdout.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <PowerInfoManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   PowerInfoManager manager;

   cout << manager.getJSON() << endl;

   return 0;
   }
