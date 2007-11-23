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

// Translation lookahead buffer
//

#ifndef Tlb_hpp
#define Tlb_hpp

#include <iostream>

// Project include
//

#include "ARMware.hpp"

// Local include
//

#include "TlbEntry.hpp"

namespace ARMware
{
  typedef class Tlb Tlb;
  class Tlb
  {
  private:
    
    // Attribute
    
    // :NOTE: Wei 2004-Jan-15:
    //
    // Althought the TLB entries amount in the specification of Intel SA-1110 is 32,
    // however, due to performance consideration, I increment the amount.
    //
    // :NOTE: Wei 2004-May-08:
    //
    // See the notes about Tlb::find_entry().
    // The tlbEntryCount has to be the the power of 2.
    // (i.e. tlbEntryCount == 2^n)
    static uint32_t const HASH_TABLE_BUCKET_SIZE = 2;
    static uint32_t const HASH_TABLE_BUCKET_NUMBER = 256;
    
    TlbEntry * const mp_entries;
    uint32_t * const mp_bucket_start;
    uint32_t * const mp_bucket_index;
    
  public:
    
    // Life cycle
    
    Tlb();
    ~Tlb();
    
    // Operation
    
    inline void
    flush_all()
    {
      memset(mp_entries, 0, sizeof(TlbEntry) * (HASH_TABLE_BUCKET_NUMBER * HASH_TABLE_BUCKET_SIZE));
      memset(mp_bucket_index, 0, sizeof(uint32_t) * HASH_TABLE_BUCKET_NUMBER);
    }
    
    inline void
    flush_single_entry(uint32_t const address)
    {
      TlbEntry * const entry = find_entry(address);
      
      if (entry != 0)
      {
#if TRACE_TLB
        g_log_file << "TLB: flush_single_entry success: " << std::hex << address << std::endl;
#endif
        
        entry->set_valid(false);
      }
#if TRACE_TLB
      else
      {
        g_log_file << "TLB: flush_single_entry failed: " << std::hex << address << std::endl;
      }
#endif
    }
    
    TlbEntry *find_entry(uint32_t const vaddr) FASTCALL;
    TlbEntry *find_empty_entry(uint32_t const vaddr) FASTCALL;
  };
}

#endif
