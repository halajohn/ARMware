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

// Memory Management Unit
//

#ifndef Mmu_hpp
#define Mmu_hpp

// Local include
//

#include "Tlb.hpp"
#include "MmuBlockType.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class AddressSpace;
  class SysCoProc;
  class Core;
  
  typedef class Mmu Mmu;
  class Mmu
  {
  private:
    
    enum Domain
    {
      NO_ACCESS = 0x0,
      CLIENT    = 0x1,
      RESERVED  = 0x2,
      MANAGER   = 0x3
    };
    typedef enum Domain Domain;
    
    // Operation
    
    // :NOTE: Wei 2004-Jan-19:
    //
    // This function only be used in class Mmu, and this function will use other
    // functions defined in class SysCoProc.
    //
    // Thus, to avoid include SysCoProc.hpp in this include file,
    // I put the definition of this function in Mmu.tcc.
    void init_args(uint32_t const vaddr, CoreMode const currentMode);
    
    template<MmuBlockTypeEnum mmu_block_type_enum,
             typename BlockApFieldNumCategoryId = typename MmuBlockTypeTraits<mmu_block_type_enum>::BlockApFieldNumCategoryId>
    struct RetrieveAccessPermissionBit;
    
    template<MmuBlockTypeEnum mmu_block_type_enum, RWType rw_type>
    void check_access_permission_by_block_type(uint32_t const ap_bits,
                                               uint32_t const domain_num,
                                               bool &exception_occur) const;
    
    template<MmuBlockTypeEnum mmu_block_type_enum, RWType rw_type>
    void check_access_right(uint32_t const ap_bits, uint32_t const domain_num, bool &exception_occur) const;
    
    template<MmuBlockTypeEnum mmu_block_type_enum>
    void raise_translation_fault_by_block_type(uint32_t const domain_num, bool &exception_occur) const;
    
    template<MmuBlockTypeEnum mmu_block_type_enum>
    void raise_permission_fault_by_block_type(uint32_t const domain_num, bool &exception_occur) const;
    
    template<MmuBlockTypeEnum mmu_block_type_enum>
    void raise_domain_fault_by_block_type(uint32_t const domain_num, bool &exception_occur) const;
    
    uint32_t get_phy_addr_from_tlb_entry(TlbEntry const * const entry) const;
    
    inline uint32_t
    get_domain_num(uint32_t const level_one_descriptor) const
    { return (level_one_descriptor & 0x1E0) >> 5; }
    
    Domain check_domain(uint32_t const index) const;
    
    template<AccessTypeEnum access_type_enum, RWType rw_type>
    uint32_t find_phy_addr(bool &exception_occur);
    
#if CHECK_UNPREDICTABLE
    template<MmuBlockTypeEnum mmu_block_type_enum>
    void check_descriptor_bits(uint32_t const level_one_descriptor, uint32_t const level_two_descriptor) const;
#endif
    
    template<AccessTypeEnum access_type_enum, RWType rw_type>
    uint32_t translate(bool &exception_occur);
    
    template<AccessTypeEnum access_type_enum, MmuBlockTypeEnum mmu_block_type_enum, RWType rw_type>
    uint32_t translate_internal(uint32_t const level_one_descriptor,
                                uint32_t const level_2_descriptor,
                                bool &exception_occur);
    
    template<AccessTypeEnum access_type_enum>
    Tlb &get_tlb() FASTCALL;
    
    // Attribute
    
    uint32_t mCurrVaddr;
    CoreMode mCurrCoreMode;
    
    Core         * const mp_core;
    SysCoProc    * const mpSysCoProc;
    AddressSpace * const mpAddressSpace;
    
    Tlb m_inst_TLB;
    Tlb m_data_TLB;
    
  public:
    
    // Life cycle
    
    Mmu(Core * const core, SysCoProc * const sysCoProc, AddressSpace * const addressSpace);
    
    // Operation
    
    inline void
    flush_inst_tlb()
    { m_inst_TLB.flush_all(); }
    
    inline void
    flush_data_tlb()
    { m_data_TLB.flush_all(); }
    
    inline void
    flush_data_tlb_single_entry(uint32_t const address)
    { m_data_TLB.flush_single_entry(address); }
    
    template<AccessTypeEnum access_type_enum, RWType rw_type>
    uint32_t translate_access(uint32_t const vaddr,
                              CoreMode const currentMode,
                              bool &exception_occur);
  };
  
  template<>
  inline Tlb &
  Mmu::get_tlb<INST>()
  { return m_inst_TLB; }
  
  template<>
  inline Tlb &
  Mmu::get_tlb<DATA>()
  { return m_data_TLB; }
}

#endif
