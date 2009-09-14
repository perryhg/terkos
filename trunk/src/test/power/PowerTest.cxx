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

