//
// begin license header
//  
// This file is part of Terk and TerkOS.
//
// All Terk and TerkOS source code is provided under the terms of the 
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).  
// Those wishing to use Terk and TerkOS source code, software and/or
// technologies under different licensing terms should contact us at 
// telepresencerobotkit@cs.cmu.edu. Such licensing terms are available for
// all portions of the Terk and TerkOS codebase presented here.
//
// end license header
//

/* -*- c-file-style: "gnu" -*- */
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
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/time.h>

#include "../../../src/libqwerk/qeinterrupt.h"


MODULE_LICENSE("Dual BSD/GPL");

#define QEINT_INT_MASK     io_base
#define QEINT_INT_STATUS   (io_base+4)
#if 0 // debug
#define DPK(format, ...) printk(KERN_NOTICE "qe_interrupt: " format, ## __VA_ARGS__)
#else
#define DPK(format, ...)
#endif

#define PK(format, ...) printk(KERN_ALERT "qe_interrupt: " format, ## __VA_ARGS__)


EXPORT_SYMBOL(qe_interrupt_enable);
EXPORT_SYMBOL(qe_interrupt_disable);
EXPORT_SYMBOL(qe_interrupt_register);
EXPORT_SYMBOL(qe_interrupt_unregister);
EXPORT_SYMBOL(qe_interrupt_time);

struct qe_interrupt_data
{
  unsigned char vector; // interrupt number (0 through 31)
  unsigned char used;    // is interrupt being used by a process?
  unsigned char mode;
  void (*callback)(unsigned char);
  struct fasync_struct *async_queue;
  struct cdev cdev;
  struct timeval tv;
};


static void *io_base = 0;
static unsigned int signal_status = 0;

static int qe_interrupt_open(struct inode *inode, struct file *filp);
static int qe_interrupt_release(struct inode *inode, struct file *filp);
static ssize_t qe_interrupt_read (struct file *filp, char __user *buf, size_t count, loff_t *pos);
static ssize_t qe_interrupt_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos);
static int qe_interrupt_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg);
static int qe_interrupt_fasync(int fd, struct file *filp, int mode);
static struct qe_interrupt_data *qe_interrupts = 0;
static struct class *qeint_class;

struct file_operations qe_interrupt_fops = 
{
  .owner = THIS_MODULE,
  .open = qe_interrupt_open,
  .release = qe_interrupt_release,
  .read =  qe_interrupt_read,
  .write = qe_interrupt_write,
  .ioctl = qe_interrupt_ioctl,
  .fasync = qe_interrupt_fasync
};


int qe_interrupt_enable(unsigned char vector)
{
  if (vector>=QEINT_NUM_INTERRUPTS)
    return -EINVAL;

  if (!qe_interrupts[vector].used)
    return -EACCES;

  //DPK("enable %d\n", vector);
  writel((1<<(unsigned int)vector) | readl(QEINT_INT_MASK), QEINT_INT_MASK);	      
  return 0;
}

int qe_interrupt_disable(unsigned char vector)
{
  if (!qe_interrupts[vector].used)
    return -EACCES;

  if (vector>=QEINT_NUM_INTERRUPTS)
    return -EINVAL;

  //DPK("disable %d\n", vector);
  writel(~(1<<(unsigned int)vector) & readl(QEINT_INT_MASK), QEINT_INT_MASK);	      
  return 0;
}

int qe_interrupt_register(unsigned char vector, unsigned char mode, void (*callback)(unsigned char))
{
  if (vector>=QEINT_NUM_INTERRUPTS)
    return -EINVAL;

  if (qe_interrupts[vector].used)
    return -EBUSY;

  DPK("register %d\n", vector);

  qe_interrupts[vector].used = TRUE;
  qe_interrupts[vector].mode = mode;
  qe_interrupts[vector].callback = callback;
  return 0;
}

int qe_interrupt_unregister(int vector)
{
  if (vector>=QEINT_NUM_INTERRUPTS)
    return -EINVAL;

  if (!qe_interrupts[vector].used)
    return -EACCES;

  DPK("unregister %d\n", vector);

  qe_interrupt_disable(vector);
  qe_interrupts[vector].used = FALSE;
  qe_interrupts[vector].mode = QEINT_MODE_UNUSED;
  qe_interrupts[vector].callback = NULL;
  return 0;
}

void qe_interrupt_time(int vector, struct timeval *ptv)
{
  unsigned long save;

  // save interrupt mask and disable
  save = readl(QEINT_INT_MASK);
  qe_interrupt_disable(vector);

  *ptv = qe_interrupts[vector].tv; 

  // restore interrupt mask
  writel(save, QEINT_INT_MASK);  
}

