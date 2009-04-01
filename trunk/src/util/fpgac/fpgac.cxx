#include <fstream>
#include "bitstream.h"
#include "memmap.h"

int main(int argc, char **argv)
	{
	int i, res;
	CMemMap gpio(0x80840000, 0xcc);
	CMemMap ssp(0x808a0000, 0x18);
	unsigned char bs[0x20000];
	unsigned long len = 0x20000;
	SBitstreamInfo info;
	
	if (argc<2)
		{
		printf("Usage: config <file.bit>\n");
		return -1;
		}
		
	std::ifstream is(argv[1], std::ios::binary);
	
	if (is.fail() || is.eof())
		{
		printf("Cannot open file\n");
		return -1;
		}
	
	res = CBitstream::ParseBitstream(is, bs, &len, &info);
	if (res<0)
		{
		printf("Cannot parse bitstream\n");
		return -1;
		}
	
	*gpio.Uint(0x10) = 0x0000000a; // set PA ddr
	*gpio.Uint(0x00) = 0x00000008; // set PROG to high (run)
	*gpio.Uint(0x14) = 0x00000003; // set PB ddr
	*gpio.Uint(0x04) = 0x00000003; // set M0, M1 to high
		
	*ssp.Uint(0x00) = 0x00000007; // sspcr0
	*ssp.Uint(0x04) = 0x00000010; // sspcr1
	*ssp.Uint(0x10) = 0x00000002; // sspcpsr
	*ssp.Uint(0x14) = 0x00000000; // sspiir

	*gpio.Uint(0x00) = 0x0000000a; // set PROG to low (reset)
	usleep(1);
	*gpio.Uint(0x00) = 0x00000008; // set PROG to high (run)
	// wait for INIT to go high
	while((*gpio.Uint(0x00)&0x80)==0);
	if (*gpio.Uint(0x00)&0x04)
		{
		printf("Done has not been cleared\n");
		return -1;
		}
			
	for (i=0; i<(int)len; i++)
		{
		*ssp.Uint(0x08) = bs[i];	
		while((*ssp.Uint(0x0c)&1)==0);
		if ((*gpio.Uint(0x00)&0x80)==0)
			{
			printf("Configuration error\n");
			return -1;
			}
		} 

	// run extra cycles
	for (i=0; i<10; i++)
		{
		*ssp.Uint(0x08) = 0xff;
		while((*ssp.Uint(0x0c)&1)==0);
		} 

	usleep(100000);
	if (*gpio.Uint(0x00)&0x04)
		printf("Configuration successful\n");
	else
		{
		printf("Configuration failed\n");
		return -1;
		}
	return 0;
	}
