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

#ifndef Number_hpp
#define Number_hpp

#include <cassert>

#include "../Type.hpp"

namespace ARMware
{
  bool FUNCTION_CONST power_of_two(uint32_t value);
  
  inline bool
  IsPositive(uint32_t const value)
  {
    return ((static_cast<int32_t>(value) >= 0) ? true : false);
  }
  
  inline bool
  IsNegative(uint32_t const value)
  {
    return ((static_cast<int32_t>(value) < 0) ? true : false);
  }
  
  inline uint32_t
  min(uint32_t const a, uint32_t const b)
  {
    if (a <= b)
    {
      return a;
    }
    else
    {
      return b;
    }
  }
  
  inline uint32_t
  max(uint32_t const a, uint32_t const b)
  {
    if (a >= b)
    {
      return a;
    }
    else
    {
      return b;
    }
  }
  
  inline uint32_t
  RotateRight(uint32_t const value, uint32_t const imm)
  {
    return (value >> imm) | (value << (32 - imm));
  }
  
  enum BitValueMoveToRightEnum
  {
    MOVE_TO_RIGHT,
    NO_MOVE_TO_RIGHT
  };
  typedef enum BitValueMoveToRightEnum BitValueMoveToRightEnum;
  
  template<BitValueMoveToRightEnum T_move>
  inline uint32_t
  Bit31(uint32_t const value)
  {
    switch (T_move)
    {
    case MOVE_TO_RIGHT:
      return value >> 31;
      
    case NO_MOVE_TO_RIGHT:
      return value & (1 << 31);
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<BitValueMoveToRightEnum T_move>
  inline uint32_t
  Bit0(uint32_t const value)
  {
    switch (T_move)
    {
    case MOVE_TO_RIGHT:
    case NO_MOVE_TO_RIGHT:
      return value & 0x1;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<BitValueMoveToRightEnum T_move>
  inline uint32_t
  Bitn(uint32_t const value, uint32_t const position)
  {
    assert(position <= 31);
    
    switch (T_move)
    {
    case MOVE_TO_RIGHT:
      return (value >> position) & 0x1;
      
    case NO_MOVE_TO_RIGHT:
      return value & (1 << position);
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
}

#endif
