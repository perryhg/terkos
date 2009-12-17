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

#include <qepower.h>

using namespace std;

const unsigned int NUM_BREAKERS = 5;

int getHardwareProperty(const int property, CQEPower &power)
   {
   long val = -1;

   try
      {
      if (power.GetProperty(property, &val) != PROP_OK)
         {
         cerr << "getHardwareProperty(): failed to get property [" << property << "]." << endl;
         }
      }
   catch (...)
      {
      cerr << "getHardwareProperty(): failed to get CQEPower reference required to get property [" << property << "]." << endl;
      }

   return (int)val;
   }

const bool getBreakerState(const unsigned int breakerId, CQEPower &power)
   {
   if (breakerId < NUM_BREAKERS)
      {
      const int breakersState = getHardwareProperty(QP_PROP_BREAKER_STATE, power);
      if (breakersState >= 0)
         {
         return (breakersState >> breakerId & 1) == 1;
         }
      }
   return false;
   }

int main(int argc, char** argv)
   {
   CQEPower &power = CQEPower::GetRef();
   cout << "QP_PROP_MAIN_BATT_VOLTAGE = [" << getHardwareProperty(QP_PROP_MAIN_BATT_VOLTAGE, power) << "]" << endl;
   cout << "QP_PROP_BACKUP_BATT_VOLTAGE = [" << getHardwareProperty(QP_PROP_BACKUP_BATT_VOLTAGE, power) << "]" << endl;
   cout << "QP_PROP_IO_5V_VOLTAGE = [" << getHardwareProperty(QP_PROP_IO_5V_VOLTAGE, power) << "]" << endl;
   cout << "QP_PROP_MAIN_5V_VOLTAGE = [" << getHardwareProperty(QP_PROP_MAIN_5V_VOLTAGE, power) << "]" << endl;
   cout << "QP_PROP_MAIN_BATT_STATE = [" << getHardwareProperty(QP_PROP_MAIN_BATT_STATE, power) << "]" << endl;
   cout << "QP_PROP_BACKUP_BATT_STATE = [" << getHardwareProperty(QP_PROP_BACKUP_BATT_STATE, power) << "]" << endl;
   cout << "QP_PROP_IO_5V_STATE = [" << getHardwareProperty(QP_PROP_IO_5V_STATE, power) << "]" << endl;

   for (unsigned int i=0; i<NUM_BREAKERS; i++)
      {
      cout << "Breaker " << i << " = [" << getBreakerState(i, power) << "]" << endl;
      }
   CQEPower::Release();

   return 0;
   }

