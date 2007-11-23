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

#ifndef UartTraits_hpp
#define UartTraits_hpp

#include "IntController.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Serial_1;
  class Serial_2;
  class Serial_3;
  
  template<typename T_serial_port>
  struct UartTraits;
  
  template<>
  struct UartTraits<Serial_1>
  {
    static uint32_t const PORT_NUMBER = 1;
    static IntController::IntCtrlBitMaskEnum const INT_CTRL_BIT_MASK = IntController::SERIAL_1_BIT_MASK;
    
    enum
    {
      UTCR0 = 0x80010000,
      UTCR1 = 0x80010004,
      UTCR2 = 0x80010008,
      UTCR3 = 0x8001000C,
      UTDR  = 0x80010014,
      UTSR0 = 0x8001001C,
      UTSR1 = 0x80010020
    };
  };
  
  template<>
  struct UartTraits<Serial_2>
  {
    static uint32_t const PORT_NUMBER = 2;
    static IntController::IntCtrlBitMaskEnum const INT_CTRL_BIT_MASK = IntController::SERIAL_2_BIT_MASK;
    
    enum
    {
      UTCR0 = 0x80030000,
      UTCR1 = 0x80030004,
      UTCR2 = 0x80030008,
      UTCR3 = 0x8003000C,
      UTDR  = 0x80030014,
      UTSR0 = 0x8003001C,
      UTSR1 = 0x80030020
    };
  };
  
  template<>
  struct UartTraits<Serial_3>
  {
    static uint32_t const PORT_NUMBER = 3;
    static IntController::IntCtrlBitMaskEnum const INT_CTRL_BIT_MASK = IntController::SERIAL_3_BIT_MASK;
    
    enum
    {
      UTCR0 = 0x80050000,
      UTCR1 = 0x80050004,
      UTCR2 = 0x80050008,
      UTCR3 = 0x8005000C,
      UTDR  = 0x80050014,
      UTSR0 = 0x8005001C,
      UTSR1 = 0x80050020
    };
  };
}

#endif
