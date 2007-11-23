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

#include <iostream>

#include "RTC.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Life cycle =================================
  
  RTC::RTC()
  {
    init_register();
  }
  
  //============================== Operation ==================================
  
  void
  RTC::run()
  {
    // :SA-1110 Developer's Manual: Wei 2003-Dec-08:
    //
    // The counter is incremented on rising edges of the 1-Hz clock.
    ++m_RCNR;
    
    // :SA-1110 Developer's Manual: Wei 2003-Dec-08:
    //
    // If RTAR == RCNR and the enable bit (in status register) is set,
    // then the alarm bit in the RTC status register is set.
    switch (m_RTSR & STATUS_ENABLE_BIT_MASK)
    {
    case 0:
      break;
      
    case STATUS_ALE_BIT:
      if (m_RTAR == m_RCNR)
      {
#if TRACE_RTC
        g_log_file << "RTC: enable alarm: " << std::hex << m_RCNR << std::endl;
#endif
        
        m_RTSR |= STATUS_AL_BIT;
      }
      break;
      
    case STATUS_HZE_BIT:
#if TRACE_RTC
      g_log_file << "RTC: enable hz: " << std::hex << m_RCNR << std::endl;
#endif
      
      m_RTSR |= STATUS_HZ_BIT;
      break;
      
    case (STATUS_ALE_BIT | STATUS_HZE_BIT):
      if (m_RTAR == m_RCNR)
      {
#if TRACE_RTC
        g_log_file << "RTC: enable alarm: " << std::hex << m_RCNR << std::endl;
#endif
        
        m_RTSR |= STATUS_AL_BIT;
      }
      
#if TRACE_RTC
      g_log_file << "RTC: enable hz: " << std::hex << m_RCNR << std::endl;
#endif
      
      m_RTSR |= STATUS_HZ_BIT;
      break;
    }
  }
  
  void
  RTC::reset()
  {
    // :SA-1110 Developer's Manual: Wei 2003-Dec-08:
    //
    // bit (4 ~ 31) of RTSR are set to 0 when reset,
    // bit (0 ~ 3) have unknown value.
    m_RTSR &= STATUS_VALID_BIT_MASK;
    
    m_RTTR = 0;
  }
  
  template<>
  uint32_t
  RTC::get_data(uint32_t const address) const
  {
#if TRACE_RTC
    g_log_file << "RTC: get value at " << std::hex << address << std::endl;
#endif
    
    switch (address)
    {
    case RTAR: return m_RTAR;
    case RCNR: return m_RCNR;
    case RTTR: return m_RTTR;
    case RTSR: return m_RTSR;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<>
  void
  RTC::put_data(uint32_t const address, uint32_t const value)
  {
    switch (address)
    {
    case RTAR:
#if TRACE_RTC
      g_log_file << "RTC: set alarm: " << std::hex << value << std::endl;
#endif
      
      m_RTAR = value;
      break;
      
    case RCNR:
#if TRACE_RTC
      g_log_file << "RTC: set RCNR: " << std::hex << value << std::endl;
#endif
      
      m_RCNR = value;
      break;
      
    case RTTR:
      // :NOTE: Wei 2004-Jun-06:
      //
      // I don't support RTC Trim Procedure yet.
      // Thus just let it pass by.
#if TRACE_RTC
      g_log_file << "RTC: Trim Procedure, using " << value << std::endl;
#endif
      
      m_RTTR = (value & 0x3FFFFFF);
      break;
      
    case RTSR:
#if TRACE_RTC
      g_log_file << "RTC: setting RTSR: orig: " << std::hex << m_RTSR << ", value: " << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: p.90, p.91: Wei 2003-Dec-09:
      //
      // Each status bit may be cleared by writing a one to the status register in the desired bit position.
      // ...
      // The AL & HZ bits (bit 0 & 1) are cleared by writing ones to them.
      // ...
      // All reserved bits are read as 0s and are unaffected by writes.
      m_RTSR &= ~(value & STATUS_VALID_BIT_MASK);
      
#if TRACE_RTC
      g_log_file << "RTC: setting RTSR: new: " << std::hex << m_RTSR << std::endl;
#endif
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}
