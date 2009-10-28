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
