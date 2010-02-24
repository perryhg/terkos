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
#if 1
  CQEAudioController &audio = CQEAudioController::GetRef();

  if (argc<4)
    {
      printf("usage: audio <frequency hz> <duration ms> <volume 0-255>\n");
      exit(1);
    }

  audio.SetVolume(atoi(argv[3]));
  audio.PlayTone(atoi(argv[1]), atoi(argv[2]));
#endif
#if 0
  CQEAudioController &audio = CQEAudioController::GetRef();

  audio.SetVolume(atoi(argv[1]));
  //audio.PlayClip("drums.wav");
  audio.Talk("hello there this is your friendly computer");
  //audio.PlayTone(100, 200);

#endif
}

  
