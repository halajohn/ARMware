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

#if TRACE_MEMORY_CONF
#include <iostream>
#endif

#include <cassert>
#include "MemoryConf.hpp"

namespace ARMware
{
  //////////////////////////////// Public /////////////////////////////////////
  //============================== Operation ==================================
  
  void
  MemoryConf::reset()
  {
    // :NOTE: Wei 2004-Jan-18:
    //
    // Bits 0, 1, 16, 17 will reset to 0 when reset.
    mMDCNFG &= ~((1 << 0) | (1 << 1) | (1 << 16) | (1 << 17));
    
    // :NOTE: Wei 2003-Dec-11:
    //
    // I assume all of the bits which have unknown values when reset have value 0.
    mMDREFR = static_cast<uint32_t>((1 << 31) | (1 << 26) | (1 << 22) | (1 << 18));
    
    // :NOTE: Wei 2003-Dec-11:
    //
    // I assume all of the bits which have unknown values when reset have value 0.
    mMDCAS00 = (1 << 10) | (1 << 8) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | 1;
    mMDCAS01 = mMDCAS00;
    mMDCAS02 = mMDCAS00;
    mMDCAS20 = mMDCAS00;
    mMDCAS21 = mMDCAS00;
    mMDCAS22 = mMDCAS00;
    
    // :NOTE: Wei 2003-Dec-11:
    //
    // I assume all of the bits which have unknown values when reset have value 0.
    //
    // :SA-1110 Developer's Manual: p.141: Wei 2004-Jun-09:
    //
    // On iPaq H3600, RBW0 have to be 0 to indicate 32-bit 'ROM bus width'.
    mMSC0 = (1 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10)
      | (1 << 9) | (1 << 8) | (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3);
    
    // :NOTE: Wei 2003-Dec-11:
    //
    // I assume all of the bits which have unknown values when reset have value 0.
    mSMCNFG = (1 << 15) | (1 << 14) | (1 << 6);
  }
  
  template<>
  uint32_t
  MemoryConf::get_data(uint32_t const address) const
  {
#if TRACE_MEMORY_CONF
    g_log_file << "MEM CONF: read address: " << std::hex << address << std::endl;
#endif
    
    switch (address)
    {
    case MDCNFG:  return mMDCNFG;
    case MDCAS00: return mMDCAS00;
    case MDCAS01: return mMDCAS01;
    case MDCAS02: return mMDCAS02;
    case MSC0:    return mMSC0;
    case MSC1:    return mMSC1;
    case MECR:    return mMECR;
    case MDREFR:  return mMDREFR;
    case MDCAS20: return mMDCAS20;
    case MDCAS21: return mMDCAS21;
    case MDCAS22: return mMDCAS22;
    case MSC2:    return mMSC2;
    case SMCNFG:  return mSMCNFG;
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<>
  void
  MemoryConf::put_data(uint32_t const address, uint32_t const value)
  {
#if TRACE_MEMORY_CONF
    g_log_file << "MEM CONF: write address: " << std::hex << address << ", value: " << value << std::endl;
#endif
    
    switch (address)
    {
    case MDCNFG:  mMDCNFG = value; break;
    case MDCAS00: mMDCAS00 = value; break;
    case MDCAS01: mMDCAS01 = value; break;
    case MDCAS02: mMDCAS02 = value; break;
    case MSC0:    mMSC0 = value; break;
    case MSC1:    mMSC1 = value; break;
    case MECR:    mMECR = value; break;
    case MDREFR:  mMDREFR = value; break;
    case MDCAS20: mMDCAS20 = value; break;
    case MDCAS21: mMDCAS21 = value; break;
    case MDCAS22: mMDCAS22 = value; break;
    case MSC2:    mMSC2 = value; break;
    case SMCNFG:  mSMCNFG = value; break;
    default: assert(!"Should not reach here."); break;
    }
  }
}
