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
#include "memmap.h"

int main(int argc, char **argv)
	{
	int i, res;
	unsigned short s, read;
	CMemMap scr(0x80080000, 0x44);
	CMemMap bank2(0x20000000, 0x1000);

	printf("%x\n", *scr.Uint(0x08));
	*scr.Uint(0x08) = 0x10000440; // 0x10000c40;
	*bank2.Ushort(0x02) = 0xddee;
	*bank2.Ushort(0x00) = 0xbeef;
	printf("%x\n", *bank2.Ushort(0x00));	
	printf("%x\n", *bank2.Ushort(0x02));	
	printf("%x\n", *bank2.Ushort(0x04));
	
	s = i = 0;
	while(1)
		{
		*bank2.Ushort(0x02) = s;
		*bank2.Ushort(0x00) = s+0xff;
		if ((read=*bank2.Ushort(0x02))!=s || *bank2.Ushort(0x00)!=(unsigned short)(s+0xff))
			{
			printf("error %x %x %x %x\n", read, *bank2.Ushort(0x02), s, bank2.Ushort(0x00));
			return -1;
			}
		s++;
		if (s==0)
			printf("%d\n", i++);
		}		
	return 0;
	}
