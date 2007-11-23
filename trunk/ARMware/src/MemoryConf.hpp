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

#ifndef MemoryConf_hpp
#define MemoryConf_hpp

// Project include
//

#include "ARMware.hpp"

namespace ARMware
{
  typedef class MemoryConf MemoryConf;
  class MemoryConf
  {
  private:
    
    enum
    {
      MDCNFG  = 0xA0000000,
      MDCAS00 = 0xA0000004,
      MDCAS01 = 0xA0000008,
      MDCAS02 = 0xA000000C,
      MSC0    = 0xA0000010,
      MSC1    = 0xA0000014,
      MECR    = 0xA0000018,
      MDREFR  = 0xA000001C,
      MDCAS20 = 0xA0000020,
      MDCAS21 = 0xA0000024,
      MDCAS22 = 0xA0000028,
      MSC2    = 0xA000002C,
      SMCNFG  = 0xA0000030
    };
    
    // Attribute
    
    uint32_t mMDCNFG;  // DRAM Configuration register
    uint32_t mMDCAS00; // CAS waveform rotate register 0 for DRAM bank pair 0/1
    uint32_t mMDCAS01; // CAS waveform rotate register 1 for DRAM bank pair 0/1
    uint32_t mMDCAS02; // CAS waveform rotate register 2 for DRAM bank pair 0/1
    uint32_t mMSC0;    // Static memory control register 0
    uint32_t mMSC1;    // Static memory control register 1
    uint32_t mMECR;    // Expansion memory (PC-Card) bus configuration register
    uint32_t mMDREFR;  // DRAM refresh control register
    uint32_t mMDCAS20; // CAS waveform rotate register 0 for DRAM bank pair 2/3
    uint32_t mMDCAS21; // CAS waveform rotate register 1 for DRAM bank pair 2/3
    uint32_t mMDCAS22; // CAS waveform rotate register 2 for DRAM bank pair 2/3
    uint32_t mMSC2;    // Static memory control register 2
    uint32_t mSMCNFG;  // SMROM configuration register
    
    // Operation
    
    void
    init_register()
    {
      mMDCNFG = 0;
      mMDCAS00 = 0;
      mMDCAS01 = 0;
      mMDCAS02 = 0;
      mMSC0 = 0;
      mMSC1 = 0;
      mMECR = 0;
      mMDREFR = 0;
      mMDCAS20 = 0;
      mMDCAS21 = 0;
      mMDCAS22 = 0;
      mMSC2 = 0;
      mSMCNFG = 0;
    }
    
  public:
    
    // Life cycle
    
    MemoryConf()
    {
      init_register();
    }
    
    // Operation
    
    void reset();
    
    // :SA-1110 Developer's Manual: Wei 2003-Dec-11:
    //
    // These registers are readable and writable only as full words.
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
  
  template<typename T_return>
  T_return
  MemoryConf::get_data(uint32_t const /* address */) const
  {
    assert(!"Should not reach here.");
    
    return 0;
  }
  
  template<>
  uint32_t FASTCALL MemoryConf::get_data(uint32_t const address) const;
  
  template<typename T_store>
  void
  MemoryConf::put_data(uint32_t const /* address */, T_store const /* value */)
  { assert(!"Should not reach here."); }
  
  template<>
  void FASTCALL MemoryConf::put_data(uint32_t const address, uint32_t const value);
}

#endif
