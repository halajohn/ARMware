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

namespace ARMware
{
  inline void
  Mmu::init_args(uint32_t const vaddr, CoreMode const currentMode)
  {
    mCurrVaddr = vaddr;
    mCurrCoreMode = currentMode;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<MmuBlockTypeEnum mmu_block_type_enum>
  struct
  Mmu::RetrieveAccessPermissionBit<mmu_block_type_enum,
                                   BlockHasOneApField::BlockApFieldNumCategoryId>
  {
    static inline uint32_t
    Operation(Mmu const * const /* mmu */, uint32_t const descriptor)
    {
      return ((descriptor & MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_BITMASK) >>
              MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_SMALLEST_BIT_LOCATION);
    }
  };
  
  template<MmuBlockTypeEnum mmu_block_type_enum>
  struct
  Mmu::RetrieveAccessPermissionBit<mmu_block_type_enum,
                                   BlockHasFourApField::BlockApFieldNumCategoryId>
  {
    static inline uint32_t
    Operation(Mmu const * const mmu, uint32_t const descriptor)
    {
      // Determine the subpage we are currently in.
      switch (mmu->mCurrVaddr & MmuBlockTypeTraits<mmu_block_type_enum>::SUBPAGE_BITMASK)
      {
      case MmuBlockTypeTraits<mmu_block_type_enum>::SUBPAGE_1_BITMASK:
        return ((descriptor & MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_1_BITMASK) >>
                MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_1_SMALLEST_BIT_LOCATION);
        
      case MmuBlockTypeTraits<mmu_block_type_enum>::SUBPAGE_2_BITMASK:
        return ((descriptor & MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_2_BITMASK) >>
                MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_2_SMALLEST_BIT_LOCATION);
        
      case MmuBlockTypeTraits<mmu_block_type_enum>::SUBPAGE_3_BITMASK:
        return ((descriptor & MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_3_BITMASK) >>
                MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_3_SMALLEST_BIT_LOCATION);
        
      case MmuBlockTypeTraits<mmu_block_type_enum>::SUBPAGE_4_BITMASK:
        return ((descriptor & MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_4_BITMASK) >>
                MmuBlockTypeTraits<mmu_block_type_enum>::AP_FIELD_4_SMALLEST_BIT_LOCATION);
        
      default:
        assert(!"Should not reach here.");
        return 0;
      }
    }
  };
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<MmuBlockTypeEnum mmu_block_type_enum>
  inline void
  Mmu::raise_translation_fault_by_block_type(uint32_t const domain_num, bool &exception_occur) const
  {
#if TRACE_MMU
    g_log_file << "MMU: TranslationFault: <Page> " << std::hex << mCurrVaddr << std::endl;
#endif
    
    mp_core->update_FSR_FAR(FS_TRANSLATION_PAGE, domain_num, mCurrVaddr);
    
    assert(false == exception_occur);
    
    exception_occur = true;
  }
  
  template<>
  inline void
  Mmu::raise_translation_fault_by_block_type<SECTION>(uint32_t const domain_num, bool &exception_occur) const
  {
#if TRACE_MMU
    g_log_file << "MMU: TranslationFault: <Section> " << std::hex << mCurrVaddr << std::endl;
#endif
    
    mp_core->update_FSR_FAR(FS_TRANSLATION_SECTION, domain_num, mCurrVaddr);
    
    assert(false == exception_occur);
    
    exception_occur = true;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<MmuBlockTypeEnum mmu_block_type_enum>
  inline void
  Mmu::raise_domain_fault_by_block_type(uint32_t const domain_num, bool &exception_occur) const
  {
#if TRACE_MMU
    g_log_file << "MMU: DomainFault: Page."<< std::endl;
#endif
    
    mp_core->update_FSR_FAR(FS_DOMAIN_PAGE, domain_num, mCurrVaddr);
    
    assert(false == exception_occur);
    
    exception_occur = true;
  }
  
  template<>
  inline void
  Mmu::raise_domain_fault_by_block_type<SECTION>(uint32_t const domain_num, bool &exception_occur) const
  {
#if TRACE_MMU
    g_log_file << "MMU: DomainFault: Section."<< std::endl;
#endif
    
    mp_core->update_FSR_FAR(FS_DOMAIN_SECTION, domain_num, mCurrVaddr);
    
    assert(false == exception_occur);
    
    exception_occur = true;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<MmuBlockTypeEnum mmu_block_type_enum>
  inline void
  Mmu::raise_permission_fault_by_block_type(uint32_t const domain_num, bool &exception_occur) const
  {
#if TRACE_MMU
    g_log_file << "MMU: PermissionFault: <Page> " << std::hex << mCurrVaddr << std::endl;
#endif
    
    mp_core->update_FSR_FAR(FS_PERMISSION_PAGE, domain_num, mCurrVaddr);
    
    assert(false == exception_occur);
    
    exception_occur = true;
  }
  
  template<>
  inline void
  Mmu::raise_permission_fault_by_block_type<SECTION>(uint32_t const domain_num, bool &exception_occur) const
  {
#if TRACE_MMU
    g_log_file << "MMU: PermissionFault: <Section> " << std::hex << mCurrVaddr << std::endl;
#endif
    
    mp_core->update_FSR_FAR(FS_PERMISSION_SECTION, domain_num, mCurrVaddr);
    
    assert(false == exception_occur);
    
    exception_occur = true;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<MmuBlockTypeEnum mmu_block_type_enum, RWType rw_type>
  void
  Mmu::check_access_permission_by_block_type(uint32_t const ap_bits,
                                             uint32_t const domain_num,
                                             bool &exception_occur) const
  {
    switch (ap_bits)
    {
    case 0x0:
      // AP = 0b00 ==> watch S & R bits
      // S & R bits locate in the bit 8 & bit 9 in register 1 of the system co-processor.
      switch (mpSysCoProc->get_ctrl_reg() & 0x300)
      {
      case 0x000:
        // S == 0, R == 0
        raise_permission_fault_by_block_type<mmu_block_type_enum>(domain_num, exception_occur);
        return;
        
      case 0x100:
        // S == 1, R == 0
        switch (mCurrCoreMode)
        {
        case MODE_USR:
          raise_permission_fault_by_block_type<mmu_block_type_enum>(domain_num, exception_occur);
          return;
          
        case MODE_FIQ:
        case MODE_IRQ:
        case MODE_SVC:
        case MODE_ABT:
        case MODE_UND:
        case MODE_SYS:
          if (WRITE == rw_type)
          {
            raise_permission_fault_by_block_type<mmu_block_type_enum>(domain_num, exception_occur);
            return;
          }
          break;
        }
        break;
        
      case 0x200:
        // S == 0, R == 1
        if (WRITE == rw_type)
        {
          raise_permission_fault_by_block_type<mmu_block_type_enum>(domain_num, exception_occur);
          return;
        }
        break;
        
      case 0x300:
        // S == 1, R == 1
        
        // :ARM-DDI-0100E: p.B3-16: Wei 2003-Oct-29:
        //
        // this condition will lead to a unpredictable situation.
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x1:
      // AP = 0b01
      switch (mCurrCoreMode)
      {
      case MODE_USR:
        raise_permission_fault_by_block_type<mmu_block_type_enum>(domain_num, exception_occur);
        return;
        
      case MODE_FIQ:
      case MODE_IRQ:
      case MODE_SVC:
      case MODE_ABT:
      case MODE_UND:
      case MODE_SYS:
        break;
      }
      break;
      
    case 0x2:
      // AP = 0b10
      switch (mCurrCoreMode)
      {
      case MODE_USR:
        if (WRITE == rw_type)
        {
          raise_permission_fault_by_block_type<mmu_block_type_enum>(domain_num, exception_occur);
          return;
        }
        break;
        
      case MODE_FIQ:
      case MODE_IRQ:
      case MODE_SVC:
      case MODE_ABT:
      case MODE_UND:
      case MODE_SYS:
        break;
      }
      break;
      
    case 0x3:
      // AP = 0b11
      break;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<MmuBlockTypeEnum mmu_block_type_enum, RWType rw_type>
  inline void
  Mmu::check_access_right(uint32_t const ap_bits, uint32_t const domain_num, bool &exception_occur) const
  {
    switch (check_domain(domain_num))
    {
    case NO_ACCESS:
      raise_domain_fault_by_block_type<mmu_block_type_enum>(domain_num, exception_occur);
      break;
      
    case CLIENT:
      check_access_permission_by_block_type<mmu_block_type_enum, rw_type>(ap_bits,
                                                                          domain_num,
                                                                          exception_occur);
      break;
      
    case RESERVED:
      assert(!"Should not reach here.");
      break;
      
    case MANAGER:
      break;
    }
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  inline uint32_t
  Mmu::get_phy_addr_from_tlb_entry(TlbEntry const * const entry) const
  {
    switch (entry->block_type())
    {
    case SECTION:
      return entry->phy_base() | (mCurrVaddr & MmuBlockTypeTraits<SECTION>::BLOCK_INDEX_BITMASK);
      
    case LARGE_PAGE:
      return entry->phy_base() | (mCurrVaddr & MmuBlockTypeTraits<LARGE_PAGE>::BLOCK_INDEX_BITMASK);
      
    case SMALL_PAGE:
      return entry->phy_base() | (mCurrVaddr & MmuBlockTypeTraits<SMALL_PAGE>::BLOCK_INDEX_BITMASK);
      
    case TINY_PAGE:
      return entry->phy_base() | (mCurrVaddr & MmuBlockTypeTraits<TINY_PAGE>::BLOCK_INDEX_BITMASK);
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
#if CHECK_UNPREDICTABLE
  template<>
  inline void
  Mmu::check_descriptor_bits<SECTION>(uint32_t const level_one_descriptor, uint32_t const /* level_two_descriptor */) const
  {
    assert((0 == (level_one_descriptor & (1 << 9))) && (0 == (level_one_descriptor & 0xFF000)));
  }
  
  template<>
  inline void
  Mmu::check_descriptor_bits<LARGE_PAGE>(uint32_t const /* level_one_descriptor */,
                                         uint32_t const level_two_descriptor) const
  {
    assert(0 == (level_two_descriptor & 0xF000));
  }
  
  template<>
  inline void
  Mmu::check_descriptor_bits<SMALL_PAGE>(uint32_t const /* level_one_descriptor */,
                                         uint32_t const /* level_two_descriptor */) const
  { }
  
  template<>
  inline void
  Mmu::check_descriptor_bits<TINY_PAGE>(uint32_t const /* level_one_descriptor */,
                                        uint32_t const level_two_descriptor) const
  {
    assert(0 == (level_two_descriptor & 0x3C0));
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<AccessTypeEnum access_type_enum, MmuBlockTypeEnum mmu_block_type_enum, RWType rw_type>
  inline uint32_t
  Mmu::translate_internal(uint32_t const level_one_descriptor,
                          uint32_t const level_two_descriptor,
                          bool &exception_occur)
  {
#if CHECK_UNPREDICTABLE
    check_descriptor_bits<mmu_block_type_enum>(level_one_descriptor, level_two_descriptor);
#endif
    
    uint32_t const domain_num = get_domain_num(level_one_descriptor);
    uint32_t const ap_bits = RetrieveAccessPermissionBit<mmu_block_type_enum>::Operation(this, level_two_descriptor);
    
    check_access_right<mmu_block_type_enum, rw_type>(ap_bits, domain_num, exception_occur);
    
    if (true == exception_occur)
    {
      return 0;
    }
    
    uint32_t const phy_addr = ((level_two_descriptor & MmuBlockTypeTraits<mmu_block_type_enum>::BLOCK_BASE_BITMASK) |
                               (mCurrVaddr & MmuBlockTypeTraits<mmu_block_type_enum>::BLOCK_INDEX_BITMASK));
    
    TlbEntry * const entry = get_tlb<access_type_enum>().find_empty_entry(mCurrVaddr);
    
    entry->set_valid(true);
    entry->set_block_type(mmu_block_type_enum);
    entry->set_domain_num(domain_num);
    entry->set_access_perm(ap_bits);
    entry->set_table_idx(mCurrVaddr & MmuBlockTypeTraits<mmu_block_type_enum>::TABLE_INDEX_BITMASK);
    entry->set_phy_base(level_two_descriptor & MmuBlockTypeTraits<mmu_block_type_enum>::BLOCK_BASE_BITMASK);
    
    return phy_addr;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<AccessTypeEnum access_type_enum, RWType rw_type>
  uint32_t
  Mmu::translate(bool &exception_occur)
  {
    uint32_t const level_one_addr = ((mpSysCoProc->get_TTB_reg() & 0xFFFFC000) | ((mCurrVaddr & 0xFFF00000) >> 18));
    uint32_t const level_one_descriptor = mpAddressSpace->template get_data<uint32_t>(level_one_addr, exception_occur);
    
    // :NOTE: Wei 2004-Jul-30:
    //
    // AddressSpace::get_data() might throw an exception of accessing reserved memory area.
    if (true == exception_occur)
    {
      return 0;
    }
    
    switch (level_one_descriptor & 0x3)
    {
    case 0x0:
      // Unmapped virtual address
      raise_translation_fault_by_block_type<SECTION>(0, exception_occur);
      return 0;
      
    case 0x1:
      // coarse page table.
      {
        uint32_t const level_two_addr = ((level_one_descriptor & 0xFFFFFC00) | ((mCurrVaddr & 0xFF000) >> 10));
        uint32_t const level_two_descriptor = mpAddressSpace->template get_data<uint32_t>(level_two_addr, exception_occur);
        
        // :NOTE: Wei 2004-Jul-30:
        //
        // AddressSpace::get_data() might throw an exception of accessing reserved memory area.
        if (true == exception_occur)
        {
          return 0;
        }
        
        switch (level_two_descriptor & 0x3)
        {
        case 0x0:
          // Unmapped virtual address
          //
          // :NOTE: Wei 2004-May-15:
          //
          // Although we don't know whether this is a large page, small page or tiny page,
          // however, this is indeed a page translation fault,
          // thus, I use LARGE_PAGE to indicate this condition.
          raise_translation_fault_by_block_type<LARGE_PAGE>(get_domain_num(level_one_descriptor), exception_occur);
          return 0;
          
        case 0x1:
          // A large page descriptor, describing 64 kb of virtual address.
          return translate_internal<access_type_enum, LARGE_PAGE, rw_type>(level_one_descriptor,
                                                                           level_two_descriptor,
                                                                           exception_occur);
          
        case 0x2:
          // a small page descriptor, describing 4 kb of virtual address.
          return translate_internal<access_type_enum, SMALL_PAGE, rw_type>(level_one_descriptor,
                                                                           level_two_descriptor,
                                                                           exception_occur);
          
        case 0x3:
          // :NOTE: Wei 2003-Oct-29:
          //
          // a tiny page descriptor, describing 1 kb virtual address
          // but a tiny page descriptor can _not_ appears in a coarse 2nd-level table,
          // thus ARMware will exit.
          assert(!"Should not reach here.");
          break;
        }
      }
      break;
      
    case 0x2:
      // section descriptor
      
      // :NOTE: Wei 2004-May-15:
      //
      // In the case of section, the role of level_two_descriptor in translate_internal() is
      // the same with level_one_descriptor, thus I use level_one_descriptor as the second
      // argument, too.
      return translate_internal<access_type_enum, SECTION, rw_type>(level_one_descriptor,
                                                                    level_one_descriptor,
      //                                                                  ^^^
                                                                    exception_occur);
      
    case 0x3:
      // fine page table
      {
        uint32_t const level_two_addr = ((level_one_descriptor & 0xFFFFF000) | ((mCurrVaddr & 0xFFC00) >> 8));
        uint32_t const level_two_descriptor = mpAddressSpace->template get_data<uint32_t>(level_two_addr, exception_occur);
        
        // :NOTE: Wei 2004-Jul-30:
        //
        // AddressSpace::get_data() might throw an exception of accessing reserved memory area.
        if (true == exception_occur)
        {
          return 0;
        }
        
        switch (level_two_descriptor & 0x3)
        {
        case 0x0:
          // unmapped
          raise_translation_fault_by_block_type<LARGE_PAGE>(get_domain_num(level_one_descriptor), exception_occur);
          return 0;
          
        case 0x1:
          // A large page descriptor, describing 64 kb of virtual address.
          return translate_internal<access_type_enum, LARGE_PAGE, rw_type>(level_one_descriptor,
                                                                           level_two_descriptor,
                                                                           exception_occur);
          
        case 0x2:
          // a small page descriptor, describing 4 kb of virtual address.
          return translate_internal<access_type_enum, SMALL_PAGE, rw_type>(level_one_descriptor,
                                                                           level_two_descriptor,
                                                                           exception_occur);
          
        case 0x3:
          // a tiny page descriptor, describing 1 kb virtual address
          return translate_internal<access_type_enum, TINY_PAGE, rw_type>(level_one_descriptor,
                                                                          level_two_descriptor,
                                                                          exception_occur);
        }
      }
      break;
    }
    
    // shouldn't reach here.
    assert(!"Should not reach here.");
    
    return 0; // make compiler happy.
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<AccessTypeEnum access_type_enum, RWType rw_type>
  uint32_t
  Mmu::find_phy_addr(bool &exception_occur)
  {
    TlbEntry * const entry = get_tlb<access_type_enum>().find_entry(mCurrVaddr);
    
    if (0 == entry)
    {
      return translate<access_type_enum, rw_type>(exception_occur);
    }
    else
    {
      switch (entry->block_type())
      {
      case SECTION:
        check_access_right<SECTION, rw_type>(entry->access_perm(), entry->domain_num(), exception_occur);
        break;
        
      case LARGE_PAGE:
        check_access_right<LARGE_PAGE, rw_type>(entry->access_perm(), entry->domain_num(), exception_occur);
        break;
        
      case SMALL_PAGE:
        check_access_right<SMALL_PAGE, rw_type>(entry->access_perm(), entry->domain_num(), exception_occur);
        break;
        
      case TINY_PAGE:
#if ASSUME_NOT_USE_TINY_PAGE
        assert(!"Should not reach here.");
#endif
        check_access_right<TINY_PAGE, rw_type>(entry->access_perm(), entry->domain_num(), exception_occur);
        break;
      }
      
      if (true == exception_occur)
      {
        return 0;
      }
    }
    
    assert(entry != 0);
    
    return get_phy_addr_from_tlb_entry(entry);
  }
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<AccessTypeEnum access_type_enum, RWType rw_type>
  uint32_t
  Mmu::translate_access(uint32_t const vaddr, CoreMode const currentMode, bool &exception_occur)
  {
    init_args(vaddr, currentMode);
    
    return find_phy_addr<access_type_enum, rw_type>(exception_occur);
  }
}
