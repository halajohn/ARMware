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

#ifndef MemoryPool2_hpp
#define MemoryPool2_hpp

#if ENABLE_THREADED_CODE

#include "Type.hpp"

#include <cstddef> // for std::size_t

namespace ARMware
{
  // :NOTE: Wei 2004-Jul-26:
  //
  // The value of this variable should be power of 2.
  //
  // Because of in the following 'operator new' operator,
  // I will use shift left operation instead of multiply operation.
  //
  // And this substitution is only valid for a number of power of 2.
  template<typename T, uint32_t T_increment_count>
  class MemoryPool2
  {
  private:
    
    static T *msp_pool;
    
#if CHECK_CHUNK_CODE
    static uint32_t m_element_num;
#endif
    
  protected:
    
    T *mp_next_free_node;
    
  public:
    
    // Operator
    
    inline void *
    operator new(std::size_t /* size */)
    {
      if (0 == msp_pool)
      {
        // :NOTE: Wei 2004-Aug-13:
        //
        // Because 'T_increment_count' and 'sizeof' are constants at compile time,
        // thus the multiply should be eliminate at compile time.
        //
        // :NOTE: Wei 2004-Aug-13:
        //
        // I confirm GCC-3.3 has this behavior. GCC-3.3 will eliminate the multiply
        // and replace it with the result constant value.
        T *t = static_cast<T *>(::operator new(sizeof(T) * T_increment_count));
        
        for (uint32_t i = 0; i < T_increment_count; ++i)
        {
          t->mp_next_free_node = msp_pool;
          
          msp_pool = t;
          
          ++t;
        }
        
#if CHECK_CHUNK_CODE
        m_element_num += T_increment_count;
#endif
      }
      
      T * const t = msp_pool;
      
      msp_pool = msp_pool->mp_next_free_node;
      
#if CHECK_CHUNK_CODE
      t->mp_next_free_node = 0;
#endif
      
      return static_cast<void *>(t);
    }
    
    inline void
    operator delete(void * const t_arg)
    {
      T * const t = static_cast<T *>(t_arg);
      
#if CHECK_CHUNK_CODE
      assert(0 == t->mp_next_free_node);
#endif
      
      t->mp_next_free_node = msp_pool;
      msp_pool = t;
    }
    
#if CHECK_CHUNK_CODE
    static void
    check_delete_all_into_memory_pool()
    {
      uint32_t m_num = 0;
      
      for (T const *curr_t = msp_pool;
           curr_t != 0;
           curr_t = curr_t->mp_next_free_node)
      {
        ++m_num;
      }
      
      assert(m_num == m_element_num);
    }
#endif
  };
  
  template<typename T, uint32_t T_increment_count>
  T *MemoryPool2<T, T_increment_count>::msp_pool = 0;
  
#if CHECK_CHUNK_CODE
  template<typename T, uint32_t T_increment_count>
  uint32_t MemoryPool2<T, T_increment_count>::m_element_num = 0;
#endif
}
#endif // ENABLE_THREADED_CODE

#endif
