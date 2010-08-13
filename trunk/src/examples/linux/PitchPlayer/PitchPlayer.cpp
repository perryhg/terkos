/**
 * Daragh Egan
 * 8/13/10
 * PitchPlayer.cpp
 * Uses two potentiometers to generate tones
 * of varying frequency and volume.
 */

#include <qeaudio.h>
#include <qeanalog.h>
#include <keypad.h>
#include <textlcd.h>

const unsigned int PITCH = 0;
const unsigned int VOLUME = 1;

void playsong(int notes[], int length);

int main(void)
{
	CQEAudioController &audio = CQEAudioController::GetRef();
	CQEAnalog &analog = CQEAnalog::GetRef();
	CKeypad &key = CKeypad::GetRef();
	CTextLcd &lcd = CTextLcd::GetRef();

	// The octave that ends with middle C.
	// To go up an octave multiply by two, divide by two to go down.
	int c = 131, d = 147, e = 165, f = 175, g = 196, a = 220, b = 247;
	int scale[] = {c, d, e, f, g, a, b, c*2};
	int freq, vol, tmp_vol = 0;

	playsong(scale, 8);

	audio.SetVolume(32);

	// Uses two potentiometers to control frequency and volume
	while (!key.KeyCancel()) // Repeat until right arrow is pressed
	{
		// Get frequency and volume from potentiometers
		freq = analog.GetADVoltage(PITCH);
		vol = int(analog.GetADVoltage(VOLUME) / 5000.0 * 255.0);

		if (vol != tmp_vol)
		{
			audio.SetVolume(vol);
			vol = tmp_vol;
		}

		audio.PlayTone(freq, 10);

		if (!key.KeyLeft()) // Turn off display for less pause between notes.
		{
			lcd.MoveCursor(0, 0);
			lcd.printf("Vol: %d", vol);
			lcd.MoveCursor(1, 0);
			lcd.printf("Freq: %d", freq);
		}
	}

	audio.Release();
	analog.Release();
	key.Release();
	return 0;
}

/**
 * Plays each tone in the array.
 * @param notes Array of frequencies to be played.
 * @param length Size of the array.
 */
void playsong(int notes[], int length)
{
	CQEAudioController &audio = CQEAudioController::GetRef();

	for (int i = 0; i < length; i++)
		audio.PlayTone(notes[i], 500);
}
