#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include "9302hw.h"
#include "qemotoruser.h"
#include "qemotortraj.h"
#include "textlcd.h"
#include "keypad.h"
#include "qeleds.h"
#include "qeaudio.h"

CTextLcd *g_lcd;
CKeypad *g_kp;
CQELEDController *g_led;
CQEMotorTraj *g_motor;
int g_serial;

int serialInit()
{
  struct termios newtio;

  g_serial = open("/dev/ttyAM1", O_RDWR | O_NOCTTY ); 
  if (g_serial<0) 
    {
      perror("/dev/ttyAM1"); 
      exit(-1); 
    }
  
  bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
  newtio.c_cflag = B38400 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;
  newtio.c_lflag = 0;
  newtio.c_cc[VTIME]    = 0;   /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 0;   /* blocking read until 5 chars received */
  
  tcflush(g_serial, TCIFLUSH);
  tcsetattr(g_serial, TCSANOW,&newtio);
}

int testSerial()
{
  char c;

  g_lcd->Clear();
  g_lcd->printf("Serial test");

  while(1)
    {
      c = 'A';
      write(g_serial, &c, 1);

      g_lcd->MoveCursor(1, 0);
      if (read(g_serial, &c, 1)==1)
	g_lcd->printf("PASS");
      else
	g_lcd->printf("FAIL");

      if (g_kp->GetKey(false))
	return 0;

      usleep(1000);
    }
}

int testLCD()
{
  int key;

  g_lcd->Clear();
  g_lcd->printf("LCD testLCD test");
  g_lcd->MoveCursor(1, 0);
  g_lcd->printf("LCD testLCD test");
}
 
int testKeypad()
{
  int key;

  g_lcd->Clear();
  g_lcd->printf("Keypad test");
  
  while(1)
    {
      key = g_kp->GetKey(false);
      g_lcd->MoveCursor(1, 0);      
      switch(key)
	{
	case KP_KEY_RIGHT:
	  g_lcd->printf("Right ");
	  break;
	  
	case KP_KEY_LEFT:
	  g_lcd->printf("Left  ");
	  break;
	  
	case KP_KEY_UP:
	  g_lcd->printf("Up    ");
	  break;
	  
	case KP_KEY_DOWN:
	  return 0;
	  
	case KP_KEY_CANCEL:
	  g_lcd->printf("Cancel");
	  break;
	  
	case KP_KEY_OK:
	  g_lcd->printf("OK    ");
	  break;
	}
    }
}

int testLEDs()
{
  int state = 0;
  int i;

  g_lcd->Clear();
  g_lcd->printf("LED test");
  
  while(1)
    {
      switch(state)
	{
	  // RED
	case 0:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_RED);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;

	case 1:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_RED);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_RED);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	case 2:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_RED);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_RED);
	  g_led->SetLED(LED_GAME, LED_COLOR_RED);
	  break;

	case 3:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_RED);
	  g_led->SetLED(LED_GAME, LED_COLOR_RED);
	  break;	  

	case 4:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_RED);
	  break;	  

	case 5:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	  // GREEN
	case 6:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_GREEN);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;

	case 7:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_GREEN);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_GREEN);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	case 8:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_GREEN);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_GREEN);
	  g_led->SetLED(LED_GAME, LED_COLOR_GREEN);
	  break;

	case 9:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_GREEN);
	  g_led->SetLED(LED_GAME, LED_COLOR_GREEN);
	  break;	  

	case 10:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_GREEN);
	  break;	  

	case 11:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	  // YELLOW
	case 12:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_YELLOW);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;

	case 13:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_YELLOW);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_YELLOW);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	case 14:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_YELLOW);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_YELLOW);
	  g_led->SetLED(LED_GAME, LED_COLOR_YELLOW);
	  break;

	case 15:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_YELLOW);
	  g_led->SetLED(LED_GAME, LED_COLOR_YELLOW);
	  break;	  

	case 16:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_YELLOW);
	  break;	  

	case 17:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	}
      for (i=0; i<15; i++)
	{
	  usleep(100);
	  if (g_kp->GetKey(false))
	    {
	      g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	      g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	      g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	      return 0;
	    }
	}
      state++;
      if (state==18)
	state = 0;
    }
}

