// ARMware - an ARM emulator
// Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <sys/stat.h>
#include <cerrno>
#include <cassert>

#include "File.hpp"
#include "../platform_dep.hpp"

namespace ARMware
{
  CheckFileErrorCode
  check_file_error(char const * const filename)
  {
    struct stat file_stat;
    
    int const result = stat(filename, &file_stat);
    
    if (0 == result)
    {
      if (S_ISDIR(file_stat.st_mode) != 0)
      {
        return ERROR_IS_DIR;
      }
      
      return NO_ERROR;
    }
    else
    {
      // result == -1
      switch (errno)
      {
      case ENOENT:
      case ENOTDIR:
        return ERROR_FILE_NONEXIST;
        
      default:
        // :NOTE: Wei 2004-Mar-18:
        //
        // If we go here, means some other errors occurs.
        assert(!"Should not reach here.");
        return ERROR_FILE_NONEXIST;
      }
    }
  }
}
