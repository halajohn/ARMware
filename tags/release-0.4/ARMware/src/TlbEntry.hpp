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

// Translation lookahead buffer entry
//

#ifndef TlbEntry_hpp
#define TlbEntry_hpp

// Local include
//

#include "ARMware.hpp"
#include "MmuBlockType.hpp"

namespace ARMware
{
  typedef class TlbEntry TlbEntry;
  class TlbEntry
  {
  private:
    
    // Attribute
    
    bool m_valid;
    uint8_t m_domain_num;
    uint8_t m_access_perm;
    uint32_t m_table_idx;
    uint32_t m_phy_base;
    MmuBlockTypeEnum m_mmu_block_type_enum;
    
  public:
    
    // Operation
    
    inline void
    set_valid(bool const valid)
    {
      m_valid = valid;
    }
    
    inline void
    set_block_type(MmuBlockTypeEnum const mmu_block_type_enum)
    {
      m_mmu_block_type_enum = mmu_block_type_enum;
    }
    
    inline void
    set_domain_num(uint8_t const domain_num)
    {
      m_domain_num = domain_num;
    }
    
    inline void
    set_access_perm(uint8_t const access_perm)
    {
      m_access_perm = access_perm;
    }
    
    inline void
    set_table_idx(uint32_t const address)
    {
      m_table_idx = address;
    }
    
    inline void
    set_phy_base(uint32_t const address)
    {
      m_phy_base = address;
    }
    
    // Inquery
    
    inline bool
    is_valid() const
    {
      return m_valid;
    }
    
    inline MmuBlockTypeEnum
    block_type() const
    {
      return m_mmu_block_type_enum;
    }
    
    inline uint8_t
    domain_num() const
    {
      return m_domain_num;
    }
    
    inline uint8_t
    access_perm() const
    {
      return m_access_perm;
    }
    
    inline uint32_t
    table_idx() const
    {
      return m_table_idx;
    }
    
    inline uint32_t
    phy_base() const
    {
      return m_phy_base;
    }
  };
}

#endif
