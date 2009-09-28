#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/device.h>

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/kdev_t.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/wait.h>
#include <linux/cdev.h>
#include <linux/kfifo.h>
#include <asm/io.h>
#include <linux/time.h>
#include "../qeinterrupt/qeinterrupt.h"
#include "../../../src/libqwerk/qemotor.h"
#include "motor.h"

MODULE_LICENSE("Dual BSD/GPL");

#if 1 // debug
#define DPK(format, ...) printk(KERN_NOTICE "qe_motor: " format, ## __VA_ARGS__)
#else
#define DPK(format, ...)
#endif

#define PK(format, ...) printk(KERN_ALERT "qe_motor: " format, ## __VA_ARGS__)


struct qe_motor_data
{
  unsigned char axis;
  unsigned char used;    // is interrupt being used by a process?
  unsigned char mode;
  struct cdev cdev;
};

extern unsigned int mot_write_queue_size;
extern unsigned int mot_read_queue_size;

static void *io_base = 0;

static int qe_motor_open(struct inode *inode, struct file *filp);
static int qe_motor_release(struct inode *inode, struct file *filp);
static ssize_t qe_motor_read (struct file *filp, char __user *buf, size_t count, loff_t *pos);
static ssize_t qe_motor_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos);
static int qe_motor_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg);
static void qe_motor_callback(unsigned char vector);
static int qe_motor_call(unsigned int index, unsigned int, long *args);
static struct qe_motor_data *qe_motors = 0;
static struct class *qemot_class;

int qe_motor_time_usec_prev = -1;
int qe_motor_period_usec = -1; 

struct file_operations qe_motor_fops = 
{
  .owner = THIS_MODULE,
  .open = qe_motor_open,
  .release = qe_motor_release,
  .read =  qe_motor_read,
  .write = qe_motor_write,
  .ioctl = qe_motor_ioctl
};

static qe_motor_func qe_mot_funcs[] =
{
  /* 0  */ (qe_motor_func)mot_set_period,
  /* 1  */ (qe_motor_func)mot_get_frequency,
  /* 2  */ (qe_motor_func)mot_get_status,
  /* 3  */ (qe_motor_func)mot_set_mode,
  /* 4  */ (qe_motor_func)mot_get_position,
  /* 5  */ (qe_motor_func)mot_get_position_vector,
  /* 6  */ (qe_motor_func)mot_set_pwm,
  /* 7  */ (qe_motor_func)mot_get_pwm,
  /* 8  */ (qe_motor_func)mot_set_bemf_offset,
  /* 9  */ (qe_motor_func)mot_set_position,
  /* 10 */ (qe_motor_func)mot_get_mcurrent,
  /* 11 */ (qe_motor_func)mot_get_mcurrent_vector,
  /* 12 */ (qe_motor_func)mot_get_velocity,
  /* 13 */ (qe_motor_func)mot_get_velocity_vector,
  /* 14 */ (qe_motor_func)mot_set_bemf_calibrate,
  /* 15 */ (qe_motor_func)mot_set_bemf_timing,
  /* 16 */ (qe_motor_func)mot_set_pidv_gains,
  /* 17 */ (qe_motor_func)mot_set_max_error,
  /* 18 */ (qe_motor_func)mot_set_max_integral,
  /* 19 */ (qe_motor_func)mot_get_done,
  /* 20 */ (qe_motor_func)mot_set_hold,
  /* 21 */ (qe_motor_func)mot_get_bemf,
  /* 22 */ (qe_motor_func)mot_flush_write_queue,
  /* 23 */ (qe_motor_func)mot_set_write_queue_size,
  /* 24 */ (qe_motor_func)mot_stop,
  /* 25 */ (qe_motor_func)mot_set_read_queue_size
};


static int qe_motor_call(unsigned int index, unsigned int axis, long *args)
{
  long kargs[QEMOT_NUM_ARGS];

  copy_from_user(kargs, args, QEMOT_NUM_ARGS*sizeof(long));
  if (index<sizeof(qe_mot_funcs)/sizeof(void *))
    return (*qe_mot_funcs[index])(axis, kargs[0], kargs[1], 
				  kargs[2], kargs[3], kargs[4]);
  else
    // not a valid function
    return -EINVAL;
}

