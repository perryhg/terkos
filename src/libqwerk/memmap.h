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

#ifndef _MEMMAP_H
#define _MEMMAP_H

class CMemMap
{
public:
  CMemMap(unsigned int base, unsigned int size);
  ~CMemMap();
	
  unsigned long GetMap()
  {
    return (unsigned long)m_map;
  }
  inline volatile unsigned int *Uint(unsigned int offset=0) 
  {
    return (volatile unsigned int *)(m_map + offset);
  }
  inline volatile unsigned short *Ushort(unsigned int offset=0) 
  {
    return (volatile unsigned short *)(m_map + offset);
  }
  inline volatile unsigned char *Uchar(unsigned int offset=0) 
  {
    return (volatile unsigned char *)(m_map + offset);
  }
private:
  int m_fd;
  unsigned char *m_map;
};

#endif
