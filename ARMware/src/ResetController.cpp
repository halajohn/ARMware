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

#if TRACE_RESET_CTRL
#include <iostream>
#endif

#include <cassert>
#include "ResetController.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Operation ==================================
  
  void
  ResetController::reset()
  {
    mRSRR = 0;
    mRCSR = 1;
  }
  
  template<>
  uint32_t
  ResetController::get_data(uint32_t const address) const
  {
#if TRACE_RESET_CTRL
    g_log_file << "RESET CTRL: get value at address " << std::hex << address << std::endl;
#endif
    
    switch (address)
    {
    case RSRR:
      // :SA-1110 Developer's Manual: Wei 2003-Dec-11:
      //
      // RSRR is write-only.
      assert(!"Should not reach here.");
      return 0;
      
    case RCSR:
      return mRCSR;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<>
  void
  ResetController::put_data(uint32_t const address, uint32_t const value)
  {
#if TRACE_RESET_CTRL
    g_log_file << "RESET CTRL: put " << std::hex << value << " at address " << address << std::endl;
#endif
    
    switch (address)
    {
    case RSRR: mRSRR = value; break;
    case RCSR: mRCSR = value; break;
    default: assert(!"Should not reach here."); break;
    }
  }
}