static irqreturn_t
qe_interrupt_callback(int irq, void *desc)
{
  unsigned char i = 0;
  unsigned long mask;
  unsigned long status;
  struct timeval tv;
  do_gettimeofday(&tv);

  while((status=readl(QEINT_INT_STATUS)))
    {
     for (i=0; i<QEINT_NUM_INTERRUPTS; i++)
	{
	  mask = 1<<i;
	  if (status&mask)
	    {
	      if (qe_interrupts[i].mode!=QEINT_MODE_UNUSED)
		{
		  // timestamp interrupt
		  qe_interrupts[i].tv = tv;
		  // signal processes that need signalling
		  signal_status |= mask;
		  if (qe_interrupts[i].async_queue)
		    kill_fasync(&qe_interrupts[i].async_queue, SIGIO, POLL_IN);
		  // callback modules that need callbacking
		  if (qe_interrupts[i].callback)
		    {
		      if (qe_interrupts[i].mode==QEINT_MODE_FAST)
			(*qe_interrupts[i].callback)(i);
		      // else // deferred
		    }
		}
	      // clear interrupt
	      writel(mask, QEINT_INT_STATUS);	      
	    }
	}
    }

  return IRQ_HANDLED;
}

static int __init qe_interrupt_init(void)
{
  int retval, i;
  dev_t dev;

  DPK("init\n");

  dev = MKDEV(QEINT_MAJOR_NUMBER, QEINT_MINOR_NUMBER);
  retval = register_chrdev_region(dev, QEINT_NUM_INTERRUPTS, "qe_interrupt");
  
  if (retval<0)
    {
      PK("unable to register");
      return retval;
    }

  qeint_class = class_create(THIS_MODULE, "qeint");
  if (IS_ERR(qeint_class))
    {
      DPK("unable to create qeint class");
      qeint_class = NULL;
      goto fail_mem;
    }
  DPK("created qeint class\n");

  // set chip select timing for FPGA
  retval = (int)request_mem_region(0x80080000, 0xc, "scr ports");
  if (retval)
    {
      volatile unsigned long *scr = ioremap(0x80080000, 0xc);
      scr[2] = 0x10000440;
      iounmap(scr);
      release_mem_region(0x80080000, 0xc);
    }

  retval = (int)request_mem_region(QEINT_BASE_ADDR, QEINT_WIDTH, "qe_interrupt");
  if (retval)
    io_base = ioremap(QEINT_BASE_ADDR, QEINT_WIDTH);
  else
    {
      PK("memory request failed\n");
      goto fail_mem;
    }
  
  qe_interrupts = kmalloc(QEINT_NUM_INTERRUPTS*sizeof(struct qe_interrupt_data), GFP_KERNEL);
  if (qe_interrupts==NULL)
    {
      PK("out of memory\n");
      retval = -ENOMEM;
      goto fail_malloc;
    }
  
  memset(qe_interrupts, 0, QEINT_NUM_INTERRUPTS*sizeof(struct qe_interrupt_data));
  for (i=0; i<QEINT_NUM_INTERRUPTS; i++)
    {
      qe_interrupts[i].vector = i;
      cdev_init(&qe_interrupts[i].cdev, &qe_interrupt_fops);
      qe_interrupts[i].cdev.owner = THIS_MODULE;
      qe_interrupts[i].cdev.ops = &qe_interrupt_fops;
      dev = MKDEV(QEINT_MAJOR_NUMBER, QEINT_MINOR_NUMBER + i);
      retval = cdev_add(&qe_interrupts[i].cdev, dev, 1);
      if (retval)
	PK("error adding device %d\n", i);
      device_create(qeint_class, NULL, dev,
		    NULL, "qeint%d", QEINT_MINOR_NUMBER + i);
    }

  // disable all interrupts 
  writel(0, QEINT_INT_MASK);

  // enable main QwerkEngine interrupt
  retval = request_irq(QEINT_INTERRUPT, qe_interrupt_callback, 0, "qe_interrupt", NULL);
  if (retval<0)
    {
      PK("unable to request interrupt\n");
      goto fail_irq;
    }  

  // success
  return 0;

 fail_irq:
  kfree(qe_interrupts);
 fail_malloc:
  release_mem_region(QEINT_BASE_ADDR, QEINT_WIDTH);
 fail_mem:
  dev = MKDEV(QEINT_MAJOR_NUMBER, QEINT_MINOR_NUMBER);
  unregister_chrdev_region(dev, QEINT_NUM_INTERRUPTS);
  if (qeint_class)
    {
      class_destroy(qeint_class);
      qeint_class = NULL; /* FIXME: not necessary, probably */
    }
  
  return retval;
}

