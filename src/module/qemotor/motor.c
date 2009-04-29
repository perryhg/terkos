#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <asm/io.h>
#include <linux/time.h>
#include <linux/kfifo.h>
#include <asm/uaccess.h>

#include "../../../src/libqwerk/qemotor.h"
#include "motor.h"
#include "../qeinterrupt/qeinterrupt.h"

#if 1 // debug
#define DPK(format, ...) printk(KERN_NOTICE "qe_motor(motor): " format, ## __VA_ARGS__)
#else
#define DPK(format, ...)
#endif

#define PK(format, ...) printk(KERN_ALERT "qe_motor(motor): " format, ## __VA_ARGS__)

struct mot_axis_data
{
  short mode;
  Axis_position position;
  long long hopper;
  short offset;
  int mcurrent;
  int mcurrent_limit;
  long velocity;
  unsigned short cal_scale;
  int pwm;

  Axis_position desired_position;
  int error_previous;
  int error_integral;
  unsigned int p_gain;
  unsigned int i_gain;
  unsigned int d_gain;
  unsigned int v_gain;
  unsigned int max_integral;
  unsigned int max_error;
  unsigned char trajectory;
  unsigned char hold;
  long bemf;

  struct kfifo *write_queue;
  atomic_t flushed_write;
  spinlock_t write_queue_lock;

  struct kfifo *read_queue;
  spinlock_t read_queue_lock;

  wait_queue_head_t waitqueue;
  atomic_t used;
  unsigned int timer;
};

static volatile unsigned short *mot_reg_pwm_control;
static volatile unsigned short *mot_reg_status;
static volatile short *mot_reg_bemf_measurement;
static volatile short *mot_reg_mcurrent_measurement;
static volatile unsigned short *mot_reg_wait_interval;
static volatile unsigned short *mot_reg_timing;

static struct mot_axis_data *mot_axes = NULL;
unsigned int mot_write_queue_size = MOT_DEFAULT_QUEUE_SIZE;
unsigned int mot_read_queue_size = MOT_DEFAULT_QUEUE_SIZE;

extern int qe_motor_period_usec;
extern int qe_motor_time_usec_prev;

static void mot_handle_bemf(void)
{
  int i;
  int j;
  unsigned int axis;
  long bemf;

  for (axis=0, j=0; axis<QEMOT_NUM_MOTORS; axis++)
    {
      for (i=0, bemf=0; i<MOT_BEMF_MEASUREMENTS; i++, j++)
      	bemf += mot_reg_bemf_measurement[j];

      mot_axes[axis].bemf = bemf;
      
      bemf -= mot_axes[axis].offset;
      bemf *= mot_axes[axis].cal_scale;
      bemf += 1<<(MOT_CAL_SCALE-1);
      bemf >>= MOT_CAL_SCALE;
      mot_axes[axis].hopper += bemf;
      mot_axes[axis].velocity = bemf;
      mot_axes[axis].position = (Axis_position)(mot_axes[axis].hopper>>MOT_HOPPER_SCALE);
    }
}

void mot_handle_current(void)
{
  int i;
  int j;
  unsigned int axis;
  int mcurrent;

  for (axis=0, j=0; axis<QEMOT_NUM_MOTORS; axis++)
    {
      if (mot_axes[axis].pwm!=0) 
	{
	  for (i=0, mcurrent=0; i<MOT_MCURRENT_MEASUREMENTS; i++, j++)
	    mcurrent += mot_reg_mcurrent_measurement[j];
	  
	  // scale down
	  mcurrent >>= MOT_MCURRENT_SCALE;
	  
	  // deal with sign
	  if ((mot_axes[axis].pwm<0 && mcurrent>0) ||
	      (mot_axes[axis].pwm>0 && mcurrent<0))
	    mcurrent = -mcurrent;

	  mot_axes[axis].mcurrent = mcurrent;
	}
      else
	mot_axes[axis].mcurrent = 0;	
    }
 }

static void mot_handle_pid_control(void)
{
  int error;
  int pwm;
  unsigned int axis;

  for (axis=0; axis<QEMOT_NUM_MOTORS; axis++)
    {
      if (mot_axes[axis].hold || mot_axes[axis].trajectory)
	{
	  error = mot_axes[axis].desired_position - mot_axes[axis].position;
	  pwm = mot_axes[axis].p_gain*error + 
	    mot_axes[axis].d_gain*(error - mot_axes[axis].error_previous);
	  
	  if (mot_axes[axis].i_gain)
	    {
	      pwm += mot_axes[axis].i_gain*(mot_axes[axis].error_integral >> MOT_POSITION_SCALE);
	      mot_axes[axis].error_integral += error;
	      if (mot_axes[axis].error_integral>(int)mot_axes[axis].max_integral)
		mot_axes[axis].error_integral = mot_axes[axis].max_integral;
	      else if (mot_axes[axis].error_integral<-(int)mot_axes[axis].max_integral)
		mot_axes[axis].error_integral = -mot_axes[axis].max_integral;
	    }
	  
	  mot_axes[axis].error_previous = error;
	  
	  mot_axes[axis].pwm = pwm >> MOT_COMMAND_SCALE;
	  mot_set_pwm(axis, mot_axes[axis].pwm);
	}
      else if (mot_axes[axis].pwm)
	{
	  mot_axes[axis].pwm = 0;
	  mot_set_pwm(axis, 0);
	}
    }
}

