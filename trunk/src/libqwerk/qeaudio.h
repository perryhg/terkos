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

#ifndef __QEAUDIO_H__
#define __QEAUDIO_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <queue>
#include <map>

#include "singleton.h"
#include "9302hw.h"

#define QEAUDIO_RETURN_OK                          0
#define QEAUDIO_RETURN_ERROR                      -1

#define QEAUDIO_DEFAULT_ADDR                       0xfe0
#define QEAUDIO_MODE_TONE                          1
#define QEAUDIO_MODE_WAVE                          2
#define QEAUDIO_MODE_OFF                           0

/**
 * CQEAudioController controls audio output, such as setting volume, 
 * outputting tones, playing PCM/WAV files, and synthesizing text to speech.
 * To instantiate this class: 
 * \code
 * // by pointer
 * CQEAudioController *pac = CQEAudioController::GetPtr(); 
 * // or by reference
 * CQEAudioController &ac = CQEAudioController::GetRef();
 * \endcode
 * And when done with this class, call Release(), for each call to 
 * GetPtr() or GetRef():
 * \code
 * CQEAudioController::Release();
 * \endcode
 */ 
class CQEAudioController
{
public:
  /**
   * This internal macro handles instantiation of this class. 
   */ 
  SINGLETON(CQEAudioController);

  /**
   * Sets the volume of the audio speaker.
   * @param vol value between 0 and 255 with 0 being silent (off) and 
   * 255 being maximum volume.
   */
  void SetVolume(unsigned char vol);

  /**
   * Plays a tone for a specified period of time.
   * @param frequency the frequency of the tone in Hertz.  This value should 
   * be no less and 50 and no greater than 20000 or either inaudible audio,
   * or an inaccurate frequency will result.
   * @param duration the duration that you wish the sound to be played 
   * in milliseconds.
   * @return result code (0=success, otherwise error)
   */
  int PlayTone(unsigned short frequency, int duration);

  /**
   * Plays a PCM audio clip in memory.
   * @param clip data array that contains PCM audio
   * @param length length of data array in bytes
   * @return result code (0=success, otherwise error)
   */
  int PlayClip(const char clip[], int length);

  /**
   * Plays a PCM audio file.
   * @param filename name of PCM file to play.  Relative filename paths 
   * are with respect to the current directory.  
   * @return result code (0=success, otherwise error)
   */
  int PlayClip(const char *filename);         

  /**
   * Converts a piece of text to human speech and plays it.
   * @param string the text to be converted to speech
   * @return result code (0=success, otherwise error)
   */
  int Talk(const char *string);

private:
  C9302Hardware *m_p9302hw; 
  CQEAudioController();
  ~CQEAudioController();

  volatile unsigned short *m_config;
  volatile unsigned short *m_period;
  volatile unsigned short *m_volume;
};

#endif

