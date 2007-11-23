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

#ifndef RomFile_hpp
#define RomFile_hpp

// System include
//

#if TRACE_FLASH
#include <iostream>
#endif

#include <fstream>
#include <string>

// Project include
//

#include "ARMware.hpp"

// Local include
//

#include "MachineType.hpp"

namespace ARMware
{
  // Forward declaration
  //
  class Core;
  class Egpio;
  
  typedef class RomFile RomFile;
  class RomFile
  {
  private:
    
    // Attribute
    
    Core * const mp_core;
    Egpio * const mpEgpio;
    
    std::fstream m_file_stream;
    
#if LOAD_ROM_INTO_MEMORY
    uint8_t * const mp_rom_data;
#endif
    
    MachineType::FlashInfo::Mode mMode;
    
    // :NOTE: Wei 2004-Jan-10:
    //
    // According to the familiar's bootldr, 28F640J3 has 65 sectors,
    // thus it has 65 block status register.
    //
    // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-Jan-09:
    //
    // Block status register:
    //
    // Bit 0: Block Lock Status
    // Bit 1 ~ 7: Reserved for Future Use
    uint8_t mBlockStatusReg[MachineType::BLOCK_COUNT];
    uint32_t mCurrentAddress;
    
    uint8_t m_status_reg;
    uint8_t const mExtStatusReg;
    
    uint32_t mDataCount; // Used in 'write to buffer' mode.
    
    uint32_t mWriteBufferAddr[MachineType::WRITE_BUFFER_SIZE / sizeof(MachineType::DataBusWidth)];
    uint32_t mWriteBufferData[MachineType::WRITE_BUFFER_SIZE / sizeof(MachineType::DataBusWidth)];
    
    uint32_t mWriteBufferCurrIndex;
    
    // Operation
    
#if LOAD_ROM_INTO_MEMORY
    void read_rom_into_memory();
#endif
    
    template<typename T_return>
    T_return read_from_rom(uint32_t const address) const;
    
    template<typename T_store>
    void write_to_rom(uint32_t const address, T_store const value);
    
    inline void
    LockBlock(uint32_t const address)
    {
      mBlockStatusReg[MachineType::BlockAddrToIndex(address & MachineType::BLOCK_ADDR_BITMASK)] |=
        MachineType::FlashInfo::LOCK_BIT;
    }
    
    inline void
    UnlockBlock(uint32_t const address)
    {
      mBlockStatusReg[MachineType::BlockAddrToIndex(address & MachineType::BLOCK_ADDR_BITMASK)] &=
        ~MachineType::FlashInfo::LOCK_BIT;
    }
    
    inline bool
    IsBlockLocked(uint32_t const address)
    {
      if (MachineType::FlashInfo::LOCK_BIT ==
          (mBlockStatusReg[MachineType::BlockAddrToIndex(address & MachineType::BLOCK_ADDR_BITMASK)] &
           MachineType::FlashInfo::LOCK_BIT))
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    
    // Status register modify
    
    inline void
    SetStatusBit(MachineType::FlashInfo::StatusRegBit bit)
    { m_status_reg |= static_cast<uint8_t>(bit); }
    
    inline void
    ClearStatusBit(MachineType::FlashInfo::StatusRegBit bit)
    { m_status_reg &= ~(static_cast<uint8_t>(bit)); }
    
    // WSM status
    
    inline bool
    WSMIsReady() const
    {
      return
        (MachineType::FlashInfo::WSM_READY_BIT == (m_status_reg & MachineType::FlashInfo::WSM_READY_BIT))
        ? true
        : false;
    }
    
    inline void
    SetWSMReady()
    {
#if TRACE_FLASH
      g_log_file << "FLASH: Write State Machine ready." << std::endl;
#endif
      
      m_status_reg |= MachineType::FlashInfo::WSM_READY_BIT;
    }
    
    inline void
    SetWSMBusy()
    {
#if TRACE_FLASH
      g_log_file << "FLASH: Write State Machine busy." << std::endl;
#endif
      
      m_status_reg &= ~MachineType::FlashInfo::WSM_READY_BIT;
    }
    
  public:
    
    // Life cycle
    
    RomFile(Core * const core, Egpio * const egpio, std::string const &filename);
    
    ~RomFile();
    
    // Operation
    
    void reset();
    
    template<typename T_return>
    T_return FASTCALL get_data(uint32_t const address) const;
    
    template<typename T_store>
    void FASTCALL put_data(uint32_t const address, T_store const value);
  };
  
  // :NOTE: Wei 2004-May-07:
  //
  // Declare get_data() and put_data() have an explicit specification.
  // Then other files reference these 2 functions can know the existence of them,
  // and no instantiate a new form from the general form of get_data() and put_data()
  // template.
  template<>
  uint32_t FASTCALL RomFile::get_data(uint32_t const address) const;
  
  template<>
  void FASTCALL RomFile::put_data(uint32_t const address, uint32_t const value);
}

#endif
