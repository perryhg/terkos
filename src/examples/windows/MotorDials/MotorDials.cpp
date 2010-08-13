/**
 * Daragh Egan
 * 8/13/10
 * MotorDials.cpp
 *
 * Control a servo using two potentiometers
 * One controls the position and the other controls sensitivity.
 *
 * To optimize sensitivity, turn the position up to maximum
 * and then slowly turn the sensitivity up until the position
 * is as close as possible to 250. (Usually around 0.050600)
 */

#include <qeanalog.h>
#include <qeservo.h>

int main(void)
{
	CQEServo &servo = CQEServo::GetRef();
	CQEAnalog &analog = CQEAnalog::GetRef();

	short int pos = 0;	// Position
	float sens = 0;		// Sensitivity

	while(1)
	{
		// Get sensitivity from potentiometer 1
		sens = analog.GetADVoltage(1) / 15000.0;

		// Get position from potentiometer 0
		// Use sensitivity to modify position
		pos = int(analog.GetADVoltage(0) * sens);

		// Set position of servo
		servo.SetCommand(0, pos);

		// Print sensitivity and position
		printf("%f\t%d\n", sens, pos);
	}

	return 0;
}
