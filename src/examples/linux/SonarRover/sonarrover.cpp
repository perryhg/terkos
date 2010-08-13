/**
 * Daragh Egan
 * 8/11/10
 * sonarrover.cpp
 * Provides obstacle avoidance for a basic
 * Vex Pro robot using sonar.
 */

#include "connections.h"
#include "encoder.h"

using namespace std;

/*************** Movement Values ***************
 * These values affect the movement of the robot.
 * Feel free to change them.
 * My recommended values are in parenthesis.
 */
const unsigned int RANGE = 800; 	// Closest distance for obstacle (800)
const unsigned int BIAS = 5; 		// Max number of in-range hits to ignore (5)
const unsigned int VEL = 20000; 	// Default velocity 20000)
const unsigned int ACC = 100000; 	// Default acceleration (100000)
const double DIAMETER = 2.75; 		// Wheel diameter (2.75)

unsigned long sonar[16];	// Sonar ranges
bool bumper[16];			// Bumper states
int mood = 0;				// Corresponds to how much light the robot has been seeing.

// Pointers to the right and left encoders
vexEncoder ren[2];
vexEncoder len[2];

void bumperCallback(unsigned int port, struct timeval *ptv);
unsigned long diff(struct timeval *ptv0, struct timeval *ptv1);
void encoderCallback(unsigned int port, struct timeval *ptv);
void findLight();
void findPath();
void followLight();
void followSonar();
int main();
void manageBumper();
void manageSonar();
void run();
void showMood();
void sonarCallback(unsigned int port, struct timeval* tv);
void toggleSonar();
void waitForDistance(double leftdiff, double rightdiff);

/**
 * Setup various hardware devices
 * Start the thread for controlling the robot
 */
int main(void)
{
	io.SetData(0x0000);
	io.SetDataDirection((1 << ROUT) + (1 << LOUT));

	// Setup Sonar
	io.RegisterCallback(ROUT, sonarCallback);
	io.RegisterCallback(RIN, sonarCallback);
	io.RegisterCallback(LOUT, sonarCallback);
	io.RegisterCallback(LIN, sonarCallback);

	io.SetInterruptMode(ROUT, QEG_INTERRUPT_NEGEDGE);
	io.SetInterruptMode(RIN, QEG_INTERRUPT_NEGEDGE);
	io.SetInterruptMode(LOUT, QEG_INTERRUPT_NEGEDGE);
	io.SetInterruptMode(LIN, QEG_INTERRUPT_NEGEDGE);

	// Setup motors
	motor.SetPIDVGains(RMOTOR, 100, 0, 500, 0);
	motor.SetPIDVGains(LMOTOR, 100, 0, 500, 0);

	// Setup Bumpers
	io.RegisterCallback(RBUMPER, bumperCallback);
	io.RegisterCallback(LBUMPER, bumperCallback);

	// Setup Encoders
	ren[0] = vexEncoder(RENC[0], DIAMETER);
	ren[1] = vexEncoder(RENC[1], DIAMETER);
	len[0] = vexEncoder(LENC[0], DIAMETER);
	len[1] = vexEncoder(LENC[1], DIAMETER);

	io.RegisterCallback(RENC[0], encoderCallback);
	io.RegisterCallback(RENC[1], encoderCallback);
	io.RegisterCallback(LENC[0], encoderCallback);
	io.RegisterCallback(LENC[1], encoderCallback);

	run();
}

/**
 * Controls the robot's movement
 * Calls obstacle avoidance functions
 */
void run()
{
	printf("Running!\n");
	// Start by moving forward
	motor.MoveVelocity(RMOTOR, VEL, ACC);
	motor.MoveVelocity(LMOTOR, (-VEL), ACC);

	led.SetLED(LED_ROBOT, LED_COLOR_OFF);
	led.SetLED(LED_GAME, LED_COLOR_OFF);
	led.SetLED(LED_VEXNET, LED_COLOR_OFF);

	audio.SetVolume(128);

	printf("Entering loop.\n");
	// Continue until cancel button is pressed
	while (!key.KeyCancel())
	{
		toggleSonar();
		manageSonar();
		followLight();

		// Action keys
		if (key.KeyRight())
			findPath();
		if (key.KeyLeft())
			findLight();
	}
}


