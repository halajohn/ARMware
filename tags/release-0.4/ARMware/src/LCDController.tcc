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

#include "Core.hpp"
#include "LCDController.hpp"
#include "Memory.hpp"
#include "Gtk/MachineScreen.hpp"

namespace ARMware
{
  template<typename T_return>
  T_return
  LCDController::get_data(uint32_t const address) const
  {
#if TRACE_LCD
    g_log_file << "LCD: get value at " << std::hex << address << std::endl;
#endif
    
    switch (address)
    {
    case LCCR0: return m_LCCR0;
    case LCCR1: return m_LCCR1;
    case LCCR2: return m_LCCR2;
    case LCCR3: return m_LCCR3;
    case DBAR1: return m_DBAR1;
    case DCAR1: return m_DCAR1;
    case DBAR2: return m_DBAR2;
    case DCAR2: return m_DCAR2;
    case LCSR: return m_LCSR;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<typename T_store>
  void
  LCDController::put_data(uint32_t const address, T_store const value)
  {
    switch (address)
    {
    case LCCR0:
      {
#if TRACE_LCD
        g_log_file << "LCD: LCCR0: " << std::hex << value << std::endl;
#endif
        
        uint32_t const diff = (m_LCCR0 ^ value);
        
        if ((diff & LCCR0_LEN) != 0)
        {
          if (0 == (value & LCCR0_LEN))
          {
#if TRACE_LCD
            g_log_file << "LCD: disable lcd" << std::endl;
#endif
            
            m_LCSR |= LCSR_LDD;
            
            mp_machine_screen->disable_drawing();
            mp_memory->disable_drawing();
          }
          else
          {
#if TRACE_LCD
            g_log_file << "LCD: enable lcd" << std::endl;
#endif
            
            // :NOTE: Wei 2004-Jun-06:
            //
            // ARMware supports only TFT (active) mode now.
            assert(LCCR0_PAS == (value & LCCR0_PAS));
            
            // :SA-1110 Developer's Manual: Wei 2003-Dec-08:
            //
            // Value in DBAR1(or 2) is transferred to DCAR1(or 2) when LCD is first enabled (LEN = 0->1).
            m_DCAR1 = m_DBAR1;
            m_DCAR2 = m_DBAR2;
            
            // :SA-1110 Developer's Manual: p.249: Wei 2004-Jan-15:
            //
            // The base address update status (BAU) is a read/write status bit
            // that is set after the contents of the DMA base address register 1
            // are transferred to the DMA current Address register 1.
            m_LCSR |= LCSR_BAU;
            
            mp_machine_screen->enable_drawing();
            mp_memory->enable_drawing();
          }
        }
        
        if ((diff & LCCR0_SDS) != 0)
        {
          if (0 == (value & LCCR0_SDS))
          {
            // singal panel
            mp_machine_screen->disable_dual_panel();
            mp_memory->disable_dual_panel();
          }
          else
          {
            // dual panels
            mp_machine_screen->enable_dual_panel();
            mp_memory->enable_dual_panel();
          }
        }
        
        m_LCCR0 = (value & (0xFFFFF ^ (1 << 6)));
      }
      break;
      
    case LCCR1:
#if TRACE_LCD
      g_log_file << "LCD: LCCR1: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: Wei 2004-Jan-13:
      //
      // Note that the bottom four bits of PPL are not implemented and
      // therefore are not writable. Reads of these bits return zeros
      // because the LCD controller only supports displays that are a
      // multiple of 16 pixels wide.
      m_LCCR1 = (value & 0xFFFFFFF0);
      break;
      
    case LCCR2:
#if TRACE_LCD
      g_log_file << "LCD: LCCR2: " << std::hex << value << std::endl;
#endif
      
      m_LCCR2 = value;
      break;
      
    case LCCR3:
#if TRACE_LCD
      g_log_file << "LCD: LCCR3: " << std::hex << value << std::endl;
#endif
      
      m_LCCR3 = (value & 0xFFFFFF);
      break;
      
    case DBAR1:
#if TRACE_LCD
      g_log_file << "LCD: DBAR1: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: p.245: Wei 2003-Dec-08:
      //
      // Addresses programmed in the base address register must be aligned
      // on quadword boundaries;
      // the least significant four bits (DBAR1[3:0]) must always be written with zeros.
      assert(0 == (value & 0xF));
      
      // :NOTE: Wei 2004-Jan-15:
      //
      // If we want to change the frame buffer address,
      // we are likely need to update the whole screen.
      if (m_DBAR1 != value)
      {
        m_DBAR1 = value;
        
        mp_machine_screen->set_frame_buffer_addr_1(m_DBAR1);
        mp_machine_screen->template update_panel<MachineScreen::PANEL_1>();
        
        mp_memory->set_frame_buffer_addr_1(m_DBAR1);
      }
      break;
      
    case DBAR2:
#if TRACE_LCD
      g_log_file << "LCD: DBAR2: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: p.245: Wei 2003-Dec-08:
      //
      // Addresses programmed in the base address register must be aligned
      // on quadword boundaries;
      // the least significant four bits (DBAR1[3:0]) must always be written with zeros.
      assert(0 == (value & 0xF));
      
      if (m_DBAR2 != value)
      {
        m_DBAR2 = value;
        
        mp_machine_screen->set_frame_buffer_addr_2(m_DBAR2);
        mp_machine_screen->template update_panel<MachineScreen::PANEL_2>();
        
        mp_memory->set_frame_buffer_addr_2(m_DBAR2);
      }
      break;
      
    case DCAR1:
    case DCAR2:
      // :SA-1110 Developer's Manual: p.247: Wei 2004-Jun-06:
      // :SA-1110 Developer's Manual: p.248: Wei 2004-Jun-06:
      //
      // These are read-only registers.
      assert(!"Should not reach here.");
      break;
      
    case LCSR:
#if TRACE_LCD
      g_log_file << "LCD: LCSR: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: p.248: Wei 2004-Jun-06:
      //
      // Status bits are referred to as 'sticky' (once set by hardware,
      // they must be cleared by software). Writing a 1 to a sticky status bit clears it;
      // writing a zero has no effect.
      // Read-only flags are set and cleared by hardware; writes have no effect.
      
      m_LCSR &= ~(m_LCSR & (value & LCSR_READ_WRITE_BITMASK));
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}
