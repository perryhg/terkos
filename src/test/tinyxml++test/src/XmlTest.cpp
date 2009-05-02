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

