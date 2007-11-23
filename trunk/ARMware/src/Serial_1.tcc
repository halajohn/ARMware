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

#include "Serial_1.hpp"

namespace ARMware
{  
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
  Serial_1::get_data(uint32_t const address)
  {
#if TRACE_SERIAL_1
    g_log_file << "SERIAL 1: get value at address " << std::hex << address << std::endl;
#endif
    
    switch (address)
    {
    case GPCLKR0:
      // Bits 1, 2, 3, 6, 7 are reserved.
      return static_cast<T_return>(m_GPCLKR0 & (GPCLKR0_SUS | GPCLKR0_SCE | GPCLKR0_SCD));
      
    case GPCLKR1:
      // Only bit 1 is not reserved.
      return static_cast<T_return>(m_GPCLKR1 & GPCLKR1_TXE);
      
    case GPCLKR2:
      // Bits 4, 5, 6, 7 are reserved.
      return static_cast<T_return>(m_GPCLKR2 & 0xF);
      
    case GPCLKR3:
      return static_cast<T_return>(m_GPCLKR3);
      
    default:
      return m_UART.get_data<T_return>(address);
    }
  }
  
  template<typename T_store>
  void
  Serial_1::put_data(uint32_t const address, T_store const value)
  {
#if TRACE_SERIAL_1
    g_log_file << "SERIAL 1: put " << std::hex << value << " at address " << address << std::endl;
#endif
    
    switch (address)
    {
    case GPCLKR0:
      // :SA-1110 Developer's Manual: p.296: Wei 2003-Dec-14:
      //
      // The GPCLK must be disabled (SUS == 0) when changing the state 
      // of any bit within this register.
      if (value != GPCLKR0_SUS)
      {
        assert(0 == (m_GPCLKR0 & GPCLKR0_SUS));
      }
      
      m_GPCLKR0 = static_cast<uint8_t>(value & (GPCLKR0_SUS | GPCLKR0_SCE | GPCLKR0_SCD));
      break;
      
    case GPCLKR1:
      m_GPCLKR1 = static_cast<uint8_t>(value & GPCLKR1_TXE);
      break;
      
    case GPCLKR2:
      // :SA-1110 Developer's Manual: p.298: Wei 2003-Dec-14:
      //
      // The GPCLK must be disabled (SUS == 0) whenever these registers are written.
      assert(0 == (m_GPCLKR0 & GPCLKR0_SUS));
      
      m_GPCLKR2 = static_cast<uint8_t>(value & 0xF);
      break;
      
    case GPCLKR3:
      // :SA-1110 Developer's Manual: p.298: Wei 2003-Dec-14:
      //
      // The GPCLK must be disabled (SUS == 0) whenever these registers are written.
      assert(0 == (m_GPCLKR0 & GPCLKR0_SUS));
      
      m_GPCLKR3 = static_cast<uint8_t>(value);
      break;
      
    default:
      m_UART.put_data(address, value);
      break;
    }
  }
}
