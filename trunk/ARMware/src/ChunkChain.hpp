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

#ifndef ChunkChain_hpp
#define ChunkChain_hpp

#if ENABLE_THREADED_CODE

#include "ARMware.hpp"
#include "Chunk.hpp"
#include "HashTable.hpp"
#include "Core.hpp"
#include "MachineType.hpp"

namespace ARMware
{
  typedef class ChunkChain ChunkChain;
  class ChunkChain
  {
  private:
    
    // :NOTE: Wei 2004-Aug-12:
    //
    // The reason why MachineType::DRAM_SIZE needs to plus 1 is because MachineType::DRAM_SIZE
    // is started at 0, thus the real size needs to plus 1.
    //
    // And the reason why I use NATIVE_PAGE_SIZE as the divisor is because
    // when some codes need to access a memory region which I protect it from writing (use mprotect()),
    // I will use mprotect() to un-protect it again and allow the accessing,
    // and the memory unit which mprotect() operate on is a native page (in NATIVE_PAGE_SIZE bytes).
    //
    // Further more, I will flush the basic blocks located in that native page,
    // thus, I use NATIVE_PAGE_SIZE as the divisor to collect basic blocks located in the same native page
    // all together.
    //
    // Comparing to the one level hashing, this facility can simplify the basic blocks flushing.
    static uint32_t const TABLE_ENTRY_COUNT = ((MachineType::DRAM_SIZE + 1) / NATIVE_PAGE_SIZE);
    
    // :NOTE: Wei: 2004-Aug-01:
    //
    // Because the minimal size of an page is 1 kb.
    // And in order to simplify the calculation of the prefetch exception,
    // I enforce every basic block doesn't across the 1 kb page boundary.
    //
    // Thus, I break up the whole memory space into 1kb pages.
    HashTable<Chunk> *mp_chunk_table[TABLE_ENTRY_COUNT];
    
    Core * const mp_core;
    Chunk *mp_curr_chunk;
    
    // Operation
    
    inline uint32_t
    cal_table_idx(uint32_t const paddr) const
    {
      // :NOTE: Wei 2004-Aug-12:
      //
      // This value (12) is based on the native page size == 4096.
      // If native page size is changed, I need to change this value (12), too.
      return (paddr >> 12);
    }
    
  public:
    
    // Life cycle
    
    ChunkChain(Core * const core)
      : mp_core(core),
        mp_curr_chunk(0)
    {
      for (uint32_t i = 0; i < TABLE_ENTRY_COUNT; ++i)
      {
        mp_chunk_table[i] = 0;
      }
    }
    
    ~ChunkChain()
    {
      for (uint32_t i = 0; i < TABLE_ENTRY_COUNT; ++i)
      {
        if (mp_chunk_table[i] != 0)
        {
          delete mp_chunk_table[i];
        }
      }
    }
    
    // Operation
    
    inline Chunk *
    find_chunk(uint32_t const paddr)
    {
      assert(paddr <= MachineType::DRAM_SIZE);
      
      uint32_t const table_idx = cal_table_idx(paddr);
      
      assert(table_idx < TABLE_ENTRY_COUNT);
      
      if (0 == mp_chunk_table[table_idx])
      {
        mp_chunk_table[table_idx] = new HashTable<Chunk>;
      }
      
      mp_curr_chunk = mp_chunk_table[table_idx]->find_one(paddr);
      
      return mp_curr_chunk;
    }
    
    inline void
    flush_chunk(uint32_t const paddr)
    {
      assert(paddr <= MachineType::DRAM_SIZE);
      assert(mp_curr_chunk != 0);
      assert(mp_curr_chunk == mp_core->curr_chunk());
      
      uint32_t const table_idx = cal_table_idx(paddr);
      Chunk *excluded_chunk = 0;
      
      assert(table_idx < TABLE_ENTRY_COUNT);
      
      if (mp_chunk_table[table_idx] != 0)
      {
        if (cal_table_idx(mp_curr_chunk->start_paddr()) == table_idx)
        {
          // :NOTE: Wei 2004-Aug-12:
          //
          // Notify we are flushing the current basic block.
          //
          // Because I just want to notify the end of a basic block,
          // don't want to set the length of any basic block.
          // Thus, I should use 'reset_chunk()' rather than 'finalize_chunk()'.
          mp_core->reset_chunk();
          
          mp_core->set_delay_clean_chunk(mp_curr_chunk);
          
          excluded_chunk = mp_curr_chunk;
        }
        
        mp_chunk_table[table_idx]->flush_all(excluded_chunk);
      }
    }
  };
}
#endif // ENABLE_THREADED_CODE

#endif
