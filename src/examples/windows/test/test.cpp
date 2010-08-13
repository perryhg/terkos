/**
 * Daragh Egan
 * 8/13/10
 * test.cpp
 * A testing application for the VexPro.
 *
 * This is a series of functions used for
 * testing various sensors and motors.
 * Modify the main method to call whichever
 * function you wish to use.
 */

#include <stdio.h>
#include <math.h>	// For sine function
#include <unistd.h>	// For sleep function
#include <qeservo.h>
#include <qemotortraj.h>
#include <qegpioint.h>
#include <qeanalog.h>

CQEGpioInt &io = CQEGpioInt::GetRef();

long ticks = 0;

void analogtest(void);
void callback(unsigned int io, struct timeval *ptv);
void servotest(void);
void motortest(void);
void digitaltest(void);
void analogtest(void);

int main(void)
{
	printf("Start!\n");

	digitaltest();

	printf("Done!\n");
	return 0;
}

void servotest(void)
{
	printf("Servo Test:");
	int axis = 0;
	// get servo reference
	CQEServo &servo = CQEServo::GetRef();

	for (int i = 0; i < 250; i += 10)
	{
		servo.SetCommand(axis, i);
		printf("Servo%d\t%d\n", axis, i);
		sleep(1);
	}
}

void motortest(void)
{
	printf("Motor Test:");
	// Get motor reference
	CQEMotorTraj &motor = CQEMotorTraj::GetRef();

	for (int axis = 0; axis < 4; axis++)
	{
		motor.SetPIDVGains(axis, 100, 0, 500, 0);

		motor.MoveVelocity(axis, 30000, 100000);
		printf("Motor%d\n", axis);
		sleep(5);
		motor.Stop(axis);
	}
}

void callback(unsigned int port, struct timeval *ptv)
{
//	printf("[%d:%d]\tSignal at port %d\n", ptv->tv_sec, ptv->tv_usec, port);
	printf("[%d]\t", port);
	for (int i = 0; i < 16; i++)
	{
		printf("%d ", ( (io.GetData() & (1<<i) ) == (1<<i)) );
	}
	printf("\n");
	/*
	ticks++;
	float degrees = ticks * 360 / 90;
	if (degrees > 360)
		degrees -= 360;
	printf("%f degrees\n", degrees);
	*/
}

void digitaltest(void)
{
	printf("Digital Test:");
	io.SetDataDirection(1); // Set for input
	for (int i = 0; i < 16; i++)
		io.RegisterCallback(i, callback); // Register ports

	while(1);
	while(1)
	{
		for (int i = 0; i < 16; i++)
		{
			printf("%d ", ( (io.GetData() & (1<<i) ) == (1<<i)) );
		}
		printf("\n");
	}
}

void analogtest(void)
{
	printf("Analog Test:");
	CQEAnalog &analog = CQEAnalog::GetRef();

	while(1)
	{
		for (int i  = 0; i < 16; i++)
			printf("%d\t", analog.GetADVoltage(i));
		printf("\n");
	}
}



