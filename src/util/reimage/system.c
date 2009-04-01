#include <stdio.h>

int open_com(int dev)
{
  if (dev<0)
    {
      // find serial devices and print them
      
      // get user input
      open_com(dev);
      while(1)
	{
	  if (scanf("%d", &dev)==1)
	    break;
	}
    }
  else
    {
      // open serial device
    }
  return 0;
}

int select_file(void)
{

  return 0;
}
