#ifndef _TEXTLCD_H
#define _TEXTLCD_H

#define TL_HEIGHT           2
#define TL_WIDTH            16

#define TL_DELAY()          lcdDelay(20)
#define TL_DELAYL()         lcdDelay(2000)

void lcdInit(void);

void lcdClear(void);
void lcdMoveCursor(int x, int y);
int lcdPrint(char *text);
void lcdSetBacklight(int state);

void lcdPutNibble(unsigned char c);
void lcdPutByte(unsigned char c);

void lcdDelay(unsigned int us);

#endif
