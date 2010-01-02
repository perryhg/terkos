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

/**
 * CMemMap maps a region in the processor's physical memory space so a
 * process can access it (read/write) directly.  For example, this class 
 * allows direct and efficient interaction with the hardware peripherals.  
 * It is not typically used except at the lowest levels of this library.
 */  
class CMemMap
{
public:
  /** 
   * Constructor.
   * @param base physical address of the beginning of physical memory region
   * @param size size of memory region in bytes
   */
  CMemMap(unsigned int base, unsigned int size);
  ~CMemMap();
	
  /**
   * Get base address.
   */
  unsigned long GetMap()
  {
    return (unsigned long)m_map;
  }

  /**
   * Get/set unsigned int.
   * @param offset offset into physical memory in bytes
   * @return mapped address of physical memory at this location
   */
  inline volatile unsigned int *Uint(unsigned int offset=0) 
  {
    return (volatile unsigned int *)(m_map + offset);
  }

  /**
   * Get/set unsigned short.
   * @param offset offset into physical memory in bytes
   * @return address of physical memory at this location
   */
  inline volatile unsigned short *Ushort(unsigned int offset=0) 
  {
    return (volatile unsigned short *)(m_map + offset);
  }

  /**
   * Get/set unsigned char.
   * @param offset offset into physical memory in bytes
   * @return address of physical memory at this location
   */
  inline volatile unsigned char *Uchar(unsigned int offset=0) 
  {
    return (volatile unsigned char *)(m_map + offset);
  }
private:
  int m_fd;
  unsigned char *m_map;
};

#endif
