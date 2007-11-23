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

#include <cassert>

#include "Tlb.hpp"
#include "Utils/Number.hpp"

namespace ARMware
{
  ////////////////////////////////// Public ///////////////////////////////////
  //============================== Life cycle =================================
  
  Tlb::Tlb()
    : mp_entries(new TlbEntry[HASH_TABLE_BUCKET_NUMBER * HASH_TABLE_BUCKET_SIZE]),
      mp_bucket_start(new uint32_t[HASH_TABLE_BUCKET_NUMBER]),
      mp_bucket_index(new uint32_t[HASH_TABLE_BUCKET_NUMBER])
  {
    // See the notes about Tlb::find_entry().
    // The tlbEntryCount has to be the the power of 2.
    // (i.e. tlbEntryCount == 2^n)
    assert(true == power_of_two(HASH_TABLE_BUCKET_NUMBER));
    assert(true == power_of_two(HASH_TABLE_BUCKET_SIZE));
    
    flush_all();
    
    for (uint32_t i = 0; i < HASH_TABLE_BUCKET_NUMBER; ++i)
    {
      mp_bucket_start[i] = HASH_TABLE_BUCKET_SIZE * i;
    }
  }
  
  Tlb::~Tlb()
  {
    delete[] mp_entries;
    delete[] mp_bucket_start;
    delete[] mp_bucket_index;
  }
  
  //================================ Operation ===============================
  
  TlbEntry *
  Tlb::find_entry(uint32_t const vaddr)
  {
    // :NOTE: Wei 2004-May-15:
    //
    // Because the common bit locations of SECION, LARGE_PAGE, SMALL_PAGE and TINY_PAGE
    // are bits[31:20]. (i.e. Table index of the first level page directory)
    //
    // Thus I shift argument 'vaddr' right 20 bits before doing module operation.
    // (bit 20 is the smallest bit location of bits[31:20])
    //
    // Otherwise,
    // if I use the smallest bit location of table index of tiny page as the shift amount,
    // (i.e. bit 10)
    // there will be duplicate tlb entries in the tlb table.
    uint32_t const bucket = ((vaddr >> MmuBlockTypeTraits<SECTION>::TABLE_INDEX_FIELD_SMALLEST_BIT_LOCATION) &
                             (HASH_TABLE_BUCKET_NUMBER - 1));
    
    uint32_t const start = mp_bucket_start[bucket];
    uint32_t index = mp_bucket_index[bucket];
    assert(index < HASH_TABLE_BUCKET_SIZE);
    uint32_t position;
    
    for (uint32_t i = 0; i < HASH_TABLE_BUCKET_SIZE; ++i)
    {
      position = start + index;
      
      if (true == mp_entries[position].is_valid())
      {
        switch (mp_entries[position].block_type())
        {
        case SECTION:
          if (mp_entries[position].table_idx() == 
              (vaddr & MmuBlockTypeTraits<SECTION>::TABLE_INDEX_BITMASK))
          {
            return &(mp_entries[position]);
          }
          break;
          
        case LARGE_PAGE:
          if (mp_entries[position].table_idx() == 
              (vaddr & MmuBlockTypeTraits<LARGE_PAGE>::TABLE_INDEX_BITMASK))
          {
            return &(mp_entries[position]);
          }
          break;
          
        case SMALL_PAGE:
          if (mp_entries[position].table_idx() == 
              (vaddr & MmuBlockTypeTraits<SMALL_PAGE>::TABLE_INDEX_BITMASK))
          {
            return &(mp_entries[position]);
          }
          break;
          
        case TINY_PAGE:
          if (mp_entries[position].table_idx() == 
              (vaddr & MmuBlockTypeTraits<TINY_PAGE>::TABLE_INDEX_BITMASK))
          {
            return &(mp_entries[position]);
          }
          break;
        }
      }
      
      // :NOTE: Wei 2005-Apr-28:
      //
      // I can NOT write the following codes as
      //
      // index = (--index & (HASH_TABLE_BUCKET_SIZE - 1));
      //
      // Because the above is an undefined result operation.
      // The reason is that the right hand side of an assignment expression will always be
      // calculated before the assignment. However, in the C or C++ standard,
      // there are no guaranteed that the destination calculation will be calculated before
      // the right hand side calculation.
      //
      // Ex: The compiler may translate the above statements into the following 2 code statements:
      //
      // 1)
      //
      // // Determine the destination
      // uint32_t *dest = &index
      //
      // // Calculate the right hand side
      // uint32_t result = (--index & (HASH_TABLE_BUCKET_SIZE - 1))
      //
      // // Perform the assignment
      // *dest = result;
      //
      // 2)
      //
      // // Calculate the right hand side
      // uint32_t result = (--index & (HASH_TABLE_BUCKET_SIZE - 1))
      //
      // // Determine the destination, *however this time 'index' will be smaller than the original by 1*
      // uint32_t *dest = &index
      //
      // // Perform the assignment
      // *dest = result;
      //
      // Thus, the compiler can store the result into 2 different memory addresses,
      // and result in undefined behavior.
      
      --index;
      
      index = (index & (HASH_TABLE_BUCKET_SIZE - 1));
    }
    
    return 0;
  }
  
  TlbEntry *
  Tlb::find_empty_entry(uint32_t const vaddr)
  {
    uint32_t const bucket = ((vaddr >> MmuBlockTypeTraits<SECTION>::TABLE_INDEX_FIELD_SMALLEST_BIT_LOCATION) &
                             (HASH_TABLE_BUCKET_NUMBER - 1));
    
    uint32_t const start = mp_bucket_start[bucket];
    uint32_t index = mp_bucket_index[bucket];
    assert(index < HASH_TABLE_BUCKET_SIZE);
    bool find = false;
    
    for (uint32_t i = 0; i < HASH_TABLE_BUCKET_SIZE; ++i)
    {
      ++index;
      
      index = (index & (HASH_TABLE_BUCKET_SIZE - 1));
      
      if (false == mp_entries[start + index].is_valid())
      {
        find = true;
        break;
      }
    }
    
    if (false == find)
    {
      ++index;
      
      index = (index & (HASH_TABLE_BUCKET_SIZE - 1));
    }
    
    mp_bucket_index[bucket] = index;
    
    return &(mp_entries[start + index]);
  }
}
