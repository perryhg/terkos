#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ymodem.h"
#ifdef WIN32
#include "pcserial.h"
#endif

/* Some important defines for YModem */
#define YMODEM_SOH	0x01
#define YMODEM_STX	0x02
#define YMODEM_EOT	0x04
#define YMODEM_ACK	0x06
#define YMODEM_NAK	0x15
#define YMODEM_CAN	0x18
#define YMODEM_C	0x43
#define YMODEM_GET_FAILED	-1

#define CPUCLK      294912000

#define BLKSIZE     8192

extern unsigned char comecho;
#ifdef WIN32
extern CPCSerial serial;
#else
extern int comfd;
#endif

int getonebyte(void);
void putonebyte(char c);


int ymodem_getc(void)
{
  int i;
  int ch;

  for(i = 0; i < 100; i++)
    {
      ch = getonebyte();
      if(ch >= 0)
	{
	  return ch;
	}
    }

  return YMODEM_GET_FAILED;
}

/* Get a character with a timeout in seconds */
int ymodem_getc_timeout(int timeout)
{
  int i;
  int ch = YMODEM_GET_FAILED;

  for(i = 0; i < timeout; i++)
    {
      ch = ymodem_getc();
      if(ch >= 0)
	{
	  return ch;
	}
    }

  return YMODEM_GET_FAILED;
}

#define ymodem_getc_minute() ymodem_getc_timeout(60)

const char *u32_to_str(unsigned int val)
{
  /* Maximum number of decimal digits in u32 is 10 */
  static char num_str[11];
  int  pos = 10;
  num_str[10] = 0;

  if(val == 0)
    {
      /* If already zero then just return zero */
      return "0";
    }

  while((val != 0) && (pos > 0))
    {
      num_str[--pos] = (val % 10) + '0';
      val /= 10;
    }

  return &num_str[pos];
}

int calcrc(const char *ptr, int count)
{
  int crc, i;

  crc = 0;

  while (--count >= 0) {
    crc = crc ^ (int)*ptr++ << 8;

    for (i = 0; i < 8; ++i)
      if (crc & 0x8000)
	crc = crc << 1 ^ 0x1021;
      else
	crc = crc << 1;
  }
  return (crc & 0xFFFF);
}


void ymodem_send_block(const unsigned char *data, int block_no)
{
  int count;
  int crc;
  unsigned char block[BLKSIZE];

  memcpy(block, data, BLKSIZE);
  crc = calcrc((char *)block, BLKSIZE);
  putonebyte(YMODEM_SOH);
  putonebyte(block_no & 0xFF);
  putonebyte(~block_no & 0xFF);
#if 0
  for(count = 0; count < BLKSIZE; count++)
    putonebyte(block[count]);
#else
  int res;
  count = 0;
  while(1)
    {
      if (count>=BLKSIZE)
	break;
#ifdef WIN32
      res = serial.Send((char *)(block + count), BLKSIZE - count, 0xffff);
#else
      res = write(comfd, block + count, BLKSIZE - count);
#endif
      if (res>0)
	count += res;
    }
#endif
  putonebyte((crc >> 8) & 0xFF);
  putonebyte(crc & 0xFF);
}

/* Send block 0 (the filename block) */
void ymodem_send_block0(const char* filename, unsigned int size)
{
  int count;
  unsigned char block[BLKSIZE];
  const char* num;

  memset(block, 0, BLKSIZE);

  if(filename != NULL)
    {
      count = 0;
      while(*filename)
	{
	  block[count++] = *filename++;
	}
      block[count++] = 0;

      num = u32_to_str(size);
      while(*num)
	{
	  block[count++] = *num++;
	}
    }

  ymodem_send_block(block, 0);
}


void ymodem_send_data_blocks(const unsigned char* data, unsigned int size)
{
  int blockno = 1;
  int send_size;
  int ch;

  while(size > 0)
    {
      if(size > BLKSIZE)
	{
	  send_size = BLKSIZE;
	}
      else
	{
	  send_size = size;
	}

      ymodem_send_block(data, blockno);
      ch = ymodem_getc_minute();
      if(ch == YMODEM_ACK)
	{
	  blockno++;
	  data += send_size;
	  size -= send_size;
	  if (blockno%3==0 && comecho)
	    {
	      printf(".");
	      fflush(stdout);
	    }
	}
      else 
	{
	  if((ch == YMODEM_CAN) || (ch == -1))
	    {
	      return;
	    }
	  else if (comecho)
	    {
	      printf("r");
	      fflush(stdout);
	    }
	}
    }

  do
    {
      putonebyte(YMODEM_EOT);
      ch = ymodem_getc_minute();
    }
  while((ch != YMODEM_ACK) && (ch != -1));

  /* Send last data packet */
  if(ch == YMODEM_ACK)
    {
      ch = ymodem_getc_minute();
      if(ch == YMODEM_C)
	{
	  do
	    {
	      ymodem_send_block0(NULL, 0);
	      ch = ymodem_getc_minute();
	    } while((ch != YMODEM_ACK) && (ch != -1));
	}
    }
}

void ymodem_send_data(const unsigned char* data, unsigned int size, const char* filename)
{
  int ch;
  int done;
  /* Assume parameters are all valid... */
  done = 0;
  /* Flush the RX FIFO */
  while(getonebyte() != -1);
  ch = ymodem_getc_minute();
  if(ch == YMODEM_C)
    {
      do
	{
	  ymodem_send_block0(filename, size);
	  ch = ymodem_getc_minute();
	  if(ch == YMODEM_ACK)
	    {
	      ch = ymodem_getc_minute();
	      if(ch == YMODEM_C)
		{
		  ymodem_send_data_blocks(data, size);
		  done = 1;
		}
	    }
	  else if(ch != YMODEM_NAK)
	    {
	      /* Abort */
	      putonebyte(YMODEM_CAN);
	      putonebyte(YMODEM_CAN);
	      done = 1;
	    }
	}
      while(!done);
    }
  else
    {
      /* Abort transfer */
    }
}


