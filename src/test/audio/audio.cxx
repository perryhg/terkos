#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "memmap.h"
#include "9302hw.h"
#include "qwerkhw.h"
#include "qeaudio.h"

int main(int argc, char **argv)
{
#if 0
  int i, res;
  unsigned short s, read;

  CQwerkHardware hw;

  hw.SetAudioEnable(true);

  printf("usage: audio <frequency hz> <volume 0-255> <duration ms>\n");

  if (argc==1)
    {
      *(hw.m_p9302hw->m_fpga.Ushort)(QHW_AUDIO_VOLUME) = 0;
      exit(0);
    }
      
  if (argc>1)
    *hw.m_p9302hw->m_fpga.Ushort(QHW_AUDIO_PERIOD) = 1562500/atoi(argv[1]);
  if (argc>2)
    *hw.m_p9302hw->m_fpga.Ushort(QHW_AUDIO_VOLUME) = atoi(argv[2]);
  else 
    *hw.m_p9302hw->m_fpga.Ushort(QHW_AUDIO_VOLUME) = 128;
  
  if (argc>3)
    usleep(1000*atoi(argv[3]));
  else 
    usleep(200000);
  
  *hw.m_p9302hw->m_fpga.Ushort(QHW_AUDIO_VOLUME) = 0;
#endif
#if 1
  CQEAudioController &audio = CQEAudioController::GetRef();

  audio.SetVolume(atoi(argv[1]));
  //audio.PlayClip("drums.wav");
  audio.Talk("hello there this is your friendly computer");
  //audio.PlayTone(100, 200);

#endif
}

  
