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

#ifndef LiveInterval_hpp
#define LiveInterval_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>
#include <vector>

#include "LiveRange.hpp"
#include "x86/Register.hpp"

#include "../MemoryPool2.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Variable;
  
  typedef class LiveInterval LiveInterval;
  class LiveInterval : public MemoryPool2<LiveInterval, 256>
  {
  private:
    
    std::vector<LiveRange> m_range;
    std::vector<LiveRange> m_merged_range;
    
    std::vector<uint32_t> m_merged_range_bitset_without_def;
    std::vector<uint32_t> m_merged_range_bitset_without_lastuse;
    
    Register *mp_preallocated_reg;
    std::vector<Register::KindEnum> m_excluded_reg;
    
    Variable *mp_attached_var;
    
    LiveInterval *mp_prev_unhandled;
    LiveInterval *mp_next_unhandled;
    
    void set_used_bit(std::vector<uint32_t> &bitset,
                      uint32_t const start,
                      uint32_t const end);
    
    friend class MemoryPool2<LiveInterval, 256>;
    
  public:
    
    // Life cycle
    
    LiveInterval(uint32_t const start,
                 uint32_t const end,
                 Variable * const var);
    
    ~LiveInterval();
    
    // Access
    
    inline void
    add_excluded_reg(Register::KindEnum const kind)
    { m_excluded_reg.push_back(kind); }
    
    inline void
    set_prev_unhandled(LiveInterval * const live)
    { mp_prev_unhandled = live; }
    
    inline void
    set_next_unhandled(LiveInterval * const live)
    { mp_next_unhandled = live; }
    
    template<bool T_delete>
    inline void
    clear_preallocated_reg()
    {
      if (true == T_delete)
      {
        if (mp_preallocated_reg != 0)
        {
          delete mp_preallocated_reg;
        }
      }
      
      mp_preallocated_reg = 0;
    }
    
    inline void
    set_preallocated_reg(Register * const reg)
    {
      assert(reg != 0);
      
      mp_preallocated_reg = reg;
    }
    
    inline void
    set_attached_var(Variable * const var)
    { mp_attached_var = var; }
    
    // Inquiry
    
    inline std::vector<Register::KindEnum> const &
    excluded_reg() const
    { return m_excluded_reg; }
    
    inline uint32_t
    start_point_idx() const
    {
      assert(m_merged_range.size() > 0);
      
      return m_merged_range.front().m_start;
    }
    
    inline uint32_t
    end_point_idx() const
    {
      assert(m_merged_range.size() > 0);
      
      return m_merged_range.back().m_end;
    }
    
    inline std::vector<uint32_t> const &
    merged_range_bitset_without_def() const
    {
      assert(m_merged_range_bitset_without_def.size() != 0);
      
      return m_merged_range_bitset_without_def;
    }
    
    inline std::vector<uint32_t> const &
    merged_range_bitset_without_lastuse() const
    {
      assert(m_merged_range_bitset_without_lastuse.size() != 0);
      
      return m_merged_range_bitset_without_lastuse;
    }
    
    inline LiveInterval *
    prev_unhandled() const
    { return mp_prev_unhandled; }
    
    inline LiveInterval *
    next_unhandled() const
    { return mp_next_unhandled; }
    
    inline Register *
    preallocated_reg() const
    { return mp_preallocated_reg; }
    
    inline std::vector<LiveRange> const &
    merged_range() const
    { return m_merged_range; }
    
    inline Variable *
    attached_var() const
    { return mp_attached_var; }
    
    // Operation
    
    inline bool
    has_live_range()
    { return (0 == m_range.size()) ? false : true; }
    
    inline void
    move_merged_range_to_raw_range()
    {
      m_range.clear();
      
      m_range = m_merged_range;
      
      m_merged_range.clear();
    }
    
    inline void
    add_live_range(uint32_t const start, uint32_t const end)
    { m_range.push_back(LiveRange(start, end)); }
    
    template<bool T_merge_non_overlap_range>
    void merge_ranges();
    
    bool overlap(uint32_t const begin);
    
    void build_merged_range_bitset();
    bool overlap(LiveInterval * const target);
    
    inline void
    clear_bitset()
    {
      m_merged_range_bitset_without_def.clear();
      m_merged_range_bitset_without_lastuse.clear();
    }
    
#if PRINT_CHUNK_CODE
    void dump_info() const;
#endif
  };
}

#endif

#endif