/**
 * Reads the sonar value and updates the global variable.
 */
void sonarCallback(unsigned int port, struct timeval *ptv)
{
	static struct timeval rtv, ltv;
	static int rflag = 0, lflag = 0;
	unsigned int val;

	// Output is sent out
	if (port == ROUT)
	{
		rflag = 1;
		rtv = *ptv;
	}

	if (port == LOUT)
	{
		lflag = 1;
		ltv = *ptv;
	}

	// Input is received
	if (port == RIN && rflag)
	{
		val = diff(&rtv, ptv);
		sonar[RIN] = val;
	}

	if (port == LIN && lflag)
	{
		val = diff(&ltv, ptv);
		sonar[LIN] = val;
	}
}

/**
 * Toggles the state of the bumper that was hit.
 */
void bumperCallback(unsigned int port, struct timeval *ptv)
{
	bool bit = (io.GetData() & (1 << port)) == (1 << port);
	if (bit)
		bumper[port] = 1;
}

/**
 * Callback function for the encoders.
 * Updates the corresponding encoder object based on the port.
 */
void encoderCallback(unsigned int port, struct timeval *ptv)
{
	if (port == RENC[0])
		ren[0].update(ptv, io.GetData());

	if (port == RENC[1])
		ren[1].update(ptv, io.GetData());

	if (port == LENC[0])
		len[0].update(ptv, io.GetData());

	if (port == LENC[1])
		len[1].update(ptv, io.GetData());
}

/**
 * Toggles the output channel to send continuous sonar pulses.
 */
void toggleSonar()
{
	volatile unsigned int d;
	io.SetData((1 << ROUT) + (1 << LOUT));
	for (d = 0; d < 120000; d++);
	io.SetData(0x0000);
	for (d = 0; d < 120000; d++);
}

/**
 * Responds to ranges read from sonar and moves the motors accordingly.
 * If any obstacle is in range, strop right away and then back up.
 * Turn away from whichever obstacle is closer (left or right).
 * Continue moving forward.
 */
void manageSonar()
{
	// Counts consecutive hits in range
	static unsigned int rcount = 0;
	static unsigned int lcount = 0;
	double rrange, lrange;

	if (sonar[RIN] < RANGE)
		rcount++;
	else
		rcount = 0;

	if (sonar[LIN] < RANGE)
		lcount++;
	else
		lcount = 0;

	// An obstacle is in range
	if (rcount > BIAS || lcount > BIAS)
	{ // Back up
		rrange = sonar[RIN];
		lrange = sonar[LIN];
		motor.Stop(RMOTOR);
		motor.Stop(LMOTOR);
		motor.MoveVelocity(RMOTOR, -VEL, ACC * 2);
		motor.MoveVelocity(LMOTOR, -(-VEL), ACC * 2);
		waitForDistance(3, 3);

		if (rcount > lcount) // Object on LEFT
		{ // Turn RIGHT
			motor.MoveVelocity(RMOTOR, VEL, ACC);
			motor.MoveVelocity(LMOTOR, -(-VEL), ACC);
		}
		else if (lcount > rcount) // Object on RIGHT
		{ // Turn LEFT
			motor.MoveVelocity(RMOTOR, -VEL, ACC);
			motor.MoveVelocity(LMOTOR, (-VEL), ACC);
		}

		// Continue turning until each wheel has moved a certain distance.
		// The distance is defined as a function based on how close the
		// obstacle was (lrange, rrange) and how far away the robot is
		// comfortable with obstacles. (RANGE)
		//
		// Result: the closer an obstacle is, the further
		// the robot will move away from it.

		if (lrange <= 0)
			lrange = RANGE;
		if (rrange <= 0)
			rrange = RANGE;
		waitForDistance((2.0 / lrange) * RANGE, (2.0 / rrange) * RANGE);
		// Begin moving straight again.
		// Reset count for next series of obstacles.
		rcount = 0;
		lcount = 0;
	}
}

