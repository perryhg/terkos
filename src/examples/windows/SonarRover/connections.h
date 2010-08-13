/**
 * Daragh Egan
 * 8/13/10
 * connections.h
 * Connections for the Vex Pro.
 *
 * The idea is to provide a central
 * place for defining port numbers
 * such that they can be easily
 * changed, and shared between
 * different source files.
 */

#ifndef CONNECTIONS_H
#define CONNECTIONS_H
#endif

#include <keypad.h>
#include <qeanalog.h>
#include <qeaudio.h>
#include <qegpioint.h>
#include <qeleds.h>
#include <qemotortraj.h>
#include <qeservo.h>

// MOTOR Ports
const unsigned int RMOTOR = 0;
const unsigned int LMOTOR = 3;
const unsigned int RSERVO = 0;
const unsigned int LSERVO = 1;

// ANALOG Ports
const unsigned int RPHOTO = 0;
const unsigned int LPHOTO = 15;

// DIGITAL Ports
const unsigned int ROUT = 0;	// Input wire goes here
const unsigned int RIN  = 1;	// Output wire goes here
const unsigned int RENC[] = {2, 3};
const unsigned int RBUMPER = 4;
const unsigned int LBUMPER = 11;
const unsigned int LENC[] = {13, 12};
const unsigned int LIN  = 14;	// Output wire goes here
const unsigned int LOUT = 15;	// Input wire goes here

// References
CKeypad &key = CKeypad::GetRef();
CQEAnalog &analog = CQEAnalog::GetRef();
CQEAudioController &audio = CQEAudioController::GetRef();
CQEGpioInt &io = CQEGpioInt::GetRef();
CQELEDController &led = CQELEDController::GetRef();
CQEMotorTraj &motor = CQEMotorTraj::GetRef();
CQEServo &servo = CQEServo::GetRef();
