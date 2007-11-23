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

// System include
//

#include <iostream>

#include <cassert>

// Local include
//

#include "Gtk/SoftKeyboard.hpp"
#include "GPIOControlBlock.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Operation ==================================
  
  void
  GPIOControlBlock::reset()
  {
    m_GPLR &= 0xFFFFFFF;
    m_GEDR &= 0xFFFFFFF;
    m_GPDR = 0;
    m_GAFR = 0;
    
    release_action_button();
    
    m_GPLR_backup = m_GPLR;
  }
  
  template<>
  uint32_t
  GPIOControlBlock::get_data(uint32_t const address) const
  {
    switch (address)
    {
    case GPLR:
#if TRACE_GPIO
      g_log_file << "-GPIO: get GPLR: " << m_GPLR << std::endl;
#endif
      return m_GPLR;
      
    case GPDR:
#if TRACE_GPIO
      g_log_file << "-GPIO: get GPDR: " << m_GPDR << std::endl;
#endif
      return m_GPDR;
      
    case GPSR:
    case GPCR:
      // :SA-1110 Developer's Manual: p.77: Wei 2004-Jun-05:
      //
      // GPSR & GPCR are write-only registers.
      assert(!"Should not reach here.");
      return 0;
      
    case GRER:
#if TRACE_GPIO
      g_log_file << "-GPIO: get GRER: " << m_GRER << std::endl;
#endif
      return m_GRER;
      
    case GFER:
#if TRACE_GPIO
      g_log_file << "-GPIO: get GFER: " << m_GFER << std::endl;
#endif
      return m_GFER;
      
    case GEDR:
#if TRACE_GPIO
      g_log_file << "-GPIO: get GEDR: " << m_GEDR << std::endl;
#endif
      return m_GEDR;
      
    case GAFR:
#if TRACE_GPIO
      g_log_file << "-GPIO: get GAFR: " << m_GAFR << std::endl;
#endif
      return m_GAFR;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<>
  void
  GPIOControlBlock::put_data(uint32_t const address, uint32_t const value)
  {
    switch (address)
    {
    case GPLR:
      // :SA-1110 Developer's Manual: p.75: Wei 2004-Jun-05:
      //
      // GPLR is a read-only register.
      assert(!"Should not reach here.");
      break;
      
    case GPDR:
      // :SA-1110 Developer's Manual: p.233: Wei 2004-Jun-06:
      //
      // In active mode, GPIO pins 2..9 are also used.
      // Note that the user must configure GPIO pins 2..9 as outputs (for 16-bit/pixel mode)
      // by setting the appropriate bits within the GPIO pin direction register (GPDR)
      // and GPIO alternate function register (GAFR).
      //
      // :NOTE: Wei 2004-Jun-06:
      //
      // ARMware supports only TFT (active) mode now.
      assert(LDD_BITS == (value & LDD_BITS));
      
      // :SA-1110 Developer's Manual: p.76: Wei 2004-Jun-05:
      //
      // The upper 4 bits are always 0.
      m_GPDR = (value & 0xFFFFFFF);
      
#if TRACE_GPIO
      g_log_file << "-GPIO: put GPDR: " << std::hex << m_GPDR << std::endl;
#endif
      break;
      
    case GPSR:
      {
#if TRACE_GPIO
        g_log_file << "-GPIO: put GPSR: " << std::hex << value << std::endl;
#endif
        
        // Determine what pins are configured as output and we want to set it value now.
        uint32_t const temp = (m_GPDR & (value & 0xFFFFFFF));
        uint32_t const new_GPLR = m_GPLR | temp;
        uint32_t const diff = m_GPLR ^ new_GPLR;
        
        // Update GEDR
        m_GEDR |= (m_GRER & (new_GPLR & diff));
        
        // Update GPLR
        m_GPLR = new_GPLR;
        
#if TRACE_GPIO
        g_log_file << "-GPIO: new GPLR: " << std::hex << m_GPLR << std::endl;
#endif
      }
      break;
      
    case GPCR:
      {
#if TRACE_GPIO
        g_log_file << "-GPIO: put GPCR: " << std::hex << value << std::endl;
#endif
        
        // Determine what pins are configured as output and we want to clear it value now.
        uint32_t const temp = (m_GPDR & (value & 0xFFFFFFF));
        
        // Update GEDR
        m_GEDR |= (m_GFER & (m_GPLR & temp));
        
        // Update GPLR
        
        // :SA-1110 Developer's Manual: p.77: Wei 2004-Jul-1:
        //
        // To clear an output pin, a one is written to the corresponding bit within the GPCR.
        m_GPLR &= ~temp;
        
#if TRACE_GPIO
        g_log_file << "-GPIO: new GPLR: " << std::hex << m_GPLR << std::endl;
#endif
      }
      break;
      
    case GRER:
      m_GRER = (value & 0xFFFFFFF);
      
#if TRACE_GPIO
      g_log_file << "-GPIO: put GRER: " << std::hex << m_GRER << std::endl;
#endif
      break;
      
    case GFER:
      m_GFER = (value & 0xFFFFFFF);
      
#if TRACE_GPIO
      g_log_file << "-GPIO: put GFER: " << std::hex << m_GFER << std::endl;
#endif
      break;
      
    case GEDR:
      // :SA-1110 Developer's Manual: p.79: Wei 2004-Jun-05:
      //
      // GEDR status bits are cleared by writing a one to them.
      // Writing a zero to a GEDR status bit has no effect.
      m_GEDR &= ~(value & 0xFFFFFFF);
      
#if TRACE_GPIO
      g_log_file << "-GPIO: put GEDR: " << std::hex << m_GEDR << std::endl;
#endif
      break;
      
    case GAFR:
      // :SA-1110 Developer's Manual: p.233: Wei 2004-Jun-06:
      //
      // In active mode, GPIO pins 2..9 are also used.
      // Note that the user must configure GPIO pins 2..9 as outputs (for 16-bit/pixel mode)
      // by setting the appropriate bits within the GPIO pin direction register (GPDR)
      // and GPIO alternate function register (GAFR).
      //
      // :NOTE: Wei 2004-Jun-06:
      //
      // ARMware supports only TFT (active) mode now.
      
      // :SA-1110 Developer's Manual: p.80: Wei 2004-Jun-06:
      //
      // A bit set in this register indicates that the corresponding GPIO pin
      // is to be used for its alternate function.
      assert(LDD_BITS == (value & LDD_BITS));
      
      m_GAFR = (value & 0xFFFFFFF);
      
#if TRACE_GPIO
      g_log_file << "-GPIO: put GAFR: " << std::hex << m_GAFR << std::endl;
#endif
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}
