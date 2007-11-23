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

#include <asm/uaccess.h>

#include "CircularBuffer.h"

static inline uint32_t
cb_init_internal(CircularBuffer * const cb)
{
  cb->m_read_idx = 0;
  cb->m_write_idx = 0;
  
  cb->mp_data = kmalloc(BUFFER_SIZE, GFP_KERNEL);
  
  if (0 == cb->mp_data)
  {
    return 1;
  }
  
  init_MUTEX(&(cb->sem));
  
  return 0;
}

static inline void
cb_exit_internal(CircularBuffer * const cb)
{
  // :NOTE: Wei 2005-May-27:
  //
  // There's no need to check a pointer for NULL before calling kfree() on it -
  // kfree() handles NULL pointers just fine on its own.
  kfree(cb->mp_data);
}

int
cb_init(CircularBuffer * const pipe)
{
  uint32_t result;
  
  result = cb_init_internal(&(pipe[0]));
  
  if (result != 0)
  {
    // :NOTE: Wei 2005-May-26:
    //
    // A non-zero return means cb_init failed; the module can't be loaded.
    return 1;
  }
  
  result = cb_init_internal(&(pipe[1]));
  
  if (result != 0)
  {
    cb_exit_internal(&(pipe[0]));
    
    // :NOTE: Wei 2005-May-26:
    //
    // A non-zero return means cb_init failed; the module can't be loaded.
    return 1;
  }
  
  return 0;
}

void
cb_exit(CircularBuffer * const pipe)
{
  cb_exit_internal(&(pipe[0]));
  cb_exit_internal(&(pipe[1]));
}

static inline uint32_t
get_max_writable_size(CircularBuffer const * const cb)
{
  if (cb->mp_data != 0)
  {
    if (cb->m_read_idx == cb->m_write_idx)
    {
      // :NOTE: Wei 2005-May-23:
      //
      // Buffer is empty.
      // Thus the writable size is BUFFER_SIZE.
      return BUFFER_SIZE;
    }
    else if (cb->m_write_idx < cb->m_read_idx)
    {
      return cb->m_read_idx - cb->m_write_idx;
    }
    else
    {
      return (BUFFER_SIZE - cb->m_write_idx) + cb->m_read_idx;
    }
  }
  else
  {
    printk(KERN_DEBUG "CircularBuffer: internal data buffer pointer is NULL.");
    
    return 0;
  }
}

static inline unsigned long
do_write(CircularBuffer * const cb, uint8_t const * const data, uint32_t const length)
{
  unsigned long still_need_to_write = 0;
  
  // :NOTE: Wei 2005-May-24:
  //
  // There are 2 conditions:
  //
  // 1) Need to rewind.
  //
  // 2) Don't need to rewind.
  
  if ((BUFFER_SIZE - cb->m_write_idx) >= length)
  {
    // :NOTE: Wei 2005-May-24:
    //
    // Don't need to rewind.
    //
    // Ex: BUFFER_SIZE = 10
    //     length = 5
    //     write_idx = 3
    //     new write_idx = 8 = length + write_idx
    //
    // BUFFER_SIZE - m_write_idx = 7 > 5 = length
    //
    // 0 1 2 3 4 5 6 7 8 9
    //       w w w w w
    //
    // Ex: BUFFER_SIZE = 10
    //     length = 5
    //     write idx = 5
    //     new write_idx = 0 (= 10 % BUFFER_SIZE) = (length + write_idx) % BUFFER_SIZE
    //
    // BUFFER_SIZE - m_write_idx = 5 = 5 = length
    //
    // 0 1 2 3 4 5 6 7 8 9
    //           w w w w w
    still_need_to_write += copy_from_user(&(cb->mp_data[cb->m_write_idx]), data, length);
  }
  else
  {
    // :NOTE: Wei 2005-May-24:
    //
    // Need to rewind.
    //
    // Ex: BUFFER_SIZE = 10
    //     length = 5
    //     write_idx = 6
    //     new write_idx = 1 = (length + write_idx) % BUFFER_SIZE
    //     the 1st part length = 4 = 10 - 6 = BUFFER_SIZE - write_idx
    //
    // BUFFER_SIZE - m_write_idx = 7 > 5 = length
    //
    // 0 1 2 3 4 5 6 7 8 9
    // w           w w w w
    if (BUFFER_SIZE >= length)
    {
      still_need_to_write += copy_from_user(&(cb->mp_data[cb->m_write_idx]),
                                            data,
                                            BUFFER_SIZE - cb->m_write_idx);
      
      // :NOTE: Wei 2005-May-24:
      //
      // The 2nd part length = length - 1st part length
      // The start point of the 2nd part in dest is 0
      // The start point of the 2nd part in source is (source + 1st part length)
      still_need_to_write += copy_from_user(&(cb->mp_data[0]),
                                            data + (BUFFER_SIZE - cb->m_write_idx),
                                            length - (BUFFER_SIZE - cb->m_write_idx));
    }
    else
    {
      // Ex: BUFFER_SIZE = 10
      //     length = 21
      //     write_idx = 6
      //     new_write_idx = 7 = (length + write_idx) % BUFFER_SIZE
      //     the 1st part size = 3 = 10 - 7 = BUFFER_SIZE - new_write_idx
      //     the 1st part start from source: 12 = length - BUFFER_SIZE + 1
      //     the 1st part start from dest: new_write_idx
      //
      // 0 1 2 3 4 5 6 7 8 9
      //             w w w w
      // w w w w w w w w w w
      // w w w w w w w - - - --> the 1st length = 3
      uint32_t const new_write_idx = (cb->m_write_idx + length) & (BUFFER_SIZE - 1);
      
      still_need_to_write += copy_from_user(&(cb->mp_data[new_write_idx]),
                                            data + (length - BUFFER_SIZE + 1),
                                            BUFFER_SIZE - new_write_idx);
      
      // :NOTE: Wei 2005-May-25:
      //
      // the 2nd part size = BUFFER_SIZE - 1st part size
      // the 2nd part start point in dest = 0
      // the 2nd part start point in source = 7 = length - BUFFER_SIZE + 1 + 1st part size
      still_need_to_write += copy_from_user(&(cb->mp_data[0]),
                                            data + (length - BUFFER_SIZE + 1 + (BUFFER_SIZE - new_write_idx)),
                                            length - (BUFFER_SIZE - new_write_idx));
    }
  }
  
  // :NOTE: Wei 2005-May-24:
  //
  // Come from (cb->m_write_idx + length) % BUFFER_SIZE
  cb->m_write_idx = (cb->m_write_idx + length) & (BUFFER_SIZE - 1);
  
  return still_need_to_write;
}

