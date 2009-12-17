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

#include <iostream>
#include <ticpp.h>
#include <tinyxml.h>

using namespace std;
using namespace ticpp;

void printXml(ticpp::Element* parentElement, const string padding = "")
   {
   if (parentElement)
      {
      cout << padding << parentElement->Value() << endl;
      string newPadding = padding + "   ";
      if (!parentElement->NoChildren())
         {
         ticpp::Element* childElement = NULL;
         for (childElement = parentElement->FirstChildElement(false); childElement; childElement = childElement->NextSiblingElement(false))
            {
            printXml(childElement, newPadding);
            }
         }
      }
   }

int main(int argc, char** argv)
   {
   string filename = "menu.xml";

   try
      {
      // Load a document
      ticpp::Document doc(filename);
      doc.LoadFile();

      std::cout << "Yay, XML file loaded!" << std::endl;

      // look for the <menu> element
      ticpp::Element* rootElement = doc.FirstChildElement(false);

      // recursively print the XML element names
      printXml(rootElement);
      }
   catch (ticpp::Exception& ex)
      {
      std::cout << "Exception while reading XML:" << std::endl;
      std::cout << ex.what();
      }

   // Exit the program
   return 0;
   }

