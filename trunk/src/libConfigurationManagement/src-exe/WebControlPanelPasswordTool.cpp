/*
 * Simple program for resetting the web control panel password via the command line.
 * Note that the web server is not restarted, so changes won't take effect until it
 * is restarted.
 *
 * To reset the password, call the program with a "--reset" argument.  The program
 * will print "1" if the reset was successful and "0" otherwise.
 *
 * Chris Bartley (bartley@cmu.edu)
 */

#include <json/json.h>
#include <WebControlPanelPasswordManager.h>

using namespace std;

int main(int argc, char** argv)
   {
   if (argc > 1)
      {
      if (strcmp(argv[1], "--reset") == 0)
         {
         WebControlPanelPasswordManager configManager;

         cout << configManager.revertToDefault() << endl;
         }
      }

   return 0;
   }