static void mot_handle_write(void)
{
  unsigned int axis;
  unsigned int len;

  for (axis=0; axis<QEMOT_NUM_MOTORS; axis++)
    {
      // check trajectory queue for this axis
      if (atomic_read(&mot_axes[axis].used) &&
	  (len=kfifo_len(mot_axes[axis].write_queue))) 
	{
	  kfifo_get(mot_axes[axis].write_queue, 
		    (char *)&mot_axes[axis].desired_position, sizeof(long));
	  // wake up process, but only if we have a sufficient amount of space
	  // we don't want to wake up every single time
	  if (mot_write_queue_size-len>=(mot_write_queue_size>>1))
	    wake_up_interruptible(&mot_axes[axis].waitqueue);
	  //	  DPK("%d\n", mot_axes[axis].desired_position);
	  
	  mot_axes[axis].trajectory = TRUE;
	}
      else if (mot_axes[axis].trajectory &&
	       mot_axes[axis].timer++==MOT_TRAJ_TIMER)
	{
	  mot_axes[axis].trajectory = FALSE;
	  mot_axes[axis].timer = 0;
	  mot_axis_reset(axis);
	}
    }
}

static void mot_handle_read(void)
{
  unsigned int axis;
  char dummy[sizeof(long)];

  for (axis=0; axis<QEMOT_NUM_MOTORS; axis++)
    {
      // check trajectory queue for this axis
      if (atomic_read(&mot_axes[axis].used))
	{
	  // toss out old data
	  while (kfifo_len(mot_axes[axis].read_queue)>=mot_read_queue_size)
	    kfifo_get(mot_axes[axis].read_queue, dummy, sizeof(long));

	  // add new position data to queue
	  kfifo_put(mot_axes[axis].read_queue, 
		    (char *)&mot_axes[axis].position, sizeof(long));

	  // wake up process, but only if we have a sufficient amount of data
	  // we don't want to wake up every single time
	  if (kfifo_len(mot_axes[axis].read_queue)>=(mot_read_queue_size>>1))
	    wake_up_interruptible(&mot_axes[axis].waitqueue);
	}
    }
}

void mot_init(void *addr)
{
  unsigned int axis;

  DPK("init\n");
  mot_axes = kmalloc(QEMOT_NUM_MOTORS*sizeof(struct mot_axis_data), GFP_KERNEL);

  memset(mot_axes, 0, QEMOT_NUM_MOTORS*sizeof(struct mot_axis_data));

  for (axis=0; axis<QEMOT_NUM_MOTORS; axis++)
    {
      mot_axes[axis].cal_scale = 1<<MOT_CAL_SCALE;
      spin_lock_init(&mot_axes[axis].write_queue_lock);
      spin_lock_init(&mot_axes[axis].read_queue_lock);
      init_waitqueue_head(&mot_axes[axis].waitqueue);
    }

  mot_reg_pwm_control = (volatile unsigned short *)addr;
  mot_reg_status = (volatile unsigned short *)addr + 6;
  mot_reg_mcurrent_measurement = (volatile unsigned short *)addr + 256;
  mot_reg_bemf_measurement = (volatile short *)addr + 256 + QEMOT_NUM_MOTORS*MOT_MCURRENT_MEASUREMENTS;
  mot_reg_wait_interval = (volatile unsigned short *)addr + 5;
  mot_reg_timing = (volatile unsigned short *)addr + 4;
}

void mot_exit()
{
  DPK("exit\n");
  if (mot_axes)
    kfree(mot_axes);
}

void mot_periodic()
{
  mot_handle_bemf();
  mot_handle_current();
  mot_handle_write();
  mot_handle_read();
  mot_handle_pid_control();
}