static void qe_motor_callback(unsigned char vector)
{
  struct timeval tv;

  qe_interrupt_time(vector, &tv);
  if (qe_motor_time_usec_prev>0)
    {
      if (qe_motor_time_usec_prev>tv.tv_usec)
	qe_motor_period_usec = 1000000 - qe_motor_time_usec_prev + tv.tv_usec;
      else
	qe_motor_period_usec = tv.tv_usec - qe_motor_time_usec_prev;
    }

  mot_periodic();

  qe_motor_time_usec_prev = tv.tv_usec;
}

static int __init qe_motor_init(void)
{
  int retval, i;
  dev_t dev;

  DPK("init\n");

  dev = MKDEV(QEMOT_MAJOR_NUMBER, QEMOT_MINOR_NUMBER);
  retval = register_chrdev_region(dev, QEMOT_NUM_MOTORS, "qe_motor");
  
  if (retval<0)
    {
      PK("unable to register");
      return retval;
    }

  qemot_class = class_create(THIS_MODULE, "qemot");
  if (IS_ERR(qemot_class))
    {
      PK("unable to create qemot class");
      qemot_class = NULL;
      goto fail_mem;
    }
  PK("created qemot class");

  retval = (int)request_mem_region(QEMOT_BASE_ADDR, QEMOT_WIDTH, "qemot_ports");
  if (retval)
    io_base = ioremap(QEMOT_BASE_ADDR, QEMOT_WIDTH);
  else
    {
      PK("memory request failed\n");
      goto fail_mem;
    }
  
  retval = qe_interrupt_register(QEMOT_VECTOR, QEINT_MODE_FAST, qe_motor_callback);
  if (retval)
    {
      PK("interrupt already taken\n");
      goto fail_irq;
    }

  qe_motors = kmalloc(QEMOT_NUM_MOTORS*sizeof(struct qe_motor_data), GFP_KERNEL);
  if (qe_motors==NULL)
    {
      PK("out of memory\n");
      retval = -ENOMEM;
      goto fail_malloc;
    }

  memset(qe_motors, 0, QEMOT_NUM_MOTORS*sizeof(struct qe_motor_data));

  for (i=0; i<QEMOT_NUM_MOTORS; i++)
    {
      cdev_init(&qe_motors[i].cdev, &qe_motor_fops);
      qe_motors[i].axis = i;
      qe_motors[i].cdev.owner = THIS_MODULE;
      qe_motors[i].cdev.ops = &qe_motor_fops;
      dev = MKDEV(QEMOT_MAJOR_NUMBER, QEMOT_MINOR_NUMBER + i);
      retval = cdev_add(&qe_motors[i].cdev, dev, 1);
      if (retval)
	PK("error adding device %d\n", i);
      device_create(qemot_class, NULL, dev,
		    NULL, "qemot%d", QEMOT_MINOR_NUMBER + i);
    }

  // set enable bit in H-bridges
  retval = (int)request_mem_region(0x80930000, 0xc4, "syscon ports");
  if (retval)
    {
      volatile unsigned long *syscon = ioremap(0x80930000, 0xc4);
      syscon[0x30] = 0xaa;
      syscon[0x20] |= 0x08000800;
      iounmap((void *)syscon);
      release_mem_region(0x80930000, 0xc4);
    }
  
  mot_init(io_base);
  // enable interrupts
  qe_interrupt_enable(QEMOT_VECTOR);
  // success
  return 0;

 fail_malloc:
  qe_interrupt_unregister(QEMOT_VECTOR);
 fail_irq:
  release_mem_region(QEMOT_BASE_ADDR, QEMOT_WIDTH);
 fail_mem:
  dev = MKDEV(QEMOT_MAJOR_NUMBER, QEMOT_MINOR_NUMBER);
  unregister_chrdev_region(dev, QEMOT_NUM_MOTORS);
  if (qemot_class)
    {
      class_destroy(qemot_class);
      qemot_class = NULL; /* FIXME: not necessary, probably */
    }
  
  return retval;
}

