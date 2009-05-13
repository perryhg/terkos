#include <cyg/hal/hal_io.h>
#include <cyg/hal/ep93xx.h>
#include <cyg/hal/textlcd.h>

 
void lcdClear(void)
{
  unsigned int reg;

  // RS low
  HAL_READ_UINT32(EP9312_GPIO_PGDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PGDDR, reg | 0x02);
  TL_DELAY();
  
  lcdPutByte(1);
}

void lcdMoveCursor(int x, int y)
{
  unsigned int reg;

  // RS low
  HAL_READ_UINT32(EP9312_GPIO_PGDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PGDDR, reg | 0x02);
  TL_DELAY();

  lcdPutByte(0x80 | (x&0x0f) + (y ? 0x40 : 0)); 
}

int lcdPrint(char *text)
{
  int i;
  unsigned int reg;

  // RS high
  HAL_READ_UINT32(EP9312_GPIO_PGDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PGDDR, reg & ~0x02);
  TL_DELAY();

  for (i=0; text[i]; i++)
    lcdPutByte(text[i]);
}

// RS eedat portg1
// R/W egpio10 portb2
// E egpio11 portb3
// D4 egpio12 portb4
// D5 egpio13 portb5
// D6 egpio14 portb6
// D7 egpio15 portb7


void lcdInit(void)
{
  unsigned int reg;

  // backlight-- output
  HAL_READ_UINT32(EP9312_GPIO_PADDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PADDR, reg | 0x08);

  // R/W and E low
  HAL_READ_UINT32(EP9312_GPIO_PBDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PBDR, reg & ~0xfc);
  HAL_READ_UINT32(EP9312_GPIO_PBDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PBDDR, reg | 0x0c);

  // RS low
  HAL_READ_UINT32(EP9312_GPIO_PGDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PGDR, reg & ~0x02);
  HAL_READ_UINT32(EP9312_GPIO_PGDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PGDDR, reg | 0x02);

  lcdPutNibble(0x02);
  lcdPutNibble(0x02);
  lcdPutNibble(0x08);
  lcdPutByte(0x0c);
  lcdPutByte(0x01);
  lcdPutByte(0x03);

  lcdClear();
}

void lcdPutNibble(unsigned char c)
{
  unsigned int reg;

  HAL_READ_UINT32(EP9312_GPIO_PBDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PBDDR,  (~(c << 4)&0xf0) | (reg & 0x0f));
  TL_DELAY();
  HAL_READ_UINT32(EP9312_GPIO_PBDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PBDDR,  reg & ~0x08);
  TL_DELAY();
  HAL_READ_UINT32(EP9312_GPIO_PBDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PBDDR,  reg | 0x08);
  TL_DELAY();
  
  HAL_READ_UINT32(EP9312_GPIO_PBDDR, reg);
  HAL_WRITE_UINT32(EP9312_GPIO_PBDDR,  reg & ~0xf0);
}

void lcdPutByte(unsigned char c)
{
  lcdPutNibble(c>>4);
  lcdPutNibble(c);
  TL_DELAYL();
}

void lcdDelay(unsigned int us)
{
  volatile unsigned int i;
  
  us *= 30;
  for (i=0; i<us; i++);
}

void lcdSetBacklight(int state)
{
  unsigned int reg;

  if (state)
    {
      HAL_READ_UINT32(EP9312_GPIO_PADR, reg);
      HAL_WRITE_UINT32(EP9312_GPIO_PADR, reg | 0x08);
    }
  else
    {
      HAL_READ_UINT32(EP9312_GPIO_PADR, reg);
      HAL_WRITE_UINT32(EP9312_GPIO_PADR, reg & ~0x08);      
    }
}


