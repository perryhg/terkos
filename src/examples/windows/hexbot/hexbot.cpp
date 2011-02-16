/**
 * Daragh Egan
 * 8/13/10
 * hexbot.cpp
 * Controls the six legged vex robot.
 */

#include <qemotortraj.h>
#include <qegpioint.h>
#include <unistd.h>	// For sleep function
#include "connections.h"

using namespace std;

const long bounce = 10000;

long rtime = 0;
long ltime = 0;

void hit(unsigned int port, struct timeval *ptv, void *userdata)
{
	//cout << "Signal at port" << port << ": " << io.GetData() << "\t" << ptv->tv_sec << "." << ptv->tv_usec << endl;
	if (port == RBUMPER && (ptv->tv_usec - rtime) > bounce)
	{
		// Leg hit on right side
		rtime = ptv->tv_usec;
	}
	else if (port == LBUMPER && (ptv->tv_usec - ltime) > bounce)
	{
		// Leg hit on left side
		ltime = ptv->tv_usec;

	}
	else
		return;
}

void walk(int axis, int velocity)
{
	motor.SetPIDVGains(axis, 100, 0, 500, 0);
	motor.MoveVelocity(axis, velocity, 100000);
}

int main(void)
{
	io.RegisterCallback(RBUMPER, NULL, hit); // Register ports
	io.RegisterCallback(LBUMPER, NULL, hit); // Register ports
	io.SetDataDirection(0);

	walk(RMOTOR, -10000);
	walk(LMOTOR, 10000);
	while (!key.KeyCancel());
	motor.Stop(RMOTOR);
	motor.Stop(LMOTOR);
	return 0;
}
