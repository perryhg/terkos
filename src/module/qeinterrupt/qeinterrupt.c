#include <linux/autoconf.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

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
#include <asm/io.h>
#include <linux/time.h>

#include "qeinterrupt.h"


MODULE_LICENSE("Dual BSD/GPL");

#define QEINT_INT_MASK     io_base
#define QEINT_INT_STATUS   (io_base+2)
#if 1 // debug
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
  unsigned char vector; // interrupt number (0 through 15)
  unsigned char used;    // is interrupt being used by a process?
  unsigned char mode;
  void (*callback)(unsigned char);
  struct fasync_struct *async_queue;
  struct cdev cdev;
  struct timeval tv;
};


static void *io_base = 0;


static int qe_interrupt_open(struct inode *inode, struct file *filp);
static int qe_interrupt_release(struct inode *inode, struct file *filp);
static ssize_t qe_interrupt_read (struct file *filp, char __user *buf, size_t count, loff_t *pos);
static ssize_t qe_interrupt_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos);
static int qe_interrupt_ioctl(struct inode *inode, struct file *filp,
		unsigned int cmd, unsigned long arg);
static int qe_interrupt_fasync(int fd, struct file *filp, int mode);
static struct qe_interrupt_data *qe_interrupts = 0;


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
  writew(1<<vector | readw(QEINT_INT_MASK), QEINT_INT_MASK);	      
  return 0;
}

int qe_interrupt_disable(unsigned char vector)
{
  if (!qe_interrupts[vector].used)
    return -EACCES;

  if (vector>=QEINT_NUM_INTERRUPTS)
    return -EINVAL;

  //DPK("disable %d\n", vector);
  writew(1<<vector & ~readw(QEINT_INT_MASK), QEINT_INT_MASK);	      
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
  qe_interrupt_disable(vector);
  *ptv = qe_interrupts[vector].tv; 
  qe_interrupt_enable(vector);
}

static irqreturn_t
qe_interrupt_callback(int irq, void *desc)
{
  unsigned char i = 0;
  unsigned short mask;
  unsigned short status;

  while((status=readw(QEINT_INT_STATUS)))
    {
      for (i=0; i<QEINT_NUM_INTERRUPTS; i++)
	{
	  mask = 1<<i;
	  if (status&mask)
	    {
	      if (qe_interrupts[i].mode!=QEINT_MODE_UNUSED)
		{
		  // timestamp interrupt
		  do_gettimeofday(&qe_interrupts[i].tv);
		  // signal processes that need signalling
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
	      writew(mask, QEINT_INT_STATUS);	      
	    }
	}
    }

  return IRQ_HANDLED;
}

static int qe_interrupt_init(void)
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
    }

  // disable all interrupts 
  writew(0, QEINT_INT_MASK);

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
  
  return retval;
}

static void qe_interrupt_exit(void)
{
  int i;
  dev_t dev;

  DPK("exit\n");

  // disable all interrupts 
  writew(0, QEINT_INT_MASK);

  free_irq(QEINT_INTERRUPT, NULL);

  if (io_base)
    {
      iounmap(io_base);
      release_mem_region(QEINT_BASE_ADDR, QEINT_WIDTH);
    }

  dev = MKDEV(QEINT_MAJOR_NUMBER, QEINT_MINOR_NUMBER);
  unregister_chrdev_region(dev, QEINT_NUM_INTERRUPTS);

  if (qe_interrupts)
    {
      for (i=0; i<QEINT_NUM_INTERRUPTS; i++)
	cdev_del(&qe_interrupts[i].cdev);
      kfree(qe_interrupts);
    }
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
  data->used = TRUE; // if not, set to true

  DPK("minor %x\n", data->vector);
  //DPK("%d %d\n", imajor(inode), iminor(inode));
  return 0;
}

static int qe_interrupt_release(struct inode *inode, struct file *filp)
{
  struct qe_interrupt_data *data;
  DPK("release\n");
  DPK("private_data %x\n", (int)filp->private_data);

  data = (struct qe_interrupt_data *)filp->private_data;

  // interrupt is no longer being used
  data->used = FALSE;

  DPK("minor %d\n", data->vector);
  qe_interrupt_fasync(-1, filp, 0);

  return 0;
}

static ssize_t qe_interrupt_read (struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
  DPK("read %d %x\n", iminor(filp->f_dentry->d_inode), (int)buf);
  return 0; 
}

static ssize_t qe_interrupt_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
  DPK("write %x\n", (int)buf);
  return 0;
}

static int qe_interrupt_ioctl(struct inode *inode, struct file *filp,
                 unsigned int cmd, unsigned long arg)
{
  DPK("ioctl %d %d\n", cmd, (int)arg);
  return 0;
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