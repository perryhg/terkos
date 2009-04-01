#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "memmap.h"


CMemMap::CMemMap(unsigned int base, unsigned int size)
	{
	m_fd = open ("/dev/mem", O_RDWR);
	m_map = (unsigned char *)mmap (0, size, PROT_READ|PROT_WRITE, MAP_SHARED, m_fd, base);	
	}

CMemMap::~CMemMap()
	{
	close(m_fd);
	}

