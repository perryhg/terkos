//
// begin license header
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
// end license header
//

/**
 * @file qequadencoder.cxx
 * Implementation of the CQEQuadEncoder class, which provides support for
 * four quadrature encoders on digital I/O ports 1-8.
 */

#include "qequadencoder.h"

bool CQEQuadEncoder::Configure( unsigned int axis,
								unsigned long int scale,
								unsigned long int resolution,
								bool invert,
								CQEGpioInt *pio)
{
	if ( (axis >= QENC_AXIS_COUNT) || (resolution==0) )
		return false;

	ScaleUnits[axis] = scale;
	Resolution[axis] = resolution;
	Invert[axis] = invert;

	// Setup our GPIO pins as inputs if the caller wants us to
	if (pio && scale) {
		unsigned int ddr = pio->GetDataDirection();
		ddr &= ~(0x0003 << (axis < 1)); // Clear the two ddr bits for this axis to make them inputs
		pio->SetDataDirection(ddr);
	}

	// Initialize position and velocity ivars
	Count[axis] = 0;
	Offset[axis] = 0;
	Position(axis);
	Position(axis);
	Zero(axis);

	// This class makes use of the overflow semantics of two's-complement integer
	// math.  In order for it to work as expected, the int types must be specific
	// sizes, as enforced by the following init-time checks:

	if ( sizeof(short int) != 2 ) // Size of short int must be 2.
		return false;

	if ( sizeof(long int) != 4 ) // Size of long int must be 4.
		return false;

	if ( sizeof(long long int) != 8 ) // Size of long long int must be 8.
		return false;

	return true;
}


void CQEQuadEncoder::Zero(unsigned int axis)
{
	if (axis >= QENC_AXIS_COUNT)
		return;

	// We actually can't reset the encoder, since the register is read-only.
	// Instead, we calculate an offset to add to future reads to make it
	// appear it has been zeroed at this position.
	Offset[axis] = -Count[axis];
}


long int CQEQuadEncoder::Position(unsigned int axis)
{
	unsigned short int enc, old, delta;

	if (axis >= QENC_AXIS_COUNT)
		return 0;

	tLast[axis] = tThis[axis];

	// XXX - disable interrupts?
	enc = qenc_reg[axis];			// read the encoder register
	tThis[axis] = CTimer::ticks();
	// XXX - reenable interrups?


	old = Count[axis] & 0xffff;		// take low 16b of the last reading

	if (Invert[axis])
		enc = ~enc;

	delta = enc-old; 		// compute the delta counting on overflows wrapping

	Count[axis] += (signed short int) delta;	// add the delta to the 32b accumulator
	dCount[axis] = delta;

	return (signed long int)( (unsigned long int) Count[axis] + (unsigned long int) Offset[axis] );
}


// returns counts per second
long int CQEQuadEncoder::Velocity(unsigned int axis)
{
	if (axis >= QENC_AXIS_COUNT)
		return 0;

	signed long long int change = (signed short) dCount[axis];
	signed long int interval = tThis[axis] - tLast[axis];

	if (interval <= 0)
		interval = 1; // in case we get two reads in the same microsecond...

	change *= (T4HZ*60UL);
	change /= interval;
	return change;
}


long int CQEQuadEncoder::ScaledPosition(unsigned int axis)
{
	return ((long int)Position(axis) * ScaleUnits[axis]) / Resolution[axis];
}


long int CQEQuadEncoder::ScaledVelocity(unsigned int axis)
{
	return ((long int)Velocity(axis) * ScaleUnits[axis]) / Resolution[axis];
}


CQEQuadEncoder::CQEQuadEncoder()
{
	p9302hw = C9302Hardware::GetPtr();

    if (p9302hw->GetBitstreamMajorVersion()!=0xa0)
        throw std::runtime_error("wrong FPGA bitstream version");

    qenc_reg = p9302hw->m_fpga.Ushort(0x500);	// get a pointer to the quad encoder register

	for (unsigned int axis=0; axis<QENC_AXIS_COUNT; axis++)
		this->Configure(axis, 0);	// scale of 0 disables the axis, but all the ivars get set up.
}


CQEQuadEncoder::~CQEQuadEncoder()
{
	C9302Hardware::Release();
}
