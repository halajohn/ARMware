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

#ifndef RTC_hpp
#define RTC_hpp

// System include
//

#include <cassert>

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  typedef class RTC RTC;
  class RTC
  {
  private:
    
    // Register location
    
    enum
    {
      RTAR = 0x90010000,
      RCNR = 0x90010004,
      RTTR = 0x90010008,
      RTSR = 0x90010010
    };
    
    enum
    {
      STATUS_AL_BIT = (1 << 0),
      STATUS_HZ_BIT = (1 << 1),
      STATUS_ALE_BIT = (1 << 2),
      STATUS_HZE_BIT = (1 << 3),
      
      STATUS_ENABLE_BIT_MASK = (STATUS_ALE_BIT |
                                STATUS_HZE_BIT),
      
      STATUS_VALID_BIT_MASK = (STATUS_AL_BIT |
                               STATUS_HZ_BIT |
                               STATUS_ALE_BIT |
                               STATUS_HZE_BIT)
    };
    
    // Register
    
    uint32_t m_RTAR; // RTC alarm register
    uint32_t m_RCNR; // RTC count register
    uint32_t m_RTTR; // RTC timer trim register
    uint32_t m_RTSR; // RTC status register
    
    // Operation
    
    void
    init_register()
    {
      m_RTAR = 0;
      m_RCNR = 0;
      m_RTTR = 0;
      m_RTSR = 0;
    }
    
    // Friend
    
    friend class IntController;
    
  public:
    
    // Life cycle
    
    RTC();
    
    // Operation
    
    void run();
    void reset();
    
    inline uint32_t
    get_interrupt_status() const
    {
      return m_RTSR;
    }
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
  
  template<typename T_return>
  T_return
  RTC::get_data(uint32_t const /* address */) const
  {
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  template<>
  uint32_t FASTCALL RTC::get_data(uint32_t const address) const;
  
  template<typename T_store>
  void
  RTC::put_data(uint32_t const /* address */, T_store const /* value */)
  {
    assert(!"Should not reach here.");
  }
  
  template<>
  void FASTCALL RTC::put_data(uint32_t const address, uint32_t const value);
}

#endif
