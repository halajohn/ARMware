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

#ifndef PPC_hpp
#define PPC_hpp

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  typedef class PPC PPC;
  class PPC
  {
  private:
    
    enum
    {
      PPDR = 0x90060000,
      PPSR = 0x90060004,
      PPAR = 0x90060008,
      PSDR = 0x9006000C,
      PPFR = 0x90060010,
      
      HSCR2 = 0x90060028 // Serial 2
    };
    
    // Attribute
    
    uint32_t m_PPDR; // PPC pin direction register
    uint32_t m_PPSR; // PPC pin state register
    uint32_t m_PPAR; // PPC pin assignment register
    uint32_t m_PSDR; // PPC sleep mode direction reigster
    uint32_t m_PPFR; // PPC pin flag register
    
    uint32_t m_HSCR2; // Serial 2 HSSP Control Register 2
    
    void
    init_register()
    {
      m_PPDR = 0;
      m_PPSR = 0;
      m_PPAR = 0;
      m_PSDR = 0;
      m_PPFR = 0;
      
      m_HSCR2 = 0;
    }
    
  public:
    
    // Bit definition
    
    enum
    {
      PPAR_UPR = (1 << 12), // UART pin reassignment.
      PPAR_SPR = (1 << 18)  // SSP pin reassignment.
    };
    
    // Operation
    
    PPC()
    {
      init_register();
    }
    
    void reset();
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
    
    // Inquiry method
    
    inline uint32_t
    get_PPAR() const
    { return m_PPAR; }
  };
  
  // :SA-1110 Developer's Manual: Wei 2003-Dec-14:
  //
  // All registers in the Peripheral Control Module are accessed via
  // the CPU must be performed using word reads and writes.
  //
  // :NOTE: Wei 2004-Jan-08:
  //
  // However, this facility is hidden to the upper software layer,
  // because of the DMA controller. (see SA-1110 Developer's Manual, Figure 11.1)
  //
  // That is to say, the upper software layout can issue a non-word access to the
  // Peripheral Control Module, and the DMA controller will buffer it, so that the
  // actual access to the Peripheral Control Module is using word access.
  //
  // This facility can NOT be disabled.
  //
  // Thus, because of the performance, I choose to ignore the DMA controller layer,
  // and perform accesses to the virtual hardware (i.e. Peripheral Control Module) directly.
  template<typename T_return>
  T_return
  PPC::get_data(uint32_t const address) const
  {
#if TRACE_PPC
    g_log_file << "PPC: get value at address = " << std::hex << address << std::endl;
#endif
    
    switch (address)
    {
    case PPDR:  return m_PPDR;
    case PPSR:  return m_PPSR;
    case PPAR:  return m_PPAR;
    case PSDR:  return m_PSDR;
    case PPFR:  return m_PPFR;
    case HSCR2: return m_HSCR2;
    default: assert(!"Should not reach here."); return 0;
    }
  }
  
  template<typename T_store>
  void
  PPC::put_data(uint32_t const address, T_store const value)
  {
    switch (address)
    {
    case PPDR:
#if TRACE_PPC
      g_log_file << "PPC: PPDR: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-14:
      //
      // For reserved bits, writes are ignored and reads return "ones".
      //
      // :NOTE: Wei 2004-Jan-13:
      //
      // I have already put 'ones' in the upper 10 bits in reset(),
      // thus I only need to prevent the upper 10 bits from writing,
      // then I can meet the requirement stated above.
      m_PPDR = (value & 0x3FFFFF);
      break;
      
    case PPSR:
#if TRACE_PPC
      g_log_file << "PPC: PPSR: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-14:
      //
      // For reserved bits, writes are ignored and reads return "ones".
      //
      // :NOTE: Wei 2004-Jan-13:
      //
      // I have already put 'ones' in the upper 10 bits in reset(),
      // thus I only need to prevent the upper 10 bits from writing,
      // then I can meet the requirement stated above.
      m_PPSR = (value & 0x3FFFFF);
      break;
      
    case PPAR:
#if TRACE_PPC
      g_log_file << "PPC: PPAR: " << std::hex << value << std::endl;
#endif
      
      // :NOTE: Wei 2004-Jan-03:
      //
      // Only bits 12, 18 are not reserved.
      m_PPAR = (value & ((1 << 18) | (1 << 12)));
      break;
      
    case PSDR:
#if TRACE_PPC
      g_log_file << "PPC: PSDR: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: Wei 2003-Dec-14:
      //
      // For reserved bits, writes are ignored and reads return "zeros".
      //
      // :NOTE: Wei 2004-Jan-13:
      //
      // I have already put 'zeros' in the upper 10 bits in reset(),
      // thus I only need to prevent the upper 10 bits from writing,
      // then I can meet the requirement stated above.
      m_PSDR = (value & 0x3FFFFF);
      break;
      
    case PPFR:
#if TRACE_PPC
      g_log_file << "PPC: PPFR: " << std::hex << value << std::endl;
#endif
      
      // :NOTE: Wei 2004-Jan-03:
      //
      // Only bits 0, 12, 13, 14, 15, 16, 17, 18 are not reserved.
      m_PPFR = (value & ((1 << 18) | (1 << 17) | (1 << 16) | (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 0)));
      break;
      
    case HSCR2:
#if TRACE_PPC
      g_log_file << "PPC: HSCR2: " << std::hex << value << std::endl;
#endif
      
      // :NOTE: Wei 2004-Jan-03:
      //
      // Only bits 18, 19 are not reserved.
      m_HSCR2 = (value & ((1 << 19) | (1 << 18)));
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}

#endif
