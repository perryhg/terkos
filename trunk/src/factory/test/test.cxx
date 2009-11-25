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
#include "qeanalog.h"
#include "qepower.h"

CTextLcd *g_lcd;
CKeypad *g_kp;
CQELEDController *g_led;
CQEMotorTraj *g_motor;
C9302Hardware *g_9302;
CQEAudioController *g_audio;
CQEAnalog *g_analog;
CQEPower *g_power;
int g_serial;

bool g_preI2C;
bool g_preSerial;
unsigned short g_preAnalog;

bool helpI2C();
bool helpSerial();
unsigned short  helpAnalog();

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

bool helpSerial()
{
  char c;
  bool result;

  while(read(g_serial, &c, 1));	
  c = 'A';
  write(g_serial, &c, 1);
  usleep(1);
  result = read(g_serial, &c, 1)==1;
  return result;
}

int testSerial()
{

  g_lcd->Clear();
  g_lcd->printf("Serial test");

  while(1)
    {
      g_lcd->MoveCursor(1, 0);
      if (!g_preSerial && helpSerial())
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
  int i;

  g_lcd->Clear();
  g_lcd->printf("LCD testLCD test");
  g_lcd->MoveCursor(1, 0);
  g_lcd->printf("LCD testLCD test");

  while(1)
    {
      g_lcd->SetProperty(TL_PROP_BACKLIGHT, true);
      for (i=0; i<50; i++)
	{
	  usleep(100);
	  if (g_kp->GetKey(false))
	    {
	      g_lcd->SetProperty(TL_PROP_BACKLIGHT, true);
	      return 0;
	    }
	}
      g_lcd->SetProperty(TL_PROP_BACKLIGHT, false);
      for (i=0; i<50; i++)
	{
	  usleep(100);
	  if (g_kp->GetKey(false))
	    {
	      g_lcd->SetProperty(TL_PROP_BACKLIGHT, true);
	      return 0;
	    }
	}
    }
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
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	case 2:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_RED);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;

	case 3:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
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
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;	  

	case 8:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_GREEN);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
	  break;

	case 9:
	  g_led->SetLED(LED_ROBOT, LED_COLOR_OFF);
	  g_led->SetLED(LED_VEXNET, LED_COLOR_OFF);
	  g_led->SetLED(LED_GAME, LED_COLOR_OFF);
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

	}
      for (i=0; i<30; i++)
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
      if (state==12)
	state = 0;
    }
}

int addHeaders()
{
  g_preI2C = helpI2C();
  g_preSerial = helpSerial();
  g_preAnalog = helpAnalog();

  g_lcd->Clear();
  g_lcd->printf("Add all test"); 
  g_lcd->MoveCursor(1, 0);
  g_lcd->printf("headers, motors");

  while(1)
    {
      if (g_kp->GetKey(false))
	return 0;
      usleep(100);
    }
}

