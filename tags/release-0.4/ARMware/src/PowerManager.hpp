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

#ifndef PowerManager_hpp
#define PowerManager_hpp

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  typedef class PowerManager PowerManager;
  class PowerManager
  {
  private:
    
    enum
    {
      PMCR = 0x90020000,
      PSSR = 0x90020004,
      PSPR = 0x90020008,
      PWER = 0x9002000C,
      PCFR = 0x90020010,
      PPCR = 0x90020014,
      PGSR = 0x90020018,
      POSR = 0x9002001C
    };
    
    enum
    {
      POSR_OOK = (1 << 0)
    };
    
    // Attribute
    
    uint32_t mPMCR; // Power manager control register
    uint32_t mPSSR; // Power manager sleep status register
    uint32_t mPSPR; // Power manager scratch pad register
    uint32_t mPWER; // Power manager wake-up enable register;
    uint32_t mPCFR; // Power manager general configuration register
    uint32_t mPPCR; // Power manager PLL configuration register
    uint32_t mPGSR; // Power manager GPIO sleep state register
    uint32_t mPOSR; // Power manager oscillator status register
    
    // Operation
    
    void
    init_register()
    {
      mPMCR = 0;
      mPSSR = 0;
      mPSPR = 0;
      mPWER = 0;
      mPCFR = 0;
      mPPCR = 0;
      mPGSR = 0;
      mPOSR = 0;
    }
    
  public:
    
    PowerManager()
    {
      init_register();
    }
    
    void reset();
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
  
  template<typename T_return>
  T_return
  PowerManager::get_data(uint32_t const /* address */) const
  {
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  template<>
  uint32_t FASTCALL PowerManager::get_data(uint32_t const address) const;
  
  template<typename T_store>
  void
  PowerManager::put_data(uint32_t const /* address */, T_store const /* value */)
  {
    assert(!"Should not reach here.");
  }
  
  template<>
  void FASTCALL PowerManager::put_data(uint32_t const address, uint32_t const value);
}

#endif
