/**
 * Daragh Egan
 * 8/13/10
 * encoder.cpp
 * A class that represents an optical shaft encoder.
 *
 * Note: A single encoder part actually contains two
 * encoders so that the direction of rotation can be
 * determined. This class only implements one encoder
 * and does not determine direction of rotation.
 */

#ifndef ENCODER_H
#define ENCODER_H
#endif

class vexEncoder
{
public:
	unsigned int port; 		// Input channel
	long time; 				// Time last data was received
	int data[2]; 			// Two most recent bits received as input
	unsigned long ticks; 	// Total number of ticks
	double speed; 			// Speed in rotations per minute
	double distance; 		// Distance traveled
	double diameter; 		// Used to calculate speed and distance

	vexEncoder(unsigned int sensor_port, double wheel_diameter)
	{
		port = sensor_port;
		diameter = wheel_diameter;
		time = 0;
		ticks = 0;
		speed = 0;
		distance = 0;;
	}

	vexEncoder()
	{
		time = 0;
		ticks = 0;
		speed = 0;
		distance = 0;
	}

	/**
	 * Refreshes all variables associated with the encoder based
	 * on the most recent I/O data and the current time.
	 * This should be invoked by the callback function and passed
	 * the timeval value.
	 */
	void update(struct timeval *tv, unsigned int iodata)
	{
		long usec = tv->tv_usec;

		// Update time (microseconds)
		long dt = usec - time;
		time = usec;

		// Update data
		unsigned int cp = data[1];
		data[1] = (iodata & (1 << port)) == (1 << port);
		data[0] = cp;

		// Update number of ticks
		ticks++;

		// Update speed (rpm)
		speed = (60.0 * 1000.0 * 1000.0) / (dt * 90.0);

		// Update distance
		// Revolutions x circumference
		distance = (ticks / 90.0) * (diameter * 3.14159265358979323);
	}
};
