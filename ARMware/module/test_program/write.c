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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int
open_port(void)
{
  int fd; /* File descriptor for the port */
  
  fd = open("/dev/ClientSideSSP", O_RDWR | O_NOCTTY | O_NDELAY);
  
  if (-1 == fd)
  {
    perror("open_port: Unable to open /dev/ttyf1 - ");
  }
  else
  {
    fcntl(fd, F_SETFL, 0);
  }
  
  return fd;
}

int
main(int argc, char **argv)
{
  int fd = open_port();
  char buffer[1024] = {'a', 'b', 'c', 0x80, 0x81, 0x82, 0x93, 0xa3, 0xa4, 0xee, 0xd, };
  int num;
  int i;
  fd_set fds;
  struct timeval tv;
  
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  
  i = fd;
  
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  
  if (select(i+1, NULL, &fds, NULL, &tv) > 0)
  {
    if (FD_ISSET(fd, &fds) > 0)
    {
      write(fd, buffer, strlen(buffer));
    }
  }
  
  close(fd);
  
  return 0;
}