/**
 * Responds to bumper hits by moving the robot away from the obstacle.
 */
void manageBumper()
{
	if (bumper[RBUMPER] || bumper[LBUMPER])
	{
		motor.Stop(RMOTOR);
		motor.Stop(LMOTOR);
		motor.MoveVelocity(RMOTOR, -VEL, ACC * 2);
		motor.MoveVelocity(LMOTOR, -(-VEL), ACC * 2);
		waitForDistance(3, 3);

		if (!bumper[RBUMPER] && bumper[LBUMPER]) // LEFT hit
		{
			motor.MoveVelocity(RMOTOR, -VEL, ACC);
			motor.MoveVelocity(LMOTOR, (-VEL), ACC);
		}
		else if (bumper[RBUMPER] && !bumper[LBUMPER]) // RIGHT hit
		{
			motor.MoveVelocity(RMOTOR, VEL, ACC);
			motor.MoveVelocity(LMOTOR, -(-VEL), ACC);
		}

		waitForDistance(4.0, 4.0);
		// Begin moving straight again.
		motor.MoveVelocity(RMOTOR, VEL, ACC);
		motor.MoveVelocity(LMOTOR, (-VEL), ACC);
		bumper[RBUMPER] = 0;
		bumper[LBUMPER] = 0;
	}
}

/**
 * Seeks or avoids light.
 */
void followLight()
{
	static unsigned int lo = 3000;
	static unsigned int hi, avg, total;
	static unsigned long sum;
	unsigned short int right = analog.GetADVoltage(RPHOTO);
	unsigned short int left = analog.GetADVoltage(LPHOTO);
	unsigned short int rvel, lvel;

	sum += right + left;
	total++;
	avg = sum / total;

	if (right > hi)
		hi = right;
	if (left > hi)
		hi = left;

	if (right < lo)
		lo = right;
	if (left < lo)
		lo = left;

	rvel = VEL / analog.GetADVoltage(LPHOTO) * 100;
	lvel = VEL / analog.GetADVoltage(RPHOTO) * 100;

	// Happy level
	if (((right + left) / 2.0 / avg) < 0.25)
	{
		mood++;
		led.SetLED(LED_ROBOT, LED_COLOR_GREEN);
	}
	else if (((right + left) / 2.0 / avg) > 0.75)
	{
		led.SetLED(LED_ROBOT, LED_COLOR_RED);
		mood--;
		if (mood < -100)
		{
			findLight();
			mood = 0;
		}
	}
	else
		led.SetLED(LED_ROBOT, LED_COLOR_OFF);

	motor.MoveVelocity(RMOTOR, rvel, ACC);
	motor.MoveVelocity(LMOTOR, (-lvel), ACC);
}

/**
 * Tend to move in the direction with the highest sonar values.
 */
void followSonar()
{
	static unsigned int lo = 3000;
	static unsigned int hi, avg, total;
	static unsigned long sum;
	unsigned short int right = sonar[RIN];
	unsigned short int left = sonar[LIN];
	unsigned short int rvel, lvel;

	sum += right + left;
	total++;
	avg = sum / total;

	if (right > hi)
		hi = right;
	if (left > hi)
		hi = left;

	if (right < lo)
		lo = right;
	if (left < lo)
		lo = left;

	rvel = VEL * right / RANGE;
	lvel = VEL * left / RANGE;

	motor.MoveVelocity(RMOTOR, rvel, ACC);
	motor.MoveVelocity(LMOTOR, (-lvel), ACC);
}

/**
 * Locates the direction with the farthest obstacle
 * Begin reading distance
 * Rotate 360 degrees
 * Remember farthest distance
 * Turn toward farthest distance
 */
