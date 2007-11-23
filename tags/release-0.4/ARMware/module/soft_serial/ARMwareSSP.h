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

#ifndef ARMwareSSP_h
#define ARMwareSSP_h

#include <asm/atomic.h>

#include "CircularBuffer.h"

#define ARMWARE_SSP_DEV_NAME "ARMwareSSP"
#define ARMWARE_SSP_CLASS_NAME "ARMwareSSPClass"

#define MINOR_NUM_0 0
#define MINOR_NUM_1 1

#define DEV_FILE_NAME_0 "ARMwareSideSSP"
#define DEV_FILE_NAME_1 "ClientSideSSP"

#define RW_WAIT_TIMEOUT 100

#define DEBUG 1

typedef struct ARMwareSSPNode ARMwareSSPNode;
struct ARMwareSSPNode
{
  CircularBuffer *read_pipe;
  CircularBuffer *write_pipe;
  
  atomic_t device_opened;
};

static int ARMware_SSP_open(struct inode *inode, struct file *file);
static int ARMware_SSP_release(struct inode *inode, struct file *file);
static ssize_t ARMware_SSP_read(struct file *file, char *buffer, size_t length, loff_t *offset);
static ssize_t ARMware_SSP_write(struct file *file, char const *buff, size_t len, loff_t *off);

#if DEBUG
static loff_t ARMware_SSP_lseek(struct file *, loff_t, int);
static int ARMware_SSP_readdir(struct file *, void *, filldir_t);
static unsigned int ARMware_SSP_poll(struct file *, struct poll_table_struct *);
static int ARMware_SSP_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
static int ARMware_SSP_mmap(struct file *, struct vm_area_struct *);
static int ARMware_SSP_flush(struct file *);
static int ARMware_SSP_fsync(struct file *, struct dentry *, int datasync);
static int ARMware_SSP_fasync(int, struct file *, int);
static int ARMware_SSP_lock(struct file *, int, struct file_lock *);
static ssize_t ARMware_SSP_readv(struct file *, const struct iovec *, unsigned long, loff_t *);
static ssize_t ARMware_SSP_writev(struct file *, const struct iovec *, unsigned long, loff_t *);
#endif

#endif
