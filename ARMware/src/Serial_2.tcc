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

#include "Serial_2.hpp"

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
  Serial_2::get_data(uint32_t const address)
  {
#if TRACE_SERIAL_2
    g_log_file << "SERIAL 2: get value at address " << std::hex << address << std::endl;
#endif
    
    switch (address)
    {      
    case HSCR0: return static_cast<T_return>(m_HSCR0);
    case HSCR1: return static_cast<T_return>(m_HSCR1);
    case HSDR:  return static_cast<T_return>(m_HSDR);
      
    case HSSR0:
      // Bits 6, 7 are reserved.
      return static_cast<T_return>(m_HSSR0 & ~((1 << 7) | (1 << 6)));
      
    case HSSR1:
      // Bit 7 is reserved.
      return static_cast<T_return>(m_HSSR1 & ~(1 << 7));
      
    case UTCR4:
      // Only bits 0, 1 are not reserved.
      return static_cast<T_return>(m_UTCR4 & ((1 << 1) | (1 << 0)));
      
    default:
      return m_UART.get_data<T_return>(address);
    }
  }
  
  template<typename T_store>
  void
  Serial_2::put_data(uint32_t const address, T_store const value)
  {
#if TRACE_SERIAL_2
    g_log_file << "SERIAL 2: put " << std::hex << value << " at address " << address << std::endl;
#endif
    
    switch (address)
    {
    case HSCR0: m_HSCR0 = static_cast<uint8_t>(value); break;
    case HSCR1: m_HSCR1 = static_cast<uint8_t>(value); break;
    case HSDR:  m_HSDR = static_cast<uint8_t>(value); break;
      
    case HSSR0:
      // bits 0, 3, 4 are read-only.
      // bits 6, 7 are reserved.
      //
      // Thus I only can write value to bits 1, 2, 5
      m_HSSR0 = static_cast<uint8_t>( value & ((1 << 1) | (1 << 2) | (1 << 5)) );
      break;
      
    case HSSR1:
      // read-only register.
      break;
      
    case UTCR4:
      m_UTCR4 = static_cast<uint8_t>(value); break;
      break;
      
    default:
      m_UART.put_data(address, value);
      break;
    }
  }
}
