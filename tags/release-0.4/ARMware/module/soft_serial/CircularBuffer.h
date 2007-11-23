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

#ifndef CircularBuffer_h
#define CircularBuffer_h

#include <asm/semaphore.h>

#define BUFFER_SIZE 1024

typedef struct CircularBuffer CircularBuffer;
struct CircularBuffer
{
  uint32_t m_read_idx;
  uint32_t m_write_idx;
  
  uint8_t *mp_data;
  
  struct semaphore sem;
};

uint32_t cb_get_max_read_size(CircularBuffer const * const cb);
int  cb_init(CircularBuffer * const pipe);
void cb_exit(CircularBuffer * const pipe);
ssize_t cb_write(CircularBuffer * const cb, uint8_t const * const data, uint32_t const length);
ssize_t cb_read(CircularBuffer * const cb, uint8_t * const buffer, uint32_t const length);

#endif
