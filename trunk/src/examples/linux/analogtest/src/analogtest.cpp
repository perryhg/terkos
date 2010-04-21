#include <qeanalog.h>
#include <unistd.h>

#define NUM_ANALOG_CHANNELS      16
#define SLEEP_TIME_IN_MILLIS     1

int main(int argc, char **argv)
   {
   printf("Running analogtest...\n");

   // get analog reference
   CQEAnalog &analog = CQEAnalog::GetRef();

   // continuously read the analog inputs and write their values to the console
   while (1)
      {
      printf("Analog Voltages:");
      for (unsigned int channelId = 0; channelId < NUM_ANALOG_CHANNELS; channelId++)
         {
         printf("%5d", analog.GetADVoltage(channelId));
         }
      printf("\n");
      sleep(SLEEP_TIME_IN_MILLIS);
      }
   }
