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