int testMotors()
{
  g_lcd->Clear();
  g_lcd->printf("Motor test");

  while(1)
    {
      g_motor->Move(0, 20000, 10000, 4000);
      g_motor->Move(1, 20000, 10000, 4000);
      g_motor->Move(2, 20000, 10000, 4000);
      g_motor->Move(3, 20000, 10000, 4000);
      
      while(!g_motor->Done(0) || !g_motor->Done(1) || 
	    !g_motor->Done(2) || !g_motor->Done(3))
	{
	  usleep(100);
	  if (g_kp->GetKey(false))
	    {
	      g_motor->Stop(0);
	      g_motor->Stop(1);
	      g_motor->Stop(2);
	      g_motor->Stop(3);
	      return 0;
	    }
	}

      g_motor->Move(0, -20000, 10000, 4000);
      g_motor->Move(1, -20000, 10000, 4000);
      g_motor->Move(2, -20000, 10000, 4000);
      g_motor->Move(3, -20000, 10000, 4000);
      
      while(!g_motor->Done(0) || !g_motor->Done(1) || 
	    !g_motor->Done(2) || !g_motor->Done(3))
	{
	  usleep(100);
	  if (g_kp->GetKey(false))
	    {
	      g_motor->Stop(0);
	      g_motor->Stop(1);
	      g_motor->Stop(2);
	      g_motor->Stop(3);
	      return 0;
	    }
	}

    }
 }

int numDevs()
{
  int n = 0;
  char buf[0x100];
  FILE *fp = popen("cat /sys/bus/usb/devices/*/idProduct", "r");
  while(1)
    {
      if (fgets(buf, 0x100, fp)==NULL)
	break;
      n++;
    }
  pclose(fp);
  return n;
}

int main(int argc, char **argv)
{
  int i = 0;
  int key;

#if 0
  CQEAudioController audio;
  audio.playClip("drums.wav");
  audio.playTone(700, 255, 500);
#endif

#if 0
  int n, nPrev = numDevs();

  while(1)
    {
      n = numDevs();
      if (n-nPrev>0)
	printf("insert\n");
      else if (n-nPrev<0)
	printf("remove\n");
      nPrev = n;
      usleep(100);
    }
#endif
#if 0
  FILE *fp;
  while(1)
    {
      fp = popen("aplay drums.wav", "r");
      usleep(100000);
      pclose(fp);
      usleep(100000);     
    }
#endif

  g_kp = CKeypad::GetPtr();
  g_lcd = CTextLcd::GetPtr();
  g_led = CQELEDController::GetPtr();

  serialInit();
  // testSerial();

  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
  g_led->SetLED(LED_GAME, LED_COLOR_OFF);

  g_lcd->Clear();
  g_lcd->printf("Initializing...");

  CQEMotorTraj motor(0, 1, 2, 3);

  g_motor = &motor;
  g_motor->SetPIDVGains(0, 100, 0, 500, 0);
  g_motor->SetPIDVGains(1, 100, 0, 500, 0);
  g_motor->SetPIDVGains(2, 100, 0, 500, 0);
  g_motor->SetPIDVGains(3, 100, 0, 500, 0);

  while(1)
    {
      switch(i)
	{
	case 0:
	  testKeypad();
	  break;
	
	case 1:
	  testLCD();
	  break;

	case 2:
	  testLEDs();
	  break;

	case 3:
	  testMotors();
	  break;

	case 4:
	  testSerial();
	  break;
	}
      
      key = g_kp->GetKey();
      if (key==KP_KEY_DOWN && i<4)
	i++;
      else if (key==KP_KEY_UP && i>0)
	i--;
    }
  
#if 0
  CQEMotorTraj motor(atoi(argv[1]));
  motor.SetPIDVGains(0, 100, 0, 500, 0);
  printf("running\n");

  while(1)
    {
      motor.Move(0, 40000, 20000, 8000);
      while(!motor.Done(0)); 
      motor.Move(0, -40000, 20000, 8000);
      while(!motor.Done(0)); 
    }
#endif
}
