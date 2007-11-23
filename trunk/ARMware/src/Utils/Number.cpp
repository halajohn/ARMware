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

#include "Number.hpp"

namespace ARMware
{
  bool
  power_of_two(uint32_t value)
  {
    // :NOTE: Wei 2004-May-08:
    //
    // 0 is not the power of 2.
    //
    // 1 is the power of 2 (2^0 = 1), but I don't want Tlb has only 1 entry.
    // Thus I make this check, too.
    if ((0 == value) || (1 == value))
    {
      return false;
    }
    
    // :NOTE: Wei 2004-May-08:
    //
    // If a number is the power of 2, then its binary representation must contain only
    // one 1, and other bits have to be 0.
    // Ex:  2 = 10b
    //      4 = 100b
    //      8 = 1000b
    //     16 = 10000b
    //     32 = 100000b
    bool hit = false;
    
    for (int i = 0; i < 31; ++i)
    {
      if (0x1 == (value & 0x1))
      {
        if (true == hit)
        {
          // If this value has more than one 1.
          return false;
        }
        else
        {
          hit = true;
        }
      }
      
      value >>= 1;
    }
    
    // Because I have already excluded 0,
    // thus the value must contain at least one 1.
    // As a result, I don't need to check whether (hit == true) or not.
    return true;
  }
}
