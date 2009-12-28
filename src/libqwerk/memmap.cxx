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

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "memmap.h"
#include <stdio.h>

CMemMap::CMemMap(unsigned int base, unsigned int size)
	{
	m_fd = open ("/dev/mem", O_RDWR);
	m_map = (unsigned char *)mmap (0, size, PROT_READ|PROT_WRITE, MAP_SHARED, m_fd, base);	
	}

CMemMap::~CMemMap()
	{
	close(m_fd);
	}

