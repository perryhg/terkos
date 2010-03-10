#include "qemotortraj.h"

int main(int argc, char **argv)
{
  const int axis = 0;

  // get motor reference
  CQEMotorTraj &motor = CQEMotorTraj::GetRef();

  // set PID gains for axis
  motor.SetPIDVGains(axis, 100, 0, 500, 0);

  printf("running\n");

  // move back and forth
  while(1)
    {
      motor.Move(axis, 40000, 20000, 8000);
      while(!motor.Done(axis));
      motor.Move(axis, -40000, 20000, 8000);
      while(!motor.Done(axis));
    }
}