ssize_t
cb_write(CircularBuffer * const cb, uint8_t const * const data, uint32_t const length)
{
  if (down_interruptible(&cb->sem))
  {
    return -ERESTARTSYS;
  }
  else
  {
    uint32_t const writable_size = get_max_writable_size(cb);
    unsigned long still_need_to_write = do_write(cb, data, length);
    
    if (still_need_to_write != 0)
    {
      printk(KERN_DEBUG "can not finish write.");
      
      up(&cb->sem);
      
      return 0;
    }
    else
    {
      if (length > writable_size)
      {
        // :NOTE: Wei 2005-May-25:
        //
        // Need to update read idx to the write idx.
        cb->m_read_idx = cb->m_write_idx;
      }
      else
      {
        // :NOTE: Wei 2005-May-23:
        //
        // No overlap, thus I don't need to change the read idx.
      }
      
      up(&cb->sem);
      
      return length;
    }
  }
}

uint32_t
cb_get_max_read_size(CircularBuffer const * const cb)
{
  if (cb->mp_data != 0)
  {
    if (cb->m_read_idx == cb->m_write_idx)
    {
      return 0;
    }
    else if (cb->m_read_idx < cb->m_write_idx)
    {
      return cb->m_write_idx - cb->m_read_idx;
    }
    else
    {
      return (BUFFER_SIZE - cb->m_read_idx) + cb->m_write_idx;
    }
  }
  else
  {
    return 0;
  }
}

static inline unsigned long
do_read(CircularBuffer * const cb, uint8_t * const buffer, uint32_t const length)
{
  unsigned long still_need_to_read = 0;
  
  if ((BUFFER_SIZE - cb->m_read_idx) >= length)
  {
    // :NOTE: Wei 2005-May-25:
    //
    // No need to rewind.
    
    // Ex: BUFFER_SIZE = 10
    //     length = 5
    //     read_idx = 3
    //
    // BUFFER_SIZE - m_read_idx = 7 > 5 = length
    //
    // 0 1 2 3 4 5 6 7 8 9
    //       r r r r r
    //
    // Ex: BUFFER_SIZE = 10
    //     length = 5
    //     read_idx = 5
    //
    // BUFFER_SIZE - m_read_idx = 7 > 5 = length
    //
    // 0 1 2 3 4 5 6 7 8 9
    //           r r r r r
    still_need_to_read += copy_to_user(buffer, &(cb->mp_data[cb->m_read_idx]), length);
  }
  else
  {
    // :NOTE: Wei 2005-May-25:
    //
    // Need to rewind.
    
    // Ex: BUFFER_SIZE = 10
    //     length = 5
    //     read_idx = 8
    //
    // BUFFER_SIZE - m_read_idx = 7 > 5 = length
    //
    // 0 1 2 3 4 5 6 7 8 9
    // r r r           r r
    
    // :NOTE: Wei 2005-May-25:
    //
    // 1st read size = BUFFER_SIZE - read_idx
    still_need_to_read += copy_to_user(buffer,
                                       &(cb->mp_data[cb->m_read_idx]),
                                       BUFFER_SIZE - cb->m_read_idx);
    
    // :NOTE: Wei 2005-May-25:
    //
    // 2nd read size = length - 1st read size
    // 2nd read start point in dest = 1st read size
    still_need_to_read += copy_to_user(buffer + (BUFFER_SIZE - cb->m_read_idx),
                                       &(cb->mp_data[0]),
                                       length - (BUFFER_SIZE - cb->m_read_idx));
  }
  
  return still_need_to_read;
}

ssize_t
cb_read(CircularBuffer * const cb, uint8_t * const buffer, uint32_t const length)
{
  if (down_interruptible(&cb->sem))
  {
    return -ERESTARTSYS;
  }
  else
  {
    uint32_t const readable_size = cb_get_max_read_size(cb);
    
    if (0 == readable_size)
    {
      up(&cb->sem);
      
      return 0;
    }
    else
    {
      unsigned long still_need_to_read = do_read(cb, buffer, readable_size);
      
      if (still_need_to_read != 0)
      {
        printk(KERN_DEBUG "can not finish read.");
        
        up(&cb->sem);
        
        return 0;
      }
      else
      {
        if (readable_size >= length)
        {
          cb->m_read_idx = (cb->m_read_idx + length) & (BUFFER_SIZE - 1);
        }
        else
        {
          cb->m_read_idx = (cb->m_read_idx + readable_size) & (BUFFER_SIZE - 1);
        }
        
        up(&cb->sem);
        
        return readable_size;
      }
    }
  }
}
