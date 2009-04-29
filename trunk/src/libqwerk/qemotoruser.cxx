#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "qemotoruser.h"
#include "qwerkhw.h"

CQEMotorUser::CQEMotorUser(CQwerkHardware *pQwerk, int axis0, int axis1, int axis2, int axis3)
{
  char dev[0x10];
  int len;
  unsigned int axis;
  unsigned short motorVoltage;

  if (pQwerk==NULL)
    {
      m_pQwerk = new CQwerkHardware();
      m_allocated = true;
    }
  else
    {
      m_pQwerk = pQwerk;
      m_allocated = false;
    }

#if Q1
  // set voltage divider
  motorVoltage = m_pQwerk->GetBattVoltage();  
  if (motorVoltage<10000)
    m_pQwerk->SetMotorVoltageScale(QHW_DIV_365TH);
  else if (motorVoltage>20000)
    m_pQwerk->SetMotorVoltageScale(QHW_DIV_91TH);
  else
    m_pQwerk->SetMotorVoltageScale(QHW_DIV_108TH);
#endif
  
  // open motor device
  strcpy(dev, QEM_DEVICE);
  len = strlen(dev);
  dev[len+1] = '\0';

  dev[len] = axis0+'0';
  m_handle[0] = open(dev, O_RDWR);
  // TODO: error checking?
  m_servoAxes = 1;
  
  if (axis1>=0)
    {
      dev[len] = axis1+'0';
      m_handle[1] = open(dev, O_RDWR);
      m_servoAxes++;
    }
  if (axis2>=0)
    {
      dev[len] = axis2+'0';
      m_handle[2] = open(dev, O_RDWR);
      m_servoAxes++;
    }
  if (axis3>=0)
    {
      dev[len] = axis3+'0';
      m_handle[3] = open(dev, O_RDWR);
      m_servoAxes++;
    } 

  // set frequency and timing
  for (axis=0; axis<m_servoAxes; axis++)
    SetTiming(axis, QEM_DELAY);
  SetFrequency(QEM_PERIOD);

  HandleOffset();
  SetWriteQueueSize(QEM_WRITE_QUEUE_SIZE);
}

CQEMotorUser::~CQEMotorUser()
{
  int axis;

  for (axis=0; axis<m_servoAxes; axis++)
    {
      if (m_handle[axis]>=0)
	close(m_handle[axis]);
    }

  if (m_allocated)
    delete m_pQwerk;
}

int CQEMotorUser::ConstructCall(int axis, int findex, ...)
{
  int result;
  va_list argp;
  va_start(argp, findex);

  unsigned long args[] =
    {va_arg(argp, int), va_arg(argp, int),
     va_arg(argp, int), va_arg(argp, int),
     va_arg(argp, int)};

  result = ioctl(m_handle[axis], findex, args);

  va_end(argp);

  return result;
}

void CQEMotorUser::HandleOffset()
{
  unsigned short status;
  int axis;
  int axes;
  int i;
  int j;
  int k;
  long bemf;
  unsigned int window[m_servoAxes];
  long average[m_servoAxes];
  long offset[m_servoAxes];
  bool finished[m_servoAxes];

  for (axis=0; axis<m_servoAxes; axis++)
    {
      offset[axis] = 0;
      window[axis] = 0;
      average[axis] = 0;
      finished[axis] = false;
    }

  k = 0;
  axes = m_servoAxes;

  while(axes && k<QEM_INIT_TIMEOUT)
    {
      while(1)
	{
	  ConstructCall(0, MOT_GET_STATUS, &status);
	  if ((status&0x01)==0)
	    break;
	}
      while(1)
	{
	  ConstructCall(0, MOT_GET_STATUS, &status);
	  if ((status&0x01)!=0)
	    break;
	}  

      k++;
      for (axis=0, j=0; axis<m_servoAxes; axis++)
	{
	  ConstructCall(axis, MOT_GET_BEMF, &bemf);

	  if (!finished[axis])
	    {
	      if (bemf<QEM_OFFSET_UPPER_THRESHOLD && bemf>-QEM_OFFSET_UPPER_THRESHOLD)
		{
		  average[axis] += bemf;
		  window[axis]++;
					
		  if (window[axis]==QEM_OFFSET_WINDOW)
		    {
		      average[axis] += 1<<(QEM_OFFSET_WINDOW_SCALE-1);
		      offset[axis] = average[axis]>>QEM_OFFSET_WINDOW_SCALE;
		      finished[axis] = true;
		      axes--;
		    }
		}
	      else
		{
		  average[axis] = 0;
		  window[axis] = 0;
		}
	    }
	}
    }
  
  for (axis=0; axis<m_servoAxes; axis++)
    ConstructCall(axis, MOT_SET_BEMF_OFFSET, offset[axis]);
}

