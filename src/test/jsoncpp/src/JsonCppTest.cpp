#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <vector>
#include <json/json.h>

using namespace std;

void assertEquals(const char* testCase, const int expected, const int actual)
   {
   cout << "Test: " << testCase << ": ";
   if (expected == actual)
      {
      cout << "PASS";
      }
   else
      {
      cout << "FAIL (expected " << expected << ", found " << actual << ")";
      }
   cout << endl;
   }

void assertEquals(const char* testCase, const char* expected, const char* actual)
   {
   cout << "Test: " << testCase << ": ";
   if (strcmp(expected, actual) == 0)
      {
      cout << "PASS";
      }
   else
      {
      cout << "FAIL (expected " << expected << ", found " << actual << ")";
      }
   cout << endl;
   }

void assertNotNull(const char* testCase, const Json::Value val)
   {
   cout << "Test: " << testCase << ": ";
   if (val != NULL)
      {
      cout << "PASS";
      }
   else
      {
      cout << "FAIL (value is NULL)";
      }
   cout << endl;
   }

void assertTrue(const char* testCase, const bool val)
   {
   cout << "Test: " << testCase << ": ";
   if (val)
      {
      cout << "PASS";
      }
   else
      {
      cout << "FAIL (value is false)";
      }
   cout << endl;
   }

int main(int argc, char** argv)
   {
   cout << "-----------------------------------------------------------------" << endl;

   std::string filename = "test.json";
   cout << "Reading file [" << filename << "]" << endl;
   ifstream is(filename.c_str(), ios::in);
   Json::Value root; // will contain the root value after parsing.
   Json::Reader reader;
   bool parsingSuccessful = reader.parse(is, root, true);
   if (!parsingSuccessful)
      {
      // report to the user the failure and their locations in the document.
      std::cout << "Failed to parse configuration\n" << reader.getFormatedErrorMessages();
      return 1;
      }
   is.close();
   cout << "Success!" << endl;

   cout << "-----------------------------------------------------------------" << endl;

   assertEquals("foo", "bar", root["foo"].asCString());

   Json::Value boundsMinimums = root["bounds"]["minimums"];
   assertNotNull("bounds.minimums", boundsMinimums);
   if (boundsMinimums != NULL)
      {
      assertEquals("boundsMinimums.size()", 16, boundsMinimums.size());
      unsigned int expectedVal = 10;
      for (unsigned int i = 0; i < boundsMinimums.size(); i++)
         {
         stringstream ss(stringstream::in | stringstream::out);
         ss << "bounds.minimums[" << i << "]";

         assertEquals(ss.str().c_str(), expectedVal, boundsMinimums[i].asUInt());
         expectedVal++;
         }
      }

   Json::Value boundsMaximums = root["bounds"]["maximums"];
   assertNotNull("bounds.maximums", boundsMaximums);
   if (boundsMaximums != NULL)
      {
      assertEquals("boundsMaximums.size()", 16, boundsMaximums.size());
      unsigned int expectedVal = 210;
      for (unsigned int i = 0; i < boundsMaximums.size(); i++)
         {
         stringstream ss(stringstream::in | stringstream::out);
         ss << "bounds.maximums[" << i << "]";

         assertEquals(ss.str().c_str(), expectedVal, boundsMaximums[i].asUInt());
         expectedVal++;
         }
      }

   Json::Value initialPositions = root["initial-positions"];
   assertNotNull("initial-positions", initialPositions);
   if (initialPositions != NULL)
      {
      assertEquals("initialPositions.size()", 16, initialPositions.size());
      unsigned int expectedVal = 110;
      for (unsigned int i = 0; i < initialPositions.size(); i++)
         {
         stringstream ss(stringstream::in | stringstream::out);
         ss << "initial-positions[" << i << "]";

         assertEquals(ss.str().c_str(), expectedVal, initialPositions[i].asUInt());
         expectedVal++;
         }
      }

   assertEquals("volume.level", 5, root["volume"]["level"].asUInt());
   assertTrue("alerts.enabled", root["alerts"]["enabled"].asBool());
   assertTrue("bogus", root.isMember("bogus") == false);

   cout << "-----------------------------------------------------------------" << endl;

   Json::StyledWriter writer;

   // Make a new JSON document for the configuration. Preserve original comments.
   std::string outputConfig = writer.write(root);

   cout << outputConfig << endl;

   cout << "-----------------------------------------------------------------" << endl;
   return 0;
   }

