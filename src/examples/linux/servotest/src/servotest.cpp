#include <qeservo.h>
#include <unistd.h>

#define SERVO_ID                 0
#define SERVO_MIN                0
#define SERVO_MAX                250
#define SLEEP_TIME_IN_SECONDS    1

int main(int argc, char **argv)
   {
   printf("Running servotest...\n");

   // get servo reference
   CQEServo &servo = CQEServo::GetRef();

   // move back and forth
   while (1)
      {
      servo.SetCommand(SERVO_ID, SERVO_MIN);
      sleep(SLEEP_TIME_IN_SECONDS);

      servo.SetCommand(SERVO_ID, SERVO_MAX);
      sleep(SLEEP_TIME_IN_SECONDS);
      }
   }
