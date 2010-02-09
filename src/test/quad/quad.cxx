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
#include "9302hw.h"

#define QUAD_DEFAULT_ADDR         0x500

int main()
{
  C9302Hardware *p9302hw;
  volatile unsigned short *quad;
  p9302hw = C9302Hardware::GetPtr();
  
  quad = p9302hw->m_fpga.Ushort(QUAD_DEFAULT_ADDR); 
  while(1)
    {
      printf("%d %d %d %d\n", quad[0], quad[1], quad[2], quad[3]);
    }
  
}
