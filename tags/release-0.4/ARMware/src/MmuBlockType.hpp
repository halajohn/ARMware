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

#ifndef MmuBlockType_hpp
#define MmuBlockType_hpp

namespace ARMware
{
  template<typename T>
  struct BlockApFieldNumCategory
  {
    struct BlockApFieldNumCategoryId {};
  };
  
  struct BlockHasOneApField
    : private BlockApFieldNumCategory<BlockHasOneApField>
  {
    using BlockApFieldNumCategory<BlockHasOneApField>::BlockApFieldNumCategoryId;
  };
  
  struct BlockHasFourApField
    : private BlockApFieldNumCategory<BlockHasFourApField>
  {
    using BlockApFieldNumCategory<BlockHasFourApField>::BlockApFieldNumCategoryId;
  };
  
  enum MmuBlockTypeEnum
  {
    SECTION,
    LARGE_PAGE,
    SMALL_PAGE,
    TINY_PAGE
  };
  typedef enum MmuBlockTypeEnum MmuBlockTypeEnum;
  
  template<MmuBlockTypeEnum block_type_enum>
  struct MmuBlockTypeTraits;
  
  template<>
  struct MmuBlockTypeTraits<SECTION> : public BlockHasOneApField
  {
    static uint32_t const AP_FIELD_BITMASK = 0xC00;
    static uint32_t const AP_FIELD_SMALLEST_BIT_LOCATION = 10;
    
    static uint32_t const TABLE_INDEX_BITMASK = 0xFFF00000;
    static uint32_t const TABLE_INDEX_FIELD_SMALLEST_BIT_LOCATION = 20;
    
    static uint32_t const BLOCK_BASE_BITMASK = 0xFFF00000;
    static uint32_t const BLOCK_INDEX_BITMASK = ~BLOCK_BASE_BITMASK;
  };
  
  template<>
  struct MmuBlockTypeTraits<LARGE_PAGE> : public BlockHasFourApField
  {
    static uint32_t const SUBPAGE_BITMASK = 0xC000;
    
    static uint32_t const SUBPAGE_1_BITMASK = 0x0000; // 0000 ~ 0011
    static uint32_t const SUBPAGE_2_BITMASK = 0x4000; // 0100 ~ 0111
    static uint32_t const SUBPAGE_3_BITMASK = 0x8000; // 1000 ~ 1011
    static uint32_t const SUBPAGE_4_BITMASK = 0xC000; // 1100 ~ 1111
    
    static uint32_t const AP_FIELD_1_BITMASK = 0x30;
    static uint32_t const AP_FIELD_1_SMALLEST_BIT_LOCATION = 4;
    
    static uint32_t const AP_FIELD_2_BITMASK = 0xC0;
    static uint32_t const AP_FIELD_2_SMALLEST_BIT_LOCATION = 6;
    
    static uint32_t const AP_FIELD_3_BITMASK = 0x300;
    static uint32_t const AP_FIELD_3_SMALLEST_BIT_LOCATION = 8;
    
    static uint32_t const AP_FIELD_4_BITMASK = 0xC00;
    static uint32_t const AP_FIELD_4_SMALLEST_BIT_LOCATION = 10;
    
    static uint32_t const TABLE_INDEX_BITMASK = 0xFFFFF000;
    
    static uint32_t const BLOCK_BASE_BITMASK = 0xFFFF0000;
    static uint32_t const BLOCK_INDEX_BITMASK = ~BLOCK_BASE_BITMASK;
  };
  
  template<>
  struct MmuBlockTypeTraits<SMALL_PAGE> : public BlockHasFourApField
  {
    static uint32_t const SUBPAGE_BITMASK = 0xC00;
    
    static uint32_t const SUBPAGE_1_BITMASK = 0x000; // 0000 ~ 0011
    static uint32_t const SUBPAGE_2_BITMASK = 0x400; // 0100 ~ 0111
    static uint32_t const SUBPAGE_3_BITMASK = 0x800; // 1000 ~ 1011
    static uint32_t const SUBPAGE_4_BITMASK = 0xC00; // 1100 ~ 1111
    
    static uint32_t const AP_FIELD_1_BITMASK = 0x30;
    static uint32_t const AP_FIELD_1_SMALLEST_BIT_LOCATION = 4;
    
    static uint32_t const AP_FIELD_2_BITMASK = 0xC0;
    static uint32_t const AP_FIELD_2_SMALLEST_BIT_LOCATION = 6;
    
    static uint32_t const AP_FIELD_3_BITMASK = 0x300;
    static uint32_t const AP_FIELD_3_SMALLEST_BIT_LOCATION = 8;
    
    static uint32_t const AP_FIELD_4_BITMASK = 0xC00;
    static uint32_t const AP_FIELD_4_SMALLEST_BIT_LOCATION = 10;
    static uint32_t const TABLE_INDEX_BITMASK = 0xFFFFF000;
    
    static uint32_t const BLOCK_BASE_BITMASK = 0xFFFFF000;
    static uint32_t const BLOCK_INDEX_BITMASK = ~BLOCK_BASE_BITMASK;
  };
  
  template<>
  struct MmuBlockTypeTraits<TINY_PAGE> : public BlockHasOneApField
  {
    static uint32_t const AP_FIELD_BITMASK = 0x30;
    static uint32_t const AP_FIELD_SMALLEST_BIT_LOCATION = 4;
    
    static uint32_t const TABLE_INDEX_BITMASK = 0xFFFFFC00;
    
    static uint32_t const BLOCK_BASE_BITMASK = 0xFFFFFC00;
    static uint32_t const BLOCK_INDEX_BITMASK = ~BLOCK_BASE_BITMASK;
  };
}

#endif
