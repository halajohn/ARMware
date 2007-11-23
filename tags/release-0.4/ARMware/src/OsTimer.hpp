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

#ifndef OsTimer_hpp
#define OsTimer_hpp

// System include
//

#include <iostream>

#include <cassert>

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  typedef class OsTimer OsTimer;
  class OsTimer
  {
  private:
    
    // Attribute
    
    static uint32_t const WME_BITMASK = 0x1; // Watchdog Match Enable bit mask
    
    bool m_has_enabled_watchdog_timer;
    
    // Register
    
    uint32_t m_OSMR[4]; // OS timer match register 0 ~ 3
    uint32_t m_OSCR; // OS timer counter register
    uint32_t m_OSSR; // OS timer status register
    uint32_t m_OWER; // OS timer watchdog enable register
    uint32_t m_OIER; // OS timer interrupt enable register
    
    // Operation
    
    void
    init_register()
    {
      for (uint32_t i = 0; i < 4; ++i)
      {
        m_OSMR[i] = 0;
      }
      
      m_OSCR = 0;
      m_OSSR = 0;
      m_OWER = 0;
      m_OIER = 0;
    }
    
    template<typename OsmrTraits>
    inline void
    compare_and_set_status()
    {
      if (m_OSCR == m_OSMR[OsmrTraits::INDEX])
      {
#if TRACE_OS_TIMER
        g_log_file << "OsTimer: enable timer[" << OsmrTraits::INDEX << "]: " << std::hex << m_OSMR[OsmrTraits::INDEX] << std::endl;
#endif
        
        m_OSSR |= OsmrTraits::BITMASK;
      }
    }
    
    // Friend
    
    friend class IntController;
    
  public:
    
    // Register location
    
    enum
    {
      OSMR0 = 0x90000000,
      OSMR1 = 0x90000004,
      OSMR2 = 0x90000008,
      OSMR3 = 0x9000000C,
      OSCR  = 0x90000010,
      OSSR  = 0x90000014,
      OWER  = 0x90000018,
      OIER  = 0x9000001C
    };
    
    // OSMR Traits
    
    template<uint32_t OsmrNumber> struct OSMRTraits;
    
    // Life cycle
    
    OsTimer();
    
    // Operation
    
    void reset();
    void run();
    
    inline uint32_t
    get_interrupt_status() const
    {
      return m_OSSR;
    }
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
  
  template<>
  struct OsTimer::OSMRTraits<OsTimer::OSMR0>
  {
    static uint32_t const BITMASK = (1 << 0);
    static uint32_t const INDEX = 0;
  };
  
  template<>
  struct OsTimer::OSMRTraits<OsTimer::OSMR1>
  {
    static uint32_t const BITMASK = (1 << 1);
    static uint32_t const INDEX = 1;
  };
  
  template<>
  struct OsTimer::OSMRTraits<OsTimer::OSMR2>
  {
    static uint32_t const BITMASK = (1 << 2);
    static uint32_t const INDEX = 2;
  };
  
  template<>
  struct OsTimer::OSMRTraits<OsTimer::OSMR3>
  {
    static uint32_t const BITMASK = (1 << 3);
    static uint32_t const INDEX = 3;
  };
  
  // :NOTE: Wei 2004-Jun-06:
  //
  // Specialize for watch dog timer.
  template<>
  inline void
  OsTimer::compare_and_set_status<OsTimer::OSMRTraits<OsTimer::OSMR3> >()
  {
    if (m_OSCR == m_OSMR[OSMRTraits<OSMR3>::INDEX])
    {
      if (true == m_has_enabled_watchdog_timer)
      {
        // :TODO: Wei 2004-Apr-25:
        //
        // The watchdog reset isn't implemented yet.
        assert(!"Should not reach here.");
      }
      else
      {
#if TRACE_OS_TIMER
        g_log_file << "OsTimer: enable timer[3]: " << std::hex << m_OSMR[OSMRTraits<OSMR3>::INDEX] << std::endl;
#endif
        
        m_OSSR |= OsTimer::OSMRTraits<OsTimer::OSMR3>::BITMASK;
      }
    }
  }
  
  template<typename T_return>
  T_return
  OsTimer::get_data(uint32_t const /* address */) const
  {
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  template<>
  uint32_t FASTCALL OsTimer::get_data(uint32_t const address) const;
  
  template<typename T_store>
  void
  OsTimer::put_data(uint32_t const /* address */, T_store const /* value */)
  {
    assert(!"Should not reach here.");
  }
  
  template<>
  void FASTCALL OsTimer::put_data(uint32_t const address, uint32_t const value);
}

#endif