int mot_set_period(unsigned int axis, unsigned int period)
{
  qe_interrupt_disable(QEMOT_VECTOR);

  *mot_reg_timing = (((MOT_MCURRENT_MEASUREMENTS-1)>>2)<<12) | 
    (((MOT_BEMF_MEASUREMENTS-1)>>2)<<8) | 
    (period&0xff); 

  // invalidate current measurement
  qe_motor_period_usec = -1;
  qe_motor_time_usec_prev = -1;

  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_get_frequency(unsigned int axis, unsigned int *frequency)
{
  unsigned int f;
  
  //DPK("mot_get_frequency\n");
  qe_interrupt_disable(QEMOT_VECTOR);
  if (qe_motor_period_usec>0)
    f = (1000000+(qe_motor_period_usec>>1))/qe_motor_period_usec;
  else 
    f = -1; 
  qe_interrupt_enable(QEMOT_VECTOR);

  copy_to_user(frequency, &f, sizeof(unsigned int));
  return 0;
}

int mot_get_status(unsigned int axis, unsigned short *status)
{
  unsigned short lstatus = *mot_reg_status;
  copy_to_user(status, (void *)&lstatus, sizeof(unsigned short));
  return 0;
}

int mot_set_mode(unsigned int axis, unsigned long mode)
{
  mot_axes[axis].mode = mode;
  return 0;
}

int mot_get_position(unsigned int axis, Axis_position *position)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  copy_to_user(position, &mot_axes[axis].position, sizeof(Axis_position));
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
 
}
             
int mot_get_position_vector(unsigned axis, Axis_position position[])
{
  unsigned int laxis;
 
  qe_interrupt_disable(QEMOT_VECTOR);
  for (laxis=0; laxis<QEMOT_NUM_MOTORS; laxis++)
    copy_to_user(&position[laxis], &mot_axes[laxis].position, sizeof(Axis_position));
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}
  
int mot_set_pwm(unsigned int axis, int pwm)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  if (pwm<0)
    {
      pwm = -pwm;
      if (pwm>MOT_MAX_PWM)
	pwm = MOT_MAX_PWM;
      mot_reg_pwm_control[axis] = 0x0200 | pwm;
    }
  else if (pwm>0)
    {
      if (pwm>MOT_MAX_PWM)
	pwm = MOT_MAX_PWM;
      mot_reg_pwm_control[axis] = 0x0100 | pwm;
    }
  else // pwm==0
    mot_reg_pwm_control[axis] = 0x0000;    

  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_get_pwm(unsigned int axis, int *pwm)
{
  int lpwm;

  qe_interrupt_disable(QEMOT_VECTOR);
  lpwm = mot_reg_pwm_control[axis]&0x00ff;
  if (mot_reg_pwm_control[axis]&0x0200)
    lpwm = -lpwm;

  copy_to_user(pwm, &lpwm, sizeof(int));
  qe_interrupt_enable(QEMOT_VECTOR);
  
  return 0;
}

int mot_set_bemf_offset(unsigned int axis, int offset)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  mot_axes[axis].offset = (short)offset;
  qe_interrupt_enable(QEMOT_VECTOR);
  
  return 0;
}

int mot_set_position(unsigned int axis, Axis_position pos)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  mot_axes[axis].hopper = (long long)pos << MOT_HOPPER_SCALE;
  mot_axes[axis].position = pos;
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_get_mcurrent(unsigned int axis, int *mcurrent)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  //mot_axes[axis].mcurrent = 65536;
  copy_to_user(mcurrent, &mot_axes[axis].mcurrent, sizeof(int));
  qe_interrupt_enable(QEMOT_VECTOR);
 
  return 0;
}

