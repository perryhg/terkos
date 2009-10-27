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
  int PlayClip(const char clip[], int length);
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

