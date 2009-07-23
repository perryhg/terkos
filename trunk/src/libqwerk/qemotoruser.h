#ifndef _QEMOTORUSER_H
#define _QEMOTORUSER_H

#include "qemotor.h"

#define QEM_DEVICE                      "/dev/qemot"
#define QEM_OFFSET_UPPER_THRESHOLD      150*16 // 16 is the number of bemf measurements
#define QEM_OFFSET_WINDOW_SCALE         9
#define QEM_OFFSET_WINDOW               (1<<QEM_OFFSET_WINDOW_SCALE)
#define QEM_INIT_TIMEOUT                (1<<(QEM_OFFSET_WINDOW_SCALE+2))
#define QEM_DELAY                       9
#define QEM_PERIOD                      110
#define QEM_WRITE_QUEUE_SIZE            5*sizeof(long)

class CQwerkHardware;

class CQEMotorUser  
{
public:
  CQEMotorUser(int axis0, int axis1=-1, int axis2=-1, int axis3=-1);
  virtual ~CQEMotorUser();

  unsigned int GetFrequency();
  int          SetFrequency(unsigned int freq);
  virtual Axis_position GetPosition(unsigned int axis);             
  virtual void GetPositionVector(Axis_position position[]);  
  void         SetPWM(unsigned int axis, int pwm);
  int          GetPWM(unsigned int axis);
  void         SetPIDVGains(unsigned int axis, unsigned int pGain, 
			   unsigned int iGain, unsigned int dGain,
			   unsigned int vGain);

  void         Hold(unsigned int axis, bool val);
  void         Stop(unsigned int axis);

  // other functions...
  void         SetPosition(unsigned int axis, Axis_position pos);
  int          GetCurrent(unsigned int axis);
  void         GetCurrentVector(int current[]);
  long	       GetVelocity(unsigned int axis); 
  void         GetVelocityVector(long velocity[]);
  void         SetCalibrate(unsigned int axis, unsigned int cal);
  void         SetTiming(unsigned int axis, unsigned int waitInterval);
  bool         GetDone(unsigned int axis);
  void         HandleOffset();
  void         FlushWriteQueue(unsigned int axis);
  void         SetWriteQueueSize(unsigned int size);

protected:
  int ConstructCall(int axis, int findex, ...);

  unsigned int m_servoAxes;
  int m_handle[QEMOT_NUM_MOTORS];
  CQwerkHardware *m_pQwerk; 
};

#endif