int mot_get_mcurrent_vector(unsigned int axis, int mcurrent[])
{
  unsigned int laxis;
 
  qe_interrupt_disable(QEMOT_VECTOR);
  for (laxis=0; laxis<QEMOT_NUM_MOTORS; laxis++)
    copy_to_user(&mcurrent[laxis], &mot_axes[laxis].mcurrent, sizeof(int));
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_get_velocity(unsigned int axis, long *velocity)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  copy_to_user(velocity, &mot_axes[axis].velocity, sizeof(long));
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_get_velocity_vector(unsigned int axis, long velocity[])
{
  unsigned int laxis;
 
  qe_interrupt_disable(QEMOT_VECTOR);
  for (laxis=0; laxis<QEMOT_NUM_MOTORS; laxis++)
    copy_to_user(&velocity[laxis], &mot_axes[laxis].velocity, sizeof(long));
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_set_bemf_calibrate(unsigned int axis, unsigned int cal)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  mot_axes[axis].cal_scale = cal;
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_set_bemf_timing(unsigned int axis, unsigned int wait_interval)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  *mot_reg_wait_interval &= ~0xff;
  *mot_reg_wait_interval |= (unsigned short)wait_interval;
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_set_pidv_gains(unsigned int axis, unsigned int p_gain, 
		      unsigned int i_gain, unsigned int d_gain, 
		      unsigned int v_gain)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  mot_axes[axis].p_gain = p_gain;
  mot_axes[axis].i_gain = i_gain;
  mot_axes[axis].d_gain = d_gain;
  mot_axes[axis].v_gain = v_gain;
  qe_interrupt_enable(QEMOT_VECTOR);
 
  return 0;
}

int mot_set_max_error(unsigned int axis, unsigned int error)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  mot_axes[axis].max_error = error;
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_set_max_integral(unsigned int axis, unsigned int integral)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  mot_axes[axis].max_integral = integral;
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_get_done(unsigned int axis, unsigned short *done)
{
  unsigned short ldone; 

  qe_interrupt_disable(QEMOT_VECTOR);
  ldone = !mot_axes[axis].trajectory;
  copy_to_user(done, &ldone, sizeof(unsigned short));
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_set_hold(unsigned int axis, unsigned int hold)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  // synchronize motors before holding position
  if (hold)
    mot_axes[axis].desired_position = mot_axes[axis].position;
  mot_axes[axis].hold = hold;
  mot_axis_reset(axis);
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_stop(unsigned int axis)
{
  qe_interrupt_disable(QEMOT_VECTOR);

  DPK("stop %d\n", axis);
  mot_flush_write_queue(axis);
  mot_axes[axis].trajectory = FALSE;
  mot_axis_reset(axis);
  mot_set_pwm(axis, 0);

  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

int mot_get_bemf(unsigned int axis, long *bemf)
{
  qe_interrupt_disable(QEMOT_VECTOR);
  copy_to_user(bemf, &mot_axes[axis].bemf, sizeof(long));
  qe_interrupt_enable(QEMOT_VECTOR);

  return 0;
}

void mot_axis_init(int axis)
{
  mot_axes[axis].write_queue = kfifo_alloc(MOT_QUEUE_SIZE, GFP_KERNEL, &mot_axes[axis].write_queue_lock);
  mot_axes[axis].read_queue = kfifo_alloc(MOT_QUEUE_SIZE, GFP_KERNEL, &mot_axes[axis].read_queue_lock);

  // we are active
  atomic_set(&mot_axes[axis].used, 1);
}

void mot_axis_exit(int axis)
{  
  // turn off motor
  mot_set_hold(axis, FALSE);
  mot_stop(axis);

  // we are inactive
  atomic_set(&mot_axes[axis].used, 0);

  // free queues
  kfifo_free(mot_axes[axis].write_queue);
  kfifo_free(mot_axes[axis].read_queue);
}

void mot_axis_reset(int axis)
{
  if (!mot_axes[axis].hold && !mot_axes[axis].trajectory)
    {
      mot_axes[axis].error_previous = 0;
      mot_axes[axis].error_integral = 0;
    }
}

int mot_axis_write(int axis, char *buf, unsigned int count)
{
  unsigned int result;

  count &= ~0x03;  // throw out two lsbs

  // reset flushed_write flag in case there is a residual flag set
  atomic_set(&mot_axes[axis].flushed_write, 0);

  // note, if the condition below evaluates to false, it will sleep
  if (wait_event_interruptible(mot_axes[axis].waitqueue, 
			       mot_write_queue_size-kfifo_len(mot_axes[axis].write_queue)>=count))
    return -ERESTARTSYS;

  // check for flushed_write flag -- abort write if flushed
  if (atomic_read(&mot_axes[axis].flushed_write))
    {
      atomic_set(&mot_axes[axis].flushed_write, 0);
      return 0;
    }
  else
    {
      result = kfifo_put(mot_axes[axis].write_queue, buf, count);
      return result;
    }
}

int mot_axis_read(int axis, char *buf, unsigned int count)
{
  count &= ~0x03;  // throw out two lsbs

  // note, if the condition below evaluates to false, it will sleep
  if (wait_event_interruptible(mot_axes[axis].waitqueue, 
			       kfifo_len(mot_axes[axis].read_queue)>=count))
    return -ERESTARTSYS;
      
  return kfifo_get(mot_axes[axis].read_queue, buf, count);
}

int mot_flush_write_queue(unsigned int axis)
{
  kfifo_reset(mot_axes[axis].write_queue);  
  
  // set flag so next write is aborted
  atomic_set(&mot_axes[axis].flushed_write, 1);

  // wake up write thread if blocked
  wake_up_interruptible(&mot_axes[axis].waitqueue);

  return 0;
}

int mot_set_write_queue_size(unsigned int axis, unsigned int size)
{
  size &= ~0x03;  // throw out two lsbs
  mot_write_queue_size = size;
  return 0;
}

int mot_set_read_queue_size(unsigned int axis, unsigned int size)
{
  size &= ~0x03;  // throw out two lsbs
  mot_read_queue_size = size;
  return 0;
}
