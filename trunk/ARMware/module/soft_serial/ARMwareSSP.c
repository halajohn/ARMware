/* ARMware - an ARM emulator
 * Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/module.h>
#include <linux/config.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/poll.h>

#include "ARMwareSSP.h"

static struct file_operations fops =
{
  .read = ARMware_SSP_read,
  .write = ARMware_SSP_write,
  .open = ARMware_SSP_open,
  .release = ARMware_SSP_release,
#if DEBUG
  .llseek = ARMware_SSP_lseek,
  .readdir = ARMware_SSP_readdir,
  .poll = ARMware_SSP_poll,
  .ioctl = ARMware_SSP_ioctl,
  .mmap = ARMware_SSP_mmap,
  .flush = ARMware_SSP_flush,
  .fsync = ARMware_SSP_fsync,
  .fasync = ARMware_SSP_fasync,
  .lock = ARMware_SSP_lock,
  .readv = ARMware_SSP_readv,
  .writev = ARMware_SSP_writev,
#endif
};

static int major_num;
static struct class_simple *ARMware_SSP_class;

static ARMwareSSPNode node[2];
static CircularBuffer pipe[2];

static int __init
ARMware_SSP_init(void)
{
  int err = 0;
  
  // :NOTE: Wei 2005-May-27:
  //
  // If you pass a major number of 0 to register_chrdev,
  // the return value will be the dynamically allocated major number.
  major_num = register_chrdev(0, ARMWARE_SSP_DEV_NAME, &fops);
  
  // :NOTE: Wei 2005-May-27:
  //
  // Negative return value means the registertration failed.
  if (major_num < 0)
  {
    printk(KERN_DEBUG "Registering ARMware_SSP device driver failed with %d\n", major_num);
    
    err = major_num;
    
    goto register_failed;
  }
  else
  {
    // :NOTE: Wei 2005-May-27:
    //
    // I succesively register a character device,
    // then create the corresponding device file now.
    ARMware_SSP_class = class_simple_create(THIS_MODULE, ARMWARE_SSP_CLASS_NAME);
    
    if (IS_ERR(ARMware_SSP_class))
    {
      printk(KERN_DEBUG "class simple create failed.\n");
      
      err = PTR_ERR(ARMware_SSP_class);
      
      goto class_simple_create_failed;
    }
    
    err = cb_init(pipe);
    
    if (err != 0)
    {
      printk(KERN_DEBUG "circular buffer init failed.\n");
      
      goto circular_buffer_init_failed;
    }
    
    class_simple_device_add(ARMware_SSP_class,
                            MKDEV(major_num, MINOR_NUM_0),
                            NULL,
                            DEV_FILE_NAME_0);
    
    class_simple_device_add(ARMware_SSP_class,
                            MKDEV(major_num, MINOR_NUM_1),
                            NULL,
                            DEV_FILE_NAME_1);
    
    atomic_set(&(node[0].device_opened), -1);
    node[0].read_pipe = &pipe[0];
    node[0].write_pipe = &pipe[1];
    
    atomic_set(&(node[1].device_opened), -1);
    node[1].read_pipe = &pipe[1];
    node[1].write_pipe = &pipe[0];
    
    goto done;
  }
  
 circular_buffer_init_failed:
  
  class_simple_destroy(ARMware_SSP_class);
  
 class_simple_create_failed:
  
  unregister_chrdev(major_num, ARMWARE_SSP_DEV_NAME);
  
 register_failed:
 done:
  
  return err;
}

static void __exit
ARMware_SSP_exit(void)
{
  int ret;
  
  class_simple_device_remove(MKDEV(major_num, MINOR_NUM_0));
  class_simple_device_remove(MKDEV(major_num, MINOR_NUM_1));
  
  class_simple_destroy(ARMware_SSP_class);
  
  cb_exit(pipe);
  
  ret = unregister_chrdev(major_num, ARMWARE_SSP_DEV_NAME);
  
  if (ret < 0)
  {
    printk(KERN_DEBUG "Error in unregister_chrdev: %d\n", ret);
  }
}

static int
ARMware_SSP_open(struct inode *inode, struct file *file)
{
#if DEBUG
  printk(KERN_DEBUG "ARMware_SSP_open.\n");
#endif
  
  if (0 == atomic_inc_and_test(&(node[0].device_opened)))
  {
    atomic_dec(&(node[0].device_opened));
    
    if (0 == atomic_inc_and_test(&(node[1].device_opened)))
    {
      printk(KERN_DEBUG "both nodes are used.\n");
      
      atomic_dec(&(node[1].device_opened));
      
      return -EBUSY;
    }
    else
    {
      // :NOTE: Wei 2005-May-28:
      //
      // node[1]
      printk(KERN_DEBUG "get node[1].\n");
      
      file->private_data = (void *)&(node[1]);
      
      try_module_get(THIS_MODULE);
      
      return 0;
    }
  }
  else
  {
    // :NOTE: Wei 2005-May-28:
    //
    // node[0]
    printk(KERN_DEBUG "get node[0].\n");
    
    file->private_data = (void *)&(node[0]);
    
    try_module_get(THIS_MODULE);
    
    return 0;
  }
}

static int
ARMware_SSP_release(struct inode *inode, struct file *file)
{
  ARMwareSSPNode * const node = (ARMwareSSPNode *)file->private_data;
  
#if DEBUG
  printk(KERN_DEBUG "ARMware_SSP_release.\n");
#endif
  
  atomic_dec(&(node->device_opened));
  
  module_put(THIS_MODULE);
  
  return 0;
}

static ssize_t
ARMware_SSP_read(struct file *file,
                 char *buffer,
                 size_t length,
                 loff_t *offset)
{
  ARMwareSSPNode * const node = (ARMwareSSPNode *)file->private_data;
  
#if DEBUG
  printk(KERN_DEBUG "ARMware_SSP_read.\n");
#endif
  
  while (1)
  {
    // :NOTE: Wei 2005-May-27:
    //
    // Called when data is to be read.
    // length bytes should be transferred (via put_user or copy_to_user) to the
    // userspace memory at address buffer.
    // offset holds a pointer to the current position inside the file. It should be
    // updated to reflect the new position.
    //
    // The function has to return the number of bytes actually copied
    // (it is ok to return less then count bytes).
    // If no data is available the function should wait for new data to arrive and
    // then return that data.
    //
    // A return value of 0 indicates 'End of File'.
    int const result = cb_read(node->read_pipe, buffer, length);
    
    switch (result)
    {
    case -ERESTARTSYS:
      return -ERESTARTSYS;
      
    case 0:
      // :NOTE: Wei 2005-May-27:
      //
      // Every driver should check file->f_flags for O_NONBLOCK and return -EAGAIN
      // if no data is available immediately.
      if (file->f_flags & O_NONBLOCK)
      {
        return -EAGAIN;
      }
      else
      {
        set_current_state(TASK_INTERRUPTIBLE);
        
        // :NOTE: Wei 2005-May-28:
        //
        // the argument value is in "jiffies".
        //
        // If I set the current process state to TASK_INTERRUPTIBLE,
        // this routine may return early if a signal is delivered to the current task.
        // In this case the remaining time in jiffies will be returned,
        // or 0 if the timer expired in time
        //
        // The current task state is guaranteed to be TASK_RUNNING when this routine returns.
        //
        // Specifying a timeout value of MAX_SCHEDULE_TIMEOUT will schedule the CPU away
        // without a bound on the timeout. In this case the return value will be
        // MAX_SCHEDULE_TIMEOUT.
        //
        // In all cases the return value is guaranteed to be non-negative.
        schedule_timeout(RW_WAIT_TIMEOUT);
        
        if (signal_pending(current))
        {
          return -EINTR;
        }
      }
      break;
      
    default:
      return result;
    }
  }
}

static ssize_t
ARMware_SSP_write(struct file *file, char const *buff, size_t len, loff_t *off)
{
  ARMwareSSPNode * const node = (ARMwareSSPNode *)file->private_data;
  
#if DEBUG
  {
    size_t i;
    
    printk(KERN_DEBUG "ARMware_SSP_write: ");
    
    for (i = 0; i != len; ++i)
    {
      printk("%c ", buff[i]);
    }
    
    printk("\n");
  }
#endif
  
  return cb_write(node->write_pipe, buff, len);
}

static unsigned int
ARMware_SSP_poll(struct file *file, struct poll_table_struct *a)
{
  unsigned int mask = 0;
  ARMwareSSPNode * const node = (ARMwareSSPNode *)file->private_data;
  
#if DEBUG
  printk(KERN_DEBUG "ARMware_SSP_poll.\n");
#endif
  
  if (cb_get_max_read_size(node->read_pipe) > 0)
  {
    mask |= POLLIN | POLLRDNORM;
  }
  
  // :NOTE: Wei 2005-May-29:
  //
  // I always can do writing.
  mask |= POLLOUT | POLLWRNORM;
  
  return mask;
}

#if DEBUG
static loff_t
ARMware_SSP_lseek(struct file *file, loff_t a, int b)
{
  printk(KERN_DEBUG "ARMware_SSP_lseek.\n");
  
  return 0;
}

static int
ARMware_SSP_readdir(struct file *file, void *a, filldir_t b)
{
  printk(KERN_DEBUG "ARMware_SSP_readdir.\n");
  
  return 0;
}

static int
ARMware_SSP_ioctl(struct inode *inode, struct file *file, unsigned int a, unsigned long b)
{
  printk(KERN_DEBUG "ARMware_SSP_ioctl: a = %d, b = %ld\n", a, b);
  
  return 0;
}

static int
ARMware_SSP_mmap(struct file *file, struct vm_area_struct *vm)
{
  printk(KERN_DEBUG "ARMware_SSP_mmap.\n");
  
  return 0;
}

static int
ARMware_SSP_flush(struct file *file)
{
  printk(KERN_DEBUG "ARMware_SSP_flush.\n");
  
  return 0;
}

static int
ARMware_SSP_fsync(struct file *file, struct dentry *dentry, int datasync)
{
  printk(KERN_DEBUG "ARMware_SSP_fsync.\n");
  
  return 0;
}

static int
ARMware_SSP_fasync(int a, struct file *file, int b)
{
  printk(KERN_DEBUG "ARMware_SSP_fasync.\n");
  
  return 0;
}

static int
ARMware_SSP_lock(struct file *file, int a, struct file_lock *b)
{
  printk(KERN_DEBUG "ARMware_SSP_lock.\n");
  
  return 0;
}

static ssize_t
ARMware_SSP_readv(struct file *file, const struct iovec *a, unsigned long b, loff_t *c)
{
  printk(KERN_DEBUG "ARMware_SSP_readv.\n");
  
  return 0;
}

static ssize_t
ARMware_SSP_writev(struct file *file, const struct iovec *a, unsigned long b, loff_t *c)
{
  printk(KERN_DEBUG "ARMware_SSP_writev.\n");
  
  return 0;
}
#endif

module_init(ARMware_SSP_init);
module_exit(ARMware_SSP_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Wei Hu <wei.hu.tw@gmail.com>");
MODULE_DESCRIPTION("The inner pipe for software serial ports for ARMware");
MODULE_SUPPORTED_DEVICE("ARMware");
