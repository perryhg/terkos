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

class CQEAudioController
{
public:
  SINGLETON(CQEAudioController);

  void SetVolume(unsigned char vol);

  int PlayTone(unsigned short frequency, int duration);
  /**
   * @brief Immediately play a sound clip
   * @param clip Clip to play
   * @param length Length of clip (bytes)
   * @return QEAUDIO_RETURN_* code
   * May be called if items in queue
   * Will wait at least for currently executing command or play request
   * If there are any items in queue, will likely (but not assuredly)
   *   be played after currently executing command and before next item
   *   in queue
   */
  int PlayClip(const char clip[], int length);
  /**
   * @brief Immediately play a sound clip
   * @param filename Filename of clip to play
   * @return QEAUDIO_RETURN_* code
   * May be called if items in queue
   * Will wait at least for currently executing command or play request
   * If there are any items in queue, will likely (but not assuredly)
   *   be played after currently executing command and before next item
   *   in queue
   */
  int PlayClip(const char *filename);         
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