static void __exit qe_interrupt_exit(void)
{
  int i;
  dev_t dev;

  DPK("exit\n");

  // disable all interrupts 
  writel(0, QEINT_INT_MASK);

  free_irq(QEINT_INTERRUPT, NULL);

  if (io_base)
    {
      iounmap(io_base);
      release_mem_region(QEINT_BASE_ADDR, QEINT_WIDTH);
    }

  if (qe_interrupts)
    {
      for (i=0; i<QEINT_NUM_INTERRUPTS; i++)
	{
	  cdev_del(&qe_interrupts[i].cdev);
	  device_destroy(qeint_class, MKDEV(QEINT_MAJOR_NUMBER, QEINT_MINOR_NUMBER + i));
	}
      kfree(qe_interrupts);
    }

  if (qeint_class)
    {
      class_destroy(qeint_class);
      qeint_class = NULL;
    }

  dev = MKDEV(QEINT_MAJOR_NUMBER, QEINT_MINOR_NUMBER);
  unregister_chrdev_region(dev, QEINT_NUM_INTERRUPTS);
}

static int qe_interrupt_open(struct inode *inode, struct file *filp)
{
  struct qe_interrupt_data *data;
  DPK("open %d\n", iminor(inode));
  data = container_of(inode->i_cdev, struct qe_interrupt_data, cdev);
  DPK("data %x\n", (int)data);
  filp->private_data = data;


  // check to see if interrupt is already being used
  if (data->used)
    return -EBUSY;

  // reset signal status
  signal_status &= ~(1<<(unsigned int)data->vector);
		  
  data->used = TRUE; // if not, set to true
  data->mode = QEINT_MODE_FAST;
  qe_interrupt_enable(data->vector);

  DPK("minor %d\n", data->vector);
  //DPK("%d %d\n", imajor(inode), iminor(inode));
  return 0;
}

static int qe_interrupt_release(struct inode *inode, struct file *filp)
{
  struct qe_interrupt_data *data;

  DPK("release\n");
  DPK("private_data %x\n", (int)filp->private_data);

  data = (struct qe_interrupt_data *)filp->private_data;

  qe_interrupt_disable(data->vector);

  // interrupt is no longer being used
  data->used = FALSE;

  DPK("minor %d\n", data->vector);
  qe_interrupt_fasync(-1, filp, 0);

  return 0;
}

static ssize_t qe_interrupt_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
  struct qe_interrupt_data *data;
  struct timeval tv;
  int n;

  data = (struct qe_interrupt_data *)filp->private_data;

  DPK("read %d %x\n", iminor(filp->f_dentry->d_inode), (int)buf);

  qe_interrupt_time(data->vector, &tv);
  n = count>sizeof(tv) ? sizeof(tv) : count;
  copy_to_user(buf, (char *)&tv, n);

  return n;
}

static ssize_t qe_interrupt_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
  DPK("write %x\n", (int)buf);
  return 0;
}

static int qe_interrupt_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
  struct qe_interrupt_data *data;
  int err = 0, ret = 0;
  unsigned char status;

  DPK("ioctl %x %x\n", cmd, (int)arg);

  data = (struct qe_interrupt_data *)filp->private_data;

  /* don't even decode wrong cmds: better returning  ENOTTY than EFAULT */
  if (_IOC_TYPE(cmd) != QEINT_IOC_MAGIC) 
    return -ENOTTY;
  if (_IOC_NR(cmd) > QEINT_IOC_MAXNR) 
    return -ENOTTY;

  /*
   * the type is a bitmask, and VERIFY_WRITE catches R/W
   * transfers. Note that the type is user-oriented, while
   * verify_area is kernel-oriented, so the concept of "read" and
   * "write" is reversed
   */
  if (_IOC_DIR(cmd) & _IOC_READ)
    err = !access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd));
  else if (_IOC_DIR(cmd) & _IOC_WRITE)
    err =  !access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd));

  if (err)
    return -EFAULT;

  switch(cmd) 
    {
    case QEINT_IOC_READ_STATUS:
      DPK("read status %d\n", data->vector);
      status = signal_status & (1<<(unsigned int)data->vector) ? 1 : 0;
      ret = __put_user(status, (char __user *) arg);
      break;

    case QEINT_IOC_RESET_STATUS:
      DPK("reset status %d\n", data->vector);
      signal_status &= ~(1<<(unsigned int)data->vector);
      break;

    case QEINT_IOC_DISABLE:
      DPK("disable %d\n", data->vector);
      return qe_interrupt_disable(data->vector);

    case QEINT_IOC_ENABLE:
      DPK("enable %d\n", data->vector);
      return qe_interrupt_enable(data->vector);

    default:  /* redundant, as cmd was checked against MAXNR */
      return -ENOTTY;
    }
  
  return ret;
}

static int qe_interrupt_fasync(int fd, struct file *filp, int mode)
{
  struct qe_interrupt_data *data;

  DPK("fasync\n");
  data = (struct qe_interrupt_data *)filp->private_data;
  // if (filp->f_dentry)
  //  DPK("inode %x\n", filp->f_dentry->d_inode);
  // if (filp->f_dentry->d_inode)
  //  DPK("dev %x\n", filp->f_dentry->d_inode->i_rdev);


  return fasync_helper(fd, filp, mode, &data->async_queue);
}

module_init(qe_interrupt_init);
module_exit(qe_interrupt_exit);
