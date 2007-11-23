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

#ifndef GPIOControlBlock_hpp
#define GPIOControlBlock_hpp

#if TRACE_GPIO
#include <iostream>
#endif

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class SoftKeyboard;
  
  typedef class GPIOControlBlock GPIOControlBlock;
  class GPIOControlBlock
  {
  private:
    
    // Register location
    
    enum
    {
      GPLR = 0x90040000,
      GPDR = 0x90040004,
      GPSR = 0x90040008,
      GPCR = 0x9004000C,
      GRER = 0x90040010,
      GFER = 0x90040014,
      GEDR = 0x90040018,
      GAFR = 0x9004001C
    };
    
    // Normal function
    
    enum
    {
      GAFR_ACTION_BUTTON = (1 << 18)
    };
    
    // Alternative function
    
    enum
    {
      LDD_8  = (1 << 2),
      LDD_9  = (1 << 3),
      LDD_10 = (1 << 4),
      LDD_11 = (1 << 5),
      LDD_12 = (1 << 6),
      LDD_13 = (1 << 7),
      LDD_14 = (1 << 8),
      LDD_15 = (1 << 9),
      
      LDD_BITS = (LDD_8 | LDD_9 | LDD_10 | LDD_11 | LDD_12 | LDD_13 | LDD_14 | LDD_15)
    };
    
    // Attribute
    
    uint32_t m_GPLR; // GPIO pin-level register
    uint32_t m_GPDR; // GPIO pin direction register
    uint32_t m_GPSR; // GPIO pin output set register
    uint32_t m_GPCR; // GPIO pin output clear register
    uint32_t m_GRER; // GPIO rising-edge detect register
    uint32_t m_GFER; // GPIO falling-edge detect register
    uint32_t m_GEDR; // GPIO edge detect status register
    uint32_t m_GAFR; // GPIO alternate function register
    
    uint32_t m_GPLR_backup;
    
    // Operation
    
    void
    init_register()
    {
      m_GPLR = 0;
      m_GPDR = 0;
      m_GPSR = 0;
      m_GPCR = 0;
      m_GRER = 0;
      m_GFER = 0;
      m_GEDR = 0;
      m_GAFR = 0;
    }
    
  public:
    
    // Life cycle
    
    GPIOControlBlock()
    {
      init_register();
    }
    
    // Operation
    
    inline void
    run()
    {
      uint32_t const temp = m_GPLR ^ m_GPLR_backup;
      
      if (temp != 0)
      {
#if TRACE_GPIO
        g_log_file << "-GPIO: m_GPLR: " << std::hex << m_GPLR << std::endl;
        g_log_file << "-GPIO: m_GPLR_backup: " << std::hex << m_GPLR_backup << std::endl;
#endif
        
        m_GEDR |= (m_GRER & (m_GPLR & temp));
        m_GEDR |= (m_GFER & (m_GPLR_backup & temp));
        
        m_GPLR_backup = m_GPLR;
      }
    }
    
    void reset();
    
    inline uint32_t
    get_interrupt_status() const
    { return m_GEDR; }
    
    // :NOTE: Wei 2004-Jan-11:
    //
    // if bit 18 of GPLR == 0: action button is pressed.
    // if bit 18 of GPLR == 1: action button is released.
    inline void
    press_action_button()
    {
      // :SA-1110 Developer's Manual: p.80: Wei 2004-Jul-1:
      //
      // A zero in GAFR indicates that the corresponding GPIO pin is to be used for its normal GPIO function.
      if (0 == (m_GAFR & GAFR_ACTION_BUTTON))
      {
        m_GPLR &= ~GAFR_ACTION_BUTTON;
      }
    }
    
    inline void
    release_action_button()
    {
      if (0 == (m_GAFR & GAFR_ACTION_BUTTON))
      {
        m_GPLR |= GAFR_ACTION_BUTTON;
      }
    }
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
  
  template<typename T_return>
  T_return
  GPIOControlBlock::get_data(uint32_t const /* address */) const
  {
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  template<>
  uint32_t FASTCALL GPIOControlBlock::get_data(uint32_t const address) const;
  
  template<typename T_store>
  void
  GPIOControlBlock::put_data(uint32_t const /* address */, T_store const /* value */)
  { assert(!"Should not reach here."); }
  
  template<>
  void FASTCALL GPIOControlBlock::put_data(uint32_t const address, uint32_t const value);
}

#endif
