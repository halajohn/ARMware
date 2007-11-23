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

#include "OsTimer.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //=========================== Life cycle ====================================
  
  OsTimer::OsTimer()
  {
    init_register();
  }
  
  //============================== Operation ==================================
  
  void
  OsTimer::reset()
  {
    // :SA-1110 Developer's Manual: Wei 2004-Apr-24:
    //
    // WME (Watchdog Match Enable) bit is set by writing a one to it.
    // It can only be cleared by one of the reset functions (hardware reset,
    // software reset) and by entering sleep mode.
    // A watchdog reset also clears the watchdog enable bit.
    m_OWER = 0;
    m_has_enabled_watchdog_timer = false;
    
    // :SA-1110 Developer's Manual: p.97: Wei 2004-Apr-24:
    //
    // the value of bits 0 ~ 3 of OSSR register are unknown at reset.
    m_OSSR &= 0xF;
    
    m_OIER = 0;
  }
  
  void
  OsTimer::run()
  {
    // :SA-1110 Developer's Manual: p.96: Wei 2004-Apr-24:
    //
    // OSMR[0..3] are compared against the OSCR following every rising
    // edge of the 3.6864-MHz clock.
    switch (m_OIER & 0xF)
    {
    case 0:
      break;
      
    case 0x1: // 0001
      compare_and_set_status<OSMRTraits<OSMR0> >();
      break;
      
    case 0x2: // 0010
      compare_and_set_status<OSMRTraits<OSMR1> >();
      break;
      
    case 0x3: // 0011
      compare_and_set_status<OSMRTraits<OSMR0> >();
      compare_and_set_status<OSMRTraits<OSMR1> >();
      break;
      
    case 0x4: // 0100
      compare_and_set_status<OSMRTraits<OSMR2> >();
      break;
      
    case 0x5: // 0101
      compare_and_set_status<OSMRTraits<OSMR0> >();
      compare_and_set_status<OSMRTraits<OSMR2> >();
      break;
      
    case 0x6: // 0110
      compare_and_set_status<OSMRTraits<OSMR1> >();
      compare_and_set_status<OSMRTraits<OSMR2> >();
      break;
      
    case 0x7: // 0111
      compare_and_set_status<OSMRTraits<OSMR0> >();
      compare_and_set_status<OSMRTraits<OSMR1> >();
      compare_and_set_status<OSMRTraits<OSMR2> >();
      break;
      
    case 0x8: // 1000
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    case 0x9: // 1001
      compare_and_set_status<OSMRTraits<OSMR0> >();
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    case 0xA: // 1010
      compare_and_set_status<OSMRTraits<OSMR1> >();
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    case 0xB: // 1011
      compare_and_set_status<OSMRTraits<OSMR0> >();
      compare_and_set_status<OSMRTraits<OSMR1> >();
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    case 0xC: // 1100
      compare_and_set_status<OSMRTraits<OSMR2> >();
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    case 0xD: // 1101
      compare_and_set_status<OSMRTraits<OSMR0> >();
      compare_and_set_status<OSMRTraits<OSMR2> >();
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    case 0xE: // 1110
      compare_and_set_status<OSMRTraits<OSMR1> >();
      compare_and_set_status<OSMRTraits<OSMR2> >();
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    case 0xF: // 1111
      compare_and_set_status<OSMRTraits<OSMR0> >();
      compare_and_set_status<OSMRTraits<OSMR1> >();
      compare_and_set_status<OSMRTraits<OSMR2> >();
      compare_and_set_status<OSMRTraits<OSMR3> >();
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    // :SA-1110 Developer's Manual: p.96: Wei 2004-Apr-24:
    //
    // The OS timer count register is a 32-bit counter that increments
    // on rising edges of the 3.6864-MHz clock.
    
    // :NOTE: Wei 2004-Apr-25:
    //
    // According to the Linux Kernel source about the setup_timer() routine,
    // I think the increment of OSCR register should be put after the
    // comparation between OSCR & OSMR.
    
    ++m_OSCR;
  }
  
  template<>
  uint32_t
  OsTimer::get_data(uint32_t const address) const
  {
    switch (address)
    {
    case OSMR0:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get timer[0]: " << std::hex << m_OSMR[OSMRTraits<OSMR0>::INDEX] << std::endl;
#endif
      return m_OSMR[OSMRTraits<OSMR0>::INDEX];
      
    case OSMR1:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get timer[1]: " << std::hex << m_OSMR[OSMRTraits<OSMR1>::INDEX] << std::endl;
#endif
      return m_OSMR[OSMRTraits<OSMR1>::INDEX];
      
    case OSMR2:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get timer[2]: " << std::hex << m_OSMR[OSMRTraits<OSMR2>::INDEX] << std::endl;
#endif
      return m_OSMR[OSMRTraits<OSMR2>::INDEX];
      
    case OSMR3:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get timer[3]: " << std::hex << m_OSMR[OSMRTraits<OSMR3>::INDEX] << std::endl;
#endif
      return m_OSMR[OSMRTraits<OSMR3>::INDEX];
      
    case OSCR:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get OSCR: " << std::hex << m_OSCR << std::endl;
#endif
      return m_OSCR;
      
    case OSSR:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get OSSR: " << std::hex << m_OSSR << std::endl;
#endif
      return m_OSSR;
      
    case OWER:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get OWER: " << std::hex << m_OWER << std::endl;
#endif
      return m_OWER;
      
    case OIER:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: get OIER: " << std::hex << m_OIER << std::endl;
#endif
      return m_OIER;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<>
  void
  OsTimer::put_data(uint32_t const address, uint32_t const value)
  {
    switch (address)
    {
    case OSMR0:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set timer[0]: " << std::hex << value << std::endl;
#endif
      
      m_OSMR[OSMRTraits<OSMR0>::INDEX] = value;
      break;
      
    case OSMR1:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set timer[1]: " << std::hex << value << std::endl;
#endif
      
      m_OSMR[OSMRTraits<OSMR1>::INDEX] = value;
      break;
      
    case OSMR2:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set timer[2]: " << std::hex << value << std::endl;
#endif
      
      m_OSMR[OSMRTraits<OSMR2>::INDEX] = value;
      break;
      
    case OSMR3:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set timer[3]: " << std::hex << value << std::endl;
#endif
      
      m_OSMR[OSMRTraits<OSMR3>::INDEX] = value;
      break;
      
    case OSCR:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set OSCR: " << std::hex << value << std::endl;
#endif
      
      m_OSCR = value;
      break;
      
    case OSSR:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set OSSR: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: p.97: Wei 2004-Apr-24:
      //
      // bits 0 ~ 3 of OSSR register are cleared by writing a one to the
      // proper bit position. Writing zeros to this register has no effect.
      // All reserved bits read as zeros and are unaffected by writes;
      switch (value & 0xF)
      {
      case 0x0: // 0000
        break;
        
      case 0x1: // 0001
        m_OSSR &= ~OSMRTraits<OSMR0>::BITMASK;
        break;
        
      case 0x2: // 0010
        m_OSSR &= ~OSMRTraits<OSMR1>::BITMASK;
        break;
        
      case 0x3: // 0011
        m_OSSR &= ~(OSMRTraits<OSMR0>::BITMASK |
                    OSMRTraits<OSMR1>::BITMASK);
        break;
        
      case 0x4: // 0100
        m_OSSR &= ~OSMRTraits<OSMR2>::BITMASK;
        break;
        
      case 0x5: // 0101
        m_OSSR &= ~(OSMRTraits<OSMR0>::BITMASK |
                    OSMRTraits<OSMR2>::BITMASK);
        break;
        
      case 0x6: // 0110
        m_OSSR &= ~(OSMRTraits<OSMR1>::BITMASK |
                    OSMRTraits<OSMR2>::BITMASK);
        break;
        
      case 0x7: // 0111
        m_OSSR &= ~(OSMRTraits<OSMR0>::BITMASK |
                    OSMRTraits<OSMR1>::BITMASK |
                    OSMRTraits<OSMR2>::BITMASK);
        break;
        
      case 0x8: // 1000
        m_OSSR &= ~OSMRTraits<OSMR3>::BITMASK;
        break;
        
      case 0x9: // 1001
        m_OSSR &= ~(OSMRTraits<OSMR0>::BITMASK |
                    OSMRTraits<OSMR3>::BITMASK);
        break;
        
      case 0xA: // 1010
        m_OSSR &= ~(OSMRTraits<OSMR1>::BITMASK |
                    OSMRTraits<OSMR3>::BITMASK);
        break;
        
      case 0xB: // 1011
        m_OSSR &= ~(OSMRTraits<OSMR0>::BITMASK |
                    OSMRTraits<OSMR1>::BITMASK |
                    OSMRTraits<OSMR3>::BITMASK);
        break;
        
      case 0xC: // 1100
        m_OSSR &= ~(OSMRTraits<OSMR2>::BITMASK |
                    OSMRTraits<OSMR3>::BITMASK);
        break;
        
      case 0xD: // 1101
        m_OSSR &= ~(OSMRTraits<OSMR0>::BITMASK |
                    OSMRTraits<OSMR2>::BITMASK |
                    OSMRTraits<OSMR3>::BITMASK);
        break;
        
      case 0xE: // 1110
        m_OSSR &= ~(OSMRTraits<OSMR1>::BITMASK |
                    OSMRTraits<OSMR2>::BITMASK |
                    OSMRTraits<OSMR3>::BITMASK);
        break;
        
      case 0xF: // 1111
        m_OSSR &= ~(OSMRTraits<OSMR0>::BITMASK |
                    OSMRTraits<OSMR1>::BITMASK |
                    OSMRTraits<OSMR2>::BITMASK |
                    OSMRTraits<OSMR3>::BITMASK);
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case OWER:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set OWER: " << std::hex << value << std::endl;
#endif
      
      // :SA-1110 Developer's Manual: p.96: Wei 2004-Apr-24:
      //
      // WME (Watchdog Match Enable) bit is set by writing a one to it.
      // It can only be cleared by one of the reset functions (hardware reset,
      // software reset) and by entering sleep mode.
      // A watchdog reset also clears the watchdog enable bit.
      if (false == m_has_enabled_watchdog_timer)
      {
        if (WME_BITMASK == (value & WME_BITMASK))
        {
          // :SA-1110 Developer's Manual: p.98: Wei 2004-Apr-25:
          //
          // The user must clear OSSR:M3 before setting up a watchdog reset.
          assert(0 == (m_OSSR & OSMRTraits<OSMR3>::BITMASK));
          
          m_OWER |= WME_BITMASK;
          m_has_enabled_watchdog_timer = true;
        }
      }
      break;
      
    case OIER:
#if TRACE_OS_TIMER
      g_log_file << "OsTimer: set OIER: " << std::hex << value << std::endl;
#endif
      
      // :NOTE: Wei 2004-Apr-24:
      //
      // According to OSSR register: All reserved bits read as zeros and are unaffected by writes
      // I think OIER is the same with OSSR.
      m_OIER = (value & 0xF);
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}
