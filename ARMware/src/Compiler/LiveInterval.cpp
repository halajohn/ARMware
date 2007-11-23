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

#if ENABLE_DYNAMIC_TRANSLATOR

#include <algorithm>

#if PRINT_CHUNK_CODE
#include "../Log.hpp"
#endif

#include "LiveInterval.hpp"
#include "BitPosition.hpp"

namespace ARMware
{
  LiveInterval::LiveInterval(uint32_t const start,
                             uint32_t const end,
                             Variable * const var)
    : mp_preallocated_reg(var->preallocated_reg()),
      mp_attached_var(var)
  {
    m_range.push_back(LiveRange(start, end));
    
    m_excluded_reg = var->excluded_reg();
  }
  
  LiveInterval::~LiveInterval()
  {
    if (mp_preallocated_reg != 0)
    {
      delete mp_preallocated_reg;
    }
  }
  
  inline void
  LiveInterval::set_used_bit(std::vector<uint32_t> &bitset,
                             uint32_t const start,
                             uint32_t const end)
  {
    for (uint32_t i = start; i < (end + 1); ++i)
    {
      uint32_t vector_idx;
      uint32_t bit_position;
      
      calculate_bit_position(i, vector_idx, bit_position);
      
      assert(bitset.size() > vector_idx);
      
      bitset[vector_idx] |= bit_position;
    }
  }
  
  inline void
  LiveInterval::build_merged_range_bitset()
  {
    if (0 == m_merged_range_bitset_without_def.size())
    {
      assert(m_merged_range.size() != 0);
      assert(0 == m_merged_range_bitset_without_lastuse.size());
      
      uint32_t const bitset_size = ((m_merged_range.back().m_end + 1 + 31) >> 5);
      
      m_merged_range_bitset_without_def.resize(bitset_size);
      m_merged_range_bitset_without_lastuse.resize(bitset_size);
      
      std::vector<LiveRange>::const_iterator iter = m_merged_range.begin();
      
      // :NOTE: Wei 2005-Feb-24:
      //
      // The reason why I exclude the definition point is because if the DEF point
      // overlaps the USE point, then that is not a real overlap.
      //
      // See overlap(LiveInterval *).
      set_used_bit(m_merged_range_bitset_without_def,
                   (*iter).m_start + 1,
                   (*iter).m_end);
      
      if ((*iter).m_start == (*iter).m_end)
      {
        // :NOTE: Wei 2005-Feb-25:
        //
        // Becuase this is a DEF point, and may be overlap with another DEF point,
        // thus I have to reserve this DEF point,
        // not to exclude it from 'm_merged_range_bitset_without_lastuse'.
        set_used_bit(m_merged_range_bitset_without_lastuse,
                     (*iter).m_start,
                     (*iter).m_end);
      }
      else
      {
        set_used_bit(m_merged_range_bitset_without_lastuse,
                     (*iter).m_start,
                     (*iter).m_end - 1);
      }
      
      ++iter;
      
      for (;
           iter != m_merged_range.end();
           ++iter)
      {
        set_used_bit(m_merged_range_bitset_without_def,
                     (*iter).m_start,
                     (*iter).m_end);
        
        set_used_bit(m_merged_range_bitset_without_lastuse,
                     (*iter).m_start,
                     (*iter).m_end - 1);
      }
    }
  }
  
  bool
  LiveInterval::overlap(uint32_t const begin)
  {
    build_merged_range_bitset();
    
    uint32_t vector_idx;
    uint32_t bit_position;
    
    calculate_bit_position(begin, vector_idx, bit_position);
    
    assert(m_merged_range_bitset_without_lastuse.size() != 0);
    
    if ((m_merged_range_bitset_without_lastuse.size() > vector_idx) &&
        ((m_merged_range_bitset_without_lastuse[vector_idx] & bit_position) != 0))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  
  bool
  LiveInterval::overlap(LiveInterval * const target)
  {
    // :NOTE: Wei 2005-Feb-25:
    //
    // Check to see if 2 start points overlapping.
    build_merged_range_bitset();
    target->build_merged_range_bitset();
    
    uint32_t vector_idx;
    uint32_t bit_position;
    
    calculate_bit_position(start_point_idx(), vector_idx, bit_position);
    
    assert(target->merged_range_bitset_without_lastuse().size() != 0);
    
    if ((target->merged_range_bitset_without_lastuse().size() > vector_idx) &&
        (((target->merged_range_bitset_without_lastuse())[vector_idx] & bit_position) != 0))
    {
      return true;
    }
    
    calculate_bit_position(target->start_point_idx(), vector_idx, bit_position);
    
    assert(m_merged_range_bitset_without_lastuse.size() != 0);
    
    if ((m_merged_range_bitset_without_lastuse.size() > vector_idx) &&
        ((m_merged_range_bitset_without_lastuse[vector_idx] & bit_position) != 0))
    {
      return true;
    }
    
    std::vector<uint32_t>::const_iterator iter = m_merged_range_bitset_without_def.begin();
    std::vector<uint32_t>::const_iterator target_iter = target->merged_range_bitset_without_def().begin();
    
    do
    {
      if (((*iter) & (*target_iter)) != 0)
      {
        return true;
      }
      
      ++iter;
      ++target_iter;
    }
    while ((iter != m_merged_range_bitset_without_def.end()) &&
           (target_iter != target->merged_range_bitset_without_def().end()));
    
    return false;
  }
  
#if PRINT_CHUNK_CODE
  void
  LiveInterval::dump_info() const
  {
    mp_attached_var->dump_info<true>();
    
    g_log_file << " "
               << start_point_idx()
               << "-"
               << end_point_idx();
    
    if (mp_preallocated_reg != 0)
    {
      g_log_file << " (";
      mp_preallocated_reg->dump_info();
      g_log_file << ")";
    }
  }
#endif
}

#endif
