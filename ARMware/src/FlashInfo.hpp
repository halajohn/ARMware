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

#ifndef FlashInfo_hpp
#define FlashInfo_hpp

namespace ARMware
{
  enum FlashTypeEnum
  {
    INTEL_STRATA_J3
  };
  typedef enum FlashTypeEnum FlashTypeEnum;
  
  template<FlashTypeEnum flash_type_enum>
  struct FlashInfo;
  
  template<>
  struct FlashInfo<INTEL_STRATA_J3>
  {
    enum Mode
    {
      READ_ARRAY_MODE,
      READ_ID_CODES_MODE,
      READ_QUERY_MODE,
      READ_STATUS_REG_MODE,
      
      BYTE_WORD_PROGRAM_MODE,
      
      WRITE_TO_BUFFER_MODE_BEGIN,
      WRITE_TO_BUFFER_MODE_DATA,
      WRITE_TO_BUFFER_MODE_CONFIRM,
      
      BLOCK_ERASE_MODE,
      
      BLOCK_LOCK_BIT_MODE,
      
      SET_BLOCK_LOCK_BIT_MODE,
      CLEAR_BLOCK_LOCK_BIT_MODE
    };
    typedef enum Mode Mode;
    
    enum StatusRegBit
    {
      SR0 = (1 << 0),
      SR1 = (1 << 1),
      SR2 = (1 << 2),
      SR3 = (1 << 3),
      SR4 = (1 << 4),
      SR5 = (1 << 5),
      SR6 = (1 << 6),
      SR7 = (1 << 7),
      WSM_READY_BIT = (1 << 7)
    };
    typedef enum StatusRegBit StatusRegBit;
    
    enum BlockStatusRegBit
    {
      LOCK_BIT = (1 << 0)
    };
    typedef enum BlockStatusRegBit BlockStatusRegBit;
    
    enum ExtStatusRegBit
    {
      WRITE_BUFFER_READY_BIT = (1 << 7)
    };
    typedef enum ExtStatusRegBit ExtStatusRegBit;
    
    // Write buffer
    
    static uint32_t const WRITE_BUFFER_SIZE = 32; // in bytes
    
    // Command
    
    // :NOTE: Wei 2004-May-02:
    //
    // Linux kernel will try these 2 values,
    // see linux-2.4.25/drivers/mtd/chips/cfi_probe.c
    //
    // :NOTE: Wei 2004-May-03:
    //
    // 0xFF for Intel, 0xF0 for AMD is what I found when I trace code into bootldr.
    // see bootldr/btflash.c: queryFlashID()
    static uint32_t const READ_ARRAY_COMMAND_INTEL = 0xFF;
    static uint32_t const READ_ARRAY_COMMAND_AMD = 0xF0;
    
    static uint32_t const READ_ID_CODES_COMMAND = 0x90;
    static uint32_t const READ_QUERY_COMMAND = 0x98;
    static uint32_t const READ_STATUS_REG_COMMAND = 0x70;
    static uint32_t const CLEAR_STATUS_REG_COMMAND = 0x50;
    
    static uint32_t const BLOCK_LOCK_BIT_COMMAND = 0x60;
    
    static uint32_t const SET_BLOCK_LOCK_BIT_COMMAND = 0x01;
    static uint32_t const CLEAR_BLOCK_LOCK_BIT_COMMAND = 0xD0;
    
    static uint32_t const BYTE_WORD_PROGRAM_COMMAND_1 = 0x40;
    static uint32_t const BYTE_WORD_PROGRAM_COMMAND_2 = 0x10;
    
    static uint32_t const WRITE_TO_BUFFER_COMMAND = 0xE8;
    static uint32_t const WRITE_TO_BUFFER_COMMAND_CONFIRM = 0xD0;
    
    static uint32_t const BLOCK_ERASE_COMMAND = 0x20;
    static uint32_t const BLOCK_ERASE_COMMAND_CONFIRM = 0xD0;
  };
}

#endif
