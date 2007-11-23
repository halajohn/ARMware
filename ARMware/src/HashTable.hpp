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

#ifndef HashTable_hpp
#define HashTable_hpp

#if ENABLE_THREADED_CODE

#include "MemoryPool.hpp"

namespace ARMware
{
  template<typename T>
  class HashTable : public MemoryPool<HashTable<T>, 128>
  {
  private:
    
    static uint32_t const HASH_TABLE_ENTRY_COUNT = 16;
    
    T *mp_hash_table[HASH_TABLE_ENTRY_COUNT];
    
    uint32_t m_curr_walk_idx;
    T *m_curr_walk_item;
    
  public:
    
    // Life cycle
    
    inline
    HashTable()
    {
      for (uint32_t i = 0; i < HASH_TABLE_ENTRY_COUNT; ++i)
      {
        mp_hash_table[i] = 0;
      }
    }
    
    ~HashTable()
    {
      flush_all();
    }
    
    // Operation
    
    inline T *
    find_one_no_create(uint32_t const value, uint32_t *attach_idx)
    {
      uint32_t const curr_idx = (value & (HASH_TABLE_ENTRY_COUNT - 1));
      
      T *t = mp_hash_table[curr_idx];
      
      while (t != 0)
      {
        if (value == t->hash_value())
        {
          return t;
        }
        else
        {
          t = t->next_used_one();
        }
      }
      
      assert(0 == t);
      
      if (attach_idx != 0)
      {
        *attach_idx = curr_idx;
      }
      
      return 0;
    }
    
    inline T *
    find_one(uint32_t const value)
    {
      uint32_t attach_idx;
      
      T *t = find_one_no_create(value, &attach_idx);
      
      if (0 == t)
      {
        // :NOTE: Wei 2004-Aug-02:
        //
        // I can't find a previous T.
        // Create a new one.
        t = new T(value);
        
        // :NOTE: Wei 2004-Aug-17:
        //
        // Add to head.
        t->set_next_used_one(mp_hash_table[attach_idx]);
        mp_hash_table[attach_idx] = t;
      }
      
      return t;
    }
    
    inline void
    add_new_element(T * const t, uint32_t const idx)
    {
      t->set_next_used_one(mp_hash_table[idx]);
      mp_hash_table[idx] = t;
    }
    
    inline void
    flush_all(T * const excluded = 0)
    {
#if CHECK_CHUNK_CODE
      bool find = false;
#endif
      
      for (uint32_t i = 0; i < HASH_TABLE_ENTRY_COUNT; ++i)
      {
        T *t = mp_hash_table[i];
        T *prev_t;
        
        while (t != 0)
        {
          prev_t = t;
          t = t->next_used_one();
          
          if (prev_t != excluded)
          {            
            // :NOTE: Wei 2005-May-16:
            //
            // Because I still need to use the internal data of the current chunk,
            // thus I should not delete the current chunk until I go back to the interpreter.
            delete prev_t;
          }
#if CHECK_CHUNK_CODE
          else
          {
            assert(false == find);
            
            find = true;
          }
#endif
        }
        
        // :NOTE: Wei 2005-May-16:
        //
        // I decide to set all the mp_has_table[] array element to 0 now,
        // so that I don't need to back to here to set an entry to 0
        // when I go back to the interpreter in the situation of HashTable<Chunk>.
        mp_hash_table[i] = 0;
      }
      
#if CHECK_CHUNK_CODE
      if (excluded != 0)
      {
        assert(true == find);
      }
#endif
    }
  };
}
#endif // ENABLE_THREADED_CODE

#endif