unsigned int CQEMotorUser::GetFrequency()
{
  int result;

  while(1)
    {
      ConstructCall(0, MOT_GET_FREQUENCY, &result);
      if (result>0)
	break;
      usleep(1);
    }
  return result;
}


int CQEMotorUser::SetFrequency(unsigned int freq)
{
  unsigned int afreq, period, span;
  int error;

  span = 0x80;
  period = 0xff;

  // do binary search for the value of period that works 
  while(span)
    {
      if (period>0xff)
	return -1;
      ConstructCall(0, MOT_SET_PERIOD, period); 
      afreq = GetFrequency();
      if (afreq<freq)
	period -= span;
      else if (afreq>freq)
	period += span;
      else 
	return 0;
      span >>= 1;
    }

  // calculate error
  error = afreq-freq;
  if (error<0) 
    error = -error;

  if (error>3)
    return -1;
  else
    return 0;
}

Axis_position CQEMotorUser::GetPosition(unsigned int axis)
{
  Axis_position pos;

  ConstructCall(axis, MOT_GET_POSITION, &pos);

  return pos;
}

void CQEMotorUser::GetPositionVector(Axis_position position[])
{
}

void CQEMotorUser::SetPWM(unsigned int axis, int pwm)
{
 ConstructCall(axis, MOT_SET_PWM, pwm);  
}

int CQEMotorUser::GetPWM(unsigned int axis)
{
  int pwm;
  ConstructCall(axis, MOT_GET_PWM, &pwm);  

  return pwm;
}

void CQEMotorUser::SetPosition(unsigned int axis, Axis_position pos)
{
 ConstructCall(axis, MOT_SET_POSITION, pos);  
}

int CQEMotorUser::GetCurrent(unsigned int axis)
{
  int current;

  ConstructCall(axis, MOT_GET_MCURRENT, &current);

  return current;
}

void CQEMotorUser::GetCurrentVector(int current[])
{
}

long CQEMotorUser::GetVelocity(unsigned int axis)
{
  long velocity;

  ConstructCall(axis, MOT_GET_VELOCITY, &velocity);
  return velocity;
}

void CQEMotorUser::GetVelocityVector(long velocity[])
{
}

void CQEMotorUser::SetCalibrate(unsigned int axis, unsigned int cal)
{
  ConstructCall(axis, MOT_SET_BEMF_CALIBRATE, cal);  
}

void CQEMotorUser::SetTiming(unsigned int axis, unsigned int waitInterval)
{
  ConstructCall(axis, MOT_SET_BEMF_TIMING, waitInterval);    
}

void CQEMotorUser::SetPIDVGains(unsigned int axis, unsigned int pGain, 
			       unsigned int iGain, unsigned int dGain,
			       unsigned int vGain)
{
  ConstructCall(axis, MOT_SET_PIDV_GAINS, pGain, iGain, dGain, vGain);  
}

void CQEMotorUser::Hold(unsigned int axis, bool val)
{
  int ival = val;
  ConstructCall(axis, MOT_SET_HOLD, ival);  
}


bool CQEMotorUser::GetDone(unsigned int axis)
{
  unsigned short done;
  ConstructCall(axis, MOT_GET_DONE, &done);
  return done ? true : false;
}

void CQEMotorUser::FlushWriteQueue(unsigned int axis)
{
  ConstructCall(axis, MOT_FLUSH_WRITE_QUEUE);
}

void CQEMotorUser::SetWriteQueueSize(unsigned int size)
{  
  ConstructCall(0, MOT_SET_WRITE_QUEUE_SIZE, size);
}

void CQEMotorUser::Stop(unsigned int axis)
{
  ConstructCall(axis, MOT_STOP);
}

CQwerkHardware *CQEMotorUser::GetHardware()
{
  return m_pQwerk;
}
