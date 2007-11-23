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

#ifndef ResetController_hpp
#define ResetController_hpp

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  typedef class ResetController ResetController;
  class ResetController
  {
  private:
    
    enum
    {
      RSRR = 0x90030000,
      RCSR = 0x90030004
    };
    
    // Attribute
    
    uint32_t mRSRR; // Reset controller software reset register
    uint32_t mRCSR; // Reset controller status register
    
    // Operation
    
    void
    init_register()
    {
      mRSRR = 0;
      mRCSR = 0;
    }
    
  public:
    
    // Life cycle
    
    ResetController()
    {
      init_register();
    }
    
    // Operation
    
    void reset();
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
  
  template<typename T_return>
  T_return
  ResetController::get_data(uint32_t const /* address */) const
  {
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  template<>
  uint32_t FASTCALL ResetController::get_data(uint32_t const address) const;
  
  template<typename T_store>
  void
  ResetController::put_data(uint32_t const /* address */, T_store const /* value */)
  {
    assert(!"Should not reach here.");
  }
  
  template<>
  void FASTCALL ResetController::put_data(uint32_t const address, uint32_t const value);
}

#endif