static void __exit qe_motor_exit(void)
{
  int i;
  dev_t dev;

  DPK("exit\n");
  qe_interrupt_unregister(QEMOT_VECTOR);

  mot_exit();
  if (io_base)
    {
      iounmap(io_base);
      release_mem_region(QEMOT_BASE_ADDR, QEMOT_WIDTH);
    }

  if (qe_motors)
    {
      for (i=0; i<QEMOT_NUM_MOTORS; i++)
	{
	  cdev_del(&qe_motors[i].cdev);
	  device_destroy(qemot_class, MKDEV(QEMOT_MAJOR_NUMBER, QEMOT_MINOR_NUMBER + i));
	}
      kfree(qe_motors);
    }

  if (qemot_class)
    {
      class_destroy(qemot_class);
      qemot_class = NULL;
    }

  dev = MKDEV(QEMOT_MAJOR_NUMBER, QEMOT_MINOR_NUMBER);
  unregister_chrdev_region(dev, QEMOT_NUM_MOTORS);

}

static int qe_motor_open(struct inode *inode, struct file *filp)
{
  struct qe_motor_data *data;
  DPK("open\n");
  data = container_of(inode->i_cdev, struct qe_motor_data, cdev);
  DPK("data %x\n", (unsigned int)data);
  filp->private_data = data;

  // check to see if motor is already being used
  if (data->used)
    return -EBUSY;
  data->used = TRUE; // if not, set to true

  DPK("minor %x\n", data->axis);
  mot_axis_init(data->axis);
  //DPK("%d %d\n", imajor(inode), iminor(inode));
  return 0;
}

static int qe_motor_release(struct inode *inode, struct file *filp)
{
  struct qe_motor_data *data;
  DPK("release\n");
  DPK("private_data %x\n", (unsigned int)filp->private_data);

  data = (struct qe_motor_data *)filp->private_data;

  mot_axis_exit(data->axis);

  data->used = FALSE;

  DPK("minor %d\n", data->axis);
  
  return 0;
}

static ssize_t qe_motor_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
  char lbuf[mot_read_queue_size];
  struct qe_motor_data *data;
  unsigned int lcount, tcount;
  int result;

  //DPK("read %d\n", count);

  data = (struct qe_motor_data *)filp->private_data;

  for (lcount=count; lcount; lcount-=tcount)
    {
      tcount = lcount<mot_read_queue_size ? lcount : mot_read_queue_size;
      if ((result=mot_axis_read(data->axis, lbuf, tcount))<0)
	return result;
      copy_to_user(buf+count-lcount, lbuf, tcount);
    } 

  return count; 
}

static ssize_t qe_motor_write(struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
  char lbuf[mot_write_queue_size];
  struct qe_motor_data *data;
  int result;
  unsigned int lcount, tcount;

  // DPK("write %x %d\n", buf, count);

  data = (struct qe_motor_data *)filp->private_data;

  for (lcount=count; lcount; lcount-=tcount)
    {
      tcount = lcount<mot_write_queue_size ? lcount : mot_write_queue_size;
      copy_from_user(lbuf, buf+count-lcount, tcount);
      //      DPK("%d %d %d %x %x %x\n", count, tcount, lcount, ((int *)lbuf)[0], ((int *)lbuf)[1], ((int *)lbuf)[2]);
      if ((result=mot_axis_write(data->axis, lbuf, tcount))<0)
	return result;
    } 

  return count;
}

static int qe_motor_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
  struct qe_motor_data *data;
  data = (struct qe_motor_data *)filp->private_data;

  //DPK("ioctl %d %d\n", cmd, arg);
  return qe_motor_call(cmd, data->axis, (long *)arg);
}


module_init(qe_motor_init);
module_exit(qe_motor_exit);