void findPath()
{
	double max = 0, avg = 0, angle = 0;
	double temp = ren[0].distance;

	motor.MoveVelocity(RMOTOR, -VEL, ACC);
	motor.MoveVelocity(LMOTOR, (-VEL), ACC);

	while ((ren[0].distance - temp) < (ren[0].diameter * 3.1415926535 * 2))
	{
		toggleSonar();
		avg = (sonar[RIN] + sonar[LIN]) / 2.0;
		printf("%lf\n", avg);
		if (avg > max)
		{
			led.SetLED(LED_VEXNET, LED_COLOR_YELLOW);
			max = avg;
			angle = (ren[0].diameter * 3.1415926535 * 2) - (ren[0].distance - temp);
			led.SetLED(LED_VEXNET, LED_COLOR_OFF);
		}
	}

	motor.Stop(RMOTOR);
	motor.Stop(LMOTOR);

	for (int i = 0; i < 1000000; i++);

	motor.MoveVelocity(RMOTOR, -VEL, ACC);
	motor.MoveVelocity(LMOTOR, (-VEL), ACC);

	temp = ren[0].distance;
	while ((ren[0].distance - temp) < angle);

	motor.Stop(RMOTOR);
	motor.Stop(LMOTOR);
}

/**
 * Locates the direction with the brightest light
 * Begin reading light levels
 * Rotate 360 degrees
 * Remember highest light level
 * Turn toward highest light level
 */
void findLight()
{
	led.SetLED(LED_GAME, LED_COLOR_RED);
	double max = 65535, avg = 0, angle = 0;
	double temp = ren[0].distance;

	motor.MoveVelocity(RMOTOR, -VEL, ACC);
	motor.MoveVelocity(LMOTOR, (-VEL), ACC);

	while ((ren[0].distance - temp) < (ren[0].diameter * 3.1415926535 * 2))
	{
		avg = (analog.GetADVoltage(RPHOTO) + analog.GetADVoltage(LPHOTO)) / 2.0;
		printf("%lf\n", avg);
		if (avg < max)
		{
			max = avg;
			angle = (ren[0].diameter * 3.1415926535 * 2) - (ren[0].distance - temp);
		}
	}

	motor.Stop(RMOTOR);
	motor.Stop(LMOTOR);

	for (int i = 0; i < 1000000; i++);

	motor.MoveVelocity(RMOTOR, -VEL, ACC);
	motor.MoveVelocity(LMOTOR, (-VEL), ACC);

	waitForDistance(angle, angle);

	motor.Stop(RMOTOR);
	motor.Stop(LMOTOR);
	led.SetLED(LED_GAME, LED_COLOR_OFF);
}

/**
 * Expresses the mood of the robot using tones and colored LEDs.
 */
void showMood()
{
	if (mood > 100)
	{
		led.SetLED(LED_ROBOT, LED_COLOR_GREEN);
		audio.PlayTone(440, 500);
	}
	else if (mood < -100)
	{
		led.SetLED(LED_ROBOT, LED_COLOR_RED);
		audio.PlayTone(220, 500);
	}
	else
	{
		led.SetLED(LED_ROBOT, LED_COLOR_OFF);
	}
	sleep(1);
}


/**
 * Waits until the wheels on each side have traveled
 * the specified distances. The distance is measured from the
 * robots position at the start of this function call.
 */
void waitForDistance(double leftdiff, double rightdiff)
{
	led.SetLED(LED_GAME, LED_COLOR_RED);
	double rtemp = ren[0].distance;
	double ltemp = len[0].distance;
	while ((ren[0].distance - rtemp) < rightdiff || (len[0].distance - ltemp) < leftdiff)
	led.SetLED(LED_GAME, LED_COLOR_OFF);
}

/**
 * Returns the difference in milliseconds between two timevals.
 */
unsigned long diff(struct timeval *ptv0, struct timeval *ptv1)
{
	long val;

	val = ptv1->tv_usec - ptv0->tv_usec;
	val += (ptv1->tv_sec - ptv0->tv_sec) * 1000000;

	return val;
}