int testMotors()
{
  g_lcd->Clear();
  g_lcd->printf("Motor test");

  while(1)
    {
      g_motor->Move(0, 5000, 10000, 100000);
      g_motor->Move(1, 5000, 10000, 100000);
      g_motor->Move(2, 5000, 10000, 100000);
      g_motor->Move(3, 5000, 10000, 100000);
      
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

      g_motor->Move(0, -5000, 10000, 100000);
      g_motor->Move(1, -5000, 10000, 100000);
      g_motor->Move(2, -5000, 10000, 100000);
      g_motor->Move(3, -5000, 10000, 100000);
      
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

bool helpI2C()
{
  // set data in, clk low, loopback should produce 0, test data in, clk out
  *g_9302->m_fpga.Ushort(0x480) = 0x0000;
  usleep(100);
  if (*g_9302->m_fpga.Ushort(0x480)!=0)
    return false;
  
  // set data out and low, clk high, loopback should produce 4, test clk in
  *g_9302->m_fpga.Ushort(0x480) = 0x0006;
  usleep(100);
  if (*g_9302->m_fpga.Ushort(0x480)!=4)
    return false;
  
  // set data in, clk high, loopback should produce 3
  *g_9302->m_fpga.Ushort(0x480) = 0x0002;
  usleep(100);
  if (*g_9302->m_fpga.Ushort(0x480)!=3)
    return false;

  return true;
}

int testI2C()
{
  g_lcd->Clear();
  g_lcd->printf("I2C test");

  while(1)
    {
      if (g_kp->GetKey(false))
	return 0;

      g_lcd->MoveCursor(1, 0);

      if (!g_preI2C && helpI2C())
	g_lcd->printf("PASS");
      else
	g_lcd->printf("FAIL");

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

int testUSB()
{
  int n, m=0, nPrev = numDevs();

  g_lcd->Clear();
  g_lcd->printf("USB test");

  while(1)
    {
      g_lcd->MoveCursor(1, 0);      
      n = numDevs();
      if (n-nPrev>0)
	g_lcd->printf("Inserted");
      else if (n-nPrev<0)
	{
	  if (m>=1)
	    g_lcd->printf("Pass    ");
	  else
	    g_lcd->printf("Removed ");
	  m++;
	}
      nPrev = n;

      if (g_kp->GetKey(false))
	return 0;

      usleep(100);
    }
}

int testAudio()
{
  g_lcd->Clear();
  g_lcd->printf("Audio test");

  g_audio->SetVolume(50);
  while(1)
    {
      g_audio->PlayTone(700, 150);

      if (g_kp->GetKey(false))
	return 0;

      g_audio->PlayTone(350, 150);

      if (g_kp->GetKey(false))
	return 0;
    }
}

#define VOLTAGE_TOL    500
#define WITHIN_TOL(v, ref)  (((v)-(ref))<VOLTAGE_TOL && ((v)-(ref))>-VOLTAGE_TOL)

int testPower()
{
  int battv, v, error;

  g_lcd->Clear();
  g_lcd->printf("Power test");

  while(1)
    {
      error = 0;
      g_lcd->MoveCursor(1, 0);      
      battv = g_analog->GetADVoltage(24);

      v = g_analog->GetADVoltage(16);
      if (WITHIN_TOL(v, battv))
	g_lcd->printf("\xfc");
      else 
	{
	  g_lcd->printf("X");
	  error = 1;
	}
      v = g_analog->GetADVoltage(17);
      if (WITHIN_TOL(v, battv))
	g_lcd->printf("\xfc");
      else 
	{
	  g_lcd->printf("X");
	  error = 1;
	}
      v = g_analog->GetADVoltage(18);
      if (WITHIN_TOL(v, battv))
	g_lcd->printf("\xfc");
      else 
	{
	  g_lcd->printf("X");
	  error = 1;
	}
      v = g_analog->GetADVoltage(19);
      if (WITHIN_TOL(v, battv))
	g_lcd->printf("\xfc");
      else 
	{
	  g_lcd->printf("X");
	  error = 1;
	}
      v = g_analog->GetADVoltage(20);
      if (WITHIN_TOL(v, battv))
	g_lcd->printf("\xfc");
      else 
	{
	  g_lcd->printf("X");
	  error = 1;
	}
      v = g_analog->GetADVoltage(22);
      if (WITHIN_TOL(v, 5000))
	g_lcd->printf("\xfc");
      else 
	{
	  g_lcd->printf("X");
	  error = 1;
	}
      v = g_analog->GetADVoltage(23);
      if (WITHIN_TOL(v, 5000))
	g_lcd->printf("\xfc");
      else
	{ 
	  g_lcd->printf("X");
	  error = 1;
	}

      if (error)
	g_lcd->printf(" FAIL");
      else
	g_lcd->printf(" PASS");

      if (g_kp->GetKey(false))
	return 0;
    }
}

unsigned short helpAnalog()
{
  int i, v;
  unsigned short val = 0;

  for (i=15; i>=0; i--)
    {
      val <<= 1;
      v = g_analog->GetADVoltage(i);
      if (WITHIN_TOL(v, 5000))
	val |= 0x0001;
     }
  return val;
}

int testAnalog()
{
  int i, val, error;

  g_lcd->Clear();
  g_lcd->printf("Analog test");

  while(1)
    {
      error = 0;
      val = helpAnalog();
      g_lcd->MoveCursor(1, 0);      

      for (i=15; i>=0; i--)
	{
	  if ((~g_preAnalog)&val&(1<<i))
	    g_lcd->printf("\xfc");
	  else 
	    {
	      g_lcd->printf("X");
	      error = 1;
	    }
	}
      g_lcd->MoveCursor(0, 12); 
      if (error)
	g_lcd->printf("FAIL");
      else
	g_lcd->printf("PASS");

      if (g_kp->GetKey(false))
	return 0;
    }
}

int testBackup()
{
  g_lcd->Clear();
  g_lcd->printf("Backup batt test");
  
  while(1)
    {
      g_lcd->MoveCursor(1, 0);      
      if (g_analog->GetADVoltage(21)>6800 &&
	  g_analog->GetADVoltage(24)<5000)
	g_lcd->printf("PASS");

      usleep(1000);
      if (g_kp->GetKey(false))
	return 0;
    }
}

#define D_DATA (*g_9302->m_fpga.Ushort(0x440))
#define D_DIR  (*g_9302->m_fpga.Ushort(0x440+2))

int testDigital()
{
  int i;
  unsigned short error;

  g_lcd->Clear();
  g_lcd->printf("Digital test");

  while(1)
    {
      error = 0;
      D_DIR = 0x00ff;
      
      D_DATA = 0x00aa;   
      usleep(1);
      if ((D_DATA&0xff00)!=0xaa00)
	error |= (D_DATA&0xff00)^0xaa00;

      D_DATA = 0x0055;      
      usleep(1);
      if ((D_DATA&0xff00)!=0x5500)
	error |= (D_DATA&0xff00)^0x5500;

      D_DIR = 0xff00;
      
      D_DATA = 0xaa00;   
      usleep(1);
      if ((D_DATA&0x00ff)!=0x00aa)
	error |= (D_DATA&0x00ff)^0x00aa;

      D_DATA = 0x5500;      
      usleep(1);
      if ((D_DATA&0x00ff)!=0x0055)
	error |= (D_DATA&0x00ff)^0x0055;

      g_lcd->MoveCursor(0, 12);      
      if (error)
	g_lcd->printf("FAIL");
      else
	g_lcd->printf("PASS");

      g_lcd->MoveCursor(1, 0);      
      for (i=0; i<16; i++)
	{
	  if (error&0x8000)
	    g_lcd->printf("X");
	  else
	    g_lcd->printf("\xfc");
	  error <<= 1;
	}
	    
      if (g_kp->GetKey(false))
	return 0;
    }
}

#undef D_DATA
#undef D_DIR
#define D_DATA (*g_9302->m_fpga.Ushort(0x400))
#define D_DIR  (*g_9302->m_fpga.Ushort(0x400+2))

int testServo()
{
  int i;
  unsigned short error;

  g_lcd->Clear();
  g_lcd->printf("Servo test");

  while(1)
    {
      error = 0;
      D_DIR = 0x003f;
      
      D_DATA = 0x002a;   
      usleep(1);
      if ((D_DATA&0x0fc0)!=0x0a80)
	error |= (D_DATA&0x0fc0)^0x0a80;

      D_DATA = 0x0015;      
      usleep(1);
      if ((D_DATA&0x0fc0)!=0x0540)
	error |= (D_DATA&0x0fc0)^0x0540;

      D_DIR = 0x0fc0;
      
      D_DATA = 0x0a80;   
      usleep(1);
      if ((D_DATA&0x003f)!=0x002a)
	error |= (D_DATA&0x003f)^0x002a;

      D_DATA = 0x0540;      
      usleep(1);
      if ((D_DATA&0x003f)!=0x0015)
	error |= (D_DATA&0x003f)^0x0015;

      g_lcd->MoveCursor(0, 12);      
      if (error)
	g_lcd->printf("FAIL");
      else
	g_lcd->printf("PASS");

      g_lcd->MoveCursor(1, 0);      
      for (i=0; i<12; i++)
	{
	  if (error&0x800)
	    g_lcd->printf("X");
	  else
	    g_lcd->printf("\xfc");
	  error <<= 1;
	}
	    
      if (g_kp->GetKey(false))
	return 0;
    }
}

int testPowerDown()
{
  int key;

  g_lcd->Clear();
  g_lcd->printf("Power down test");
  g_lcd->MoveCursor(1, 0);      
  g_lcd->printf("(Press OK)");

  while(1)
    {
      key = g_kp->GetKey(false);
      if (key==KP_KEY_OK)
	{
	  printf("Powering off...\n");
	  sleep(1);
	  g_power->PowerOff();
	}
      else if (key)
	return 0;
     }
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
  FILE *fp;
  while(1)
    {
      fp = popen("aplay drums.wav", "r");
      usleep(100000);
      pclose(fp);
      usleep(100000);     
    }
#endif
  g_9302 = C9302Hardware::GetPtr();
  if (g_9302->GetBitstreamVersion()!=0xa0ff)
    {
      printf("wrong FPGA bitstrem version -- need test bitstream\n");
      exit(1);
    }
  g_kp = CKeypad::GetPtr();
  g_lcd = CTextLcd::GetPtr();
  g_led = CQELEDController::GetPtr();
  g_audio = CQEAudioController::GetPtr();
  g_analog = CQEAnalog::GetPtr();
  g_power = CQEPower::GetPtr();

  serialInit();

  g_lcd->SetProperty(TL_PROP_BACKLIGHT, true);
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
	  addHeaders();
	  break;

	case 4:
	  testMotors();
	  break;

	case 5:
	  testAudio();
	  break;

	case 6:
	  testDigital();
	  break;
	  
	case 7:
	  testI2C();
	  break;

	case 8:
	  testSerial();
	  break;

	case 9:
	  testServo();
	  break;

	case 10:
	  testAnalog();
	  break;

	case 11:
	  testUSB();
	  break;

	case 12:
	  testBackup();
	  break;

	case 13: 
	  testPowerDown();
	  break;
	}
      
      key = g_kp->GetKey();
      if (key==KP_KEY_DOWN && i<13)
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
