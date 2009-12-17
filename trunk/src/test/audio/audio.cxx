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

  
