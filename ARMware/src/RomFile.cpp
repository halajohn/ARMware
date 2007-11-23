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

#include <iostream>

#include <cassert>

#include "Log.hpp"
#include "RomFile.hpp"
#include "Egpio.hpp"
#include "Core.hpp"
#include "MachineType.hpp"

#include "RomFile.tcc"

namespace ARMware
{
  ////////////////////////////////// Private //////////////////////////////////
  //============================== Operation ==================================
  
#if LOAD_ROM_INTO_MEMORY
  void
  RomFile::read_rom_into_memory()
  {
    m_file_stream.seekg(0, std::ios::end);
    
    // Check whether the provided image is larger than the size of our partition or not.
    uint32_t const size = std::streamoff(m_file_stream.tellg());
    
    if (size > MachineType::FLASH_SIZE)
    {
      g_log_file << "WARNING: The size of flash ROM file is larger than the that of flash ROM device." << std::endl;
    }
    
    m_file_stream.seekg(0, std::ios::beg);
    
    m_file_stream.read(reinterpret_cast<char *>(mp_rom_data), MachineType::FLASH_SIZE);
  }
#endif
  
  ////////////////////////////////// Public ///////////////////////////////////
  //============================== Life cycle =================================
  
  RomFile::RomFile(Core * const core, Egpio * const egpio, std::string const &filename)
    : mp_core(core),
      mpEgpio(egpio),
#if LOAD_ROM_INTO_MEMORY
      mp_rom_data(new uint8_t[MachineType::FLASH_SIZE]),
#endif
      // :NOTE: Wei 2004-May-25:
      //
      // In the soft emulator, I think the write buffer will always be ready,
      // thus I make the mExtStatusReg variable a constant,
      // and initialize it here.
      mExtStatusReg(MachineType::FlashInfo::WRITE_BUFFER_READY_BIT)
  {
#if LOAD_ROM_INTO_MEMORY
    memset(mp_rom_data, 0, MachineType::FLASH_SIZE); // initialize to 0
#endif
    
    m_file_stream.open(filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    
    assert(true == m_file_stream.is_open());
    
    reset();
    
#if LOAD_ROM_INTO_MEMORY
    read_rom_into_memory();
#endif
  }
  
  RomFile::~RomFile()
  {
#if LOAD_ROM_INTO_MEMORY
    if (mp_rom_data != 0)
    {
      delete[] mp_rom_data;
    }
#endif
  }
  
  //=============================== Operation =================================
  
  void
  RomFile::reset()
  {
#if CONFIG_MACH_H3600
    // :Intel StrataFlash Memory Datasheet: p.19: Wei 2004-Jan-09:
    //
    // Upon initial device power-up and after exit from reset/power-down mode,
    // the device defaults to read array mode.
    mMode = MachineType::FlashInfo::READ_ARRAY_MODE;
    
    m_status_reg = static_cast<uint8_t>(0);
    SetWSMReady(); // Initialize WSM in ready mode.
    
    // :Intel StrataFlash Memory Datasheet: p.17: Wei 2004-May-02:
    //
    // The CUI is reset to read array mode and status register is set to 0x80.
    // i.e. all blocks are ready.
    for (uint32_t i = 0; i < MachineType::BLOCK_COUNT; ++i)
    {
      mBlockStatusReg[i] = 0x80;
    }
#endif
  }
  
#if CONFIG_MACH_H3600
  template<>
  uint32_t
  RomFile::get_data(uint32_t const address) const
  {
    // :NOTE: Wei 2004-May-04:
    //
    // see the comments in RomFile.hpp,
    // H3600 CPU should send an address meets 4-byte (32-bit) alignment.
    assert(0 == (address & MachineType::INVALID_ADDR_BITMASK));
    
    switch (mMode)
    {
    case MachineType::FlashInfo::READ_ARRAY_MODE:
      return read_from_rom<uint32_t>(address);
      
    case MachineType::FlashInfo::READ_ID_CODES_MODE:
      // :NOTE: Wei 2004-May-06:
      //
      // According to the Intel StrataFlash Memory Datasheet, p.47, Table 21.
      // read_query_command can read ID Codes, too.
      // Thus I process READ_ID_CODES_MODE & READ_QUERY_MODE together.
      //
      // The linux kernel use this query mode to read manufacturer code (at CFI address 0).
    case MachineType::FlashInfo::READ_QUERY_MODE:
      // :Intel StrataFlash Memory Datasheet: p.22: Wei 2004-May-06:
      //
      // Information contained in Query register can only be accessed by executing a
      // single-word (32-bit) read.
      //
      // :CFI Specification Release 2.0: p.7: Wei 2004-May-06:
      //
      // The Query addressing is always relative to the device word (largest supported)
      // with data always presented on the lowest order byte (D7 - D0 outputs).
      switch (address)
      {
      case BuildCFICpuAddress(0x0):
        // :Intel StrataFlash Memory Datasheet: p.20: Wei 2004-Jan-09:
        //
        // Manufacture Code: 0x89 - Intel
        return MachineType::BuildCFICmd(0x89);
        
      case BuildCFICpuAddress(0x1):
        // :Intel StrataFlash Memory Datasheet: p.20: Wei 2004-Jan-09:
        //
        // Device code: 64-Mbit
        //
        // :NOTE: Wei 2004-May-04:
        //
        // 64-Mbit == 8-Mbyte: means this device is a 8-Mbyte flash chip,
        // and H3600 uses 2 flash chips of this kind, thus H3600 provides 16-Mbyte
        // flash memory.
        return MachineType::BuildCFICmd(0x17);
        
        // :Intel StrataFlash Memory Datasheet: p.20: Wei 2004-Jan-09:
        //
        // Block Lock Configuration:
        //  0 - block is unlocked
        //  1 - block is locked
      case BuildCFICpuAddress(MachineType::BLOCK_0_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[0]);
      case BuildCFICpuAddress(MachineType::BLOCK_1_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[1]);
      case BuildCFICpuAddress(MachineType::BLOCK_2_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[2]);
      case BuildCFICpuAddress(MachineType::BLOCK_3_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[3]);
      case BuildCFICpuAddress(MachineType::BLOCK_4_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[4]);
      case BuildCFICpuAddress(MachineType::BLOCK_5_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[5]);
      case BuildCFICpuAddress(MachineType::BLOCK_6_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[6]);
      case BuildCFICpuAddress(MachineType::BLOCK_7_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[7]);
      case BuildCFICpuAddress(MachineType::BLOCK_8_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[8]);
      case BuildCFICpuAddress(MachineType::BLOCK_9_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[9]);
      case BuildCFICpuAddress(MachineType::BLOCK_10_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[10]);
      case BuildCFICpuAddress(MachineType::BLOCK_11_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[11]);
      case BuildCFICpuAddress(MachineType::BLOCK_12_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[12]);
      case BuildCFICpuAddress(MachineType::BLOCK_13_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[13]);
      case BuildCFICpuAddress(MachineType::BLOCK_14_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[14]);
      case BuildCFICpuAddress(MachineType::BLOCK_15_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[15]);
      case BuildCFICpuAddress(MachineType::BLOCK_16_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[16]);
      case BuildCFICpuAddress(MachineType::BLOCK_17_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[17]);
      case BuildCFICpuAddress(MachineType::BLOCK_18_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[18]);
      case BuildCFICpuAddress(MachineType::BLOCK_19_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[19]);
      case BuildCFICpuAddress(MachineType::BLOCK_20_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[20]);
      case BuildCFICpuAddress(MachineType::BLOCK_21_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[21]);
      case BuildCFICpuAddress(MachineType::BLOCK_22_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[22]);
      case BuildCFICpuAddress(MachineType::BLOCK_23_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[23]);
      case BuildCFICpuAddress(MachineType::BLOCK_24_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[24]);
      case BuildCFICpuAddress(MachineType::BLOCK_25_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[25]);
      case BuildCFICpuAddress(MachineType::BLOCK_26_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[26]);
      case BuildCFICpuAddress(MachineType::BLOCK_27_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[27]);
      case BuildCFICpuAddress(MachineType::BLOCK_28_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[28]);
      case BuildCFICpuAddress(MachineType::BLOCK_29_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[29]);
      case BuildCFICpuAddress(MachineType::BLOCK_30_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[30]);
      case BuildCFICpuAddress(MachineType::BLOCK_31_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[31]);
      case BuildCFICpuAddress(MachineType::BLOCK_32_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[32]);
      case BuildCFICpuAddress(MachineType::BLOCK_33_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[33]);
      case BuildCFICpuAddress(MachineType::BLOCK_34_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[34]);
      case BuildCFICpuAddress(MachineType::BLOCK_35_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[35]);
      case BuildCFICpuAddress(MachineType::BLOCK_36_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[36]);
      case BuildCFICpuAddress(MachineType::BLOCK_37_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[37]);
      case BuildCFICpuAddress(MachineType::BLOCK_38_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[38]);
      case BuildCFICpuAddress(MachineType::BLOCK_39_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[39]);
      case BuildCFICpuAddress(MachineType::BLOCK_40_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[40]);
      case BuildCFICpuAddress(MachineType::BLOCK_41_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[41]);
      case BuildCFICpuAddress(MachineType::BLOCK_42_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[42]);
      case BuildCFICpuAddress(MachineType::BLOCK_43_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[43]);
      case BuildCFICpuAddress(MachineType::BLOCK_44_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[44]);
      case BuildCFICpuAddress(MachineType::BLOCK_45_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[45]);
      case BuildCFICpuAddress(MachineType::BLOCK_46_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[46]);
      case BuildCFICpuAddress(MachineType::BLOCK_47_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[47]);
      case BuildCFICpuAddress(MachineType::BLOCK_48_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[48]);
      case BuildCFICpuAddress(MachineType::BLOCK_49_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[49]);
      case BuildCFICpuAddress(MachineType::BLOCK_50_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[50]);
      case BuildCFICpuAddress(MachineType::BLOCK_51_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[51]);
      case BuildCFICpuAddress(MachineType::BLOCK_52_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[52]);
      case BuildCFICpuAddress(MachineType::BLOCK_53_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[53]);
      case BuildCFICpuAddress(MachineType::BLOCK_54_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[54]);
      case BuildCFICpuAddress(MachineType::BLOCK_55_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[55]);
      case BuildCFICpuAddress(MachineType::BLOCK_56_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[56]);
      case BuildCFICpuAddress(MachineType::BLOCK_57_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[57]);
      case BuildCFICpuAddress(MachineType::BLOCK_58_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[58]);
      case BuildCFICpuAddress(MachineType::BLOCK_59_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[59]);
      case BuildCFICpuAddress(MachineType::BLOCK_60_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[60]);
      case BuildCFICpuAddress(MachineType::BLOCK_61_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[61]);
      case BuildCFICpuAddress(MachineType::BLOCK_62_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[62]);
      case BuildCFICpuAddress(MachineType::BLOCK_63_BEGIN_LOC + 2): return MachineType::BuildCFICmd(mBlockStatusReg[63]);
        
      case BuildCFICpuAddress(0x10):
        // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-Jan-09:
        //
        // 'Q'
        return MachineType::BuildCFICmd(0x51);
        
      case BuildCFICpuAddress(0x11):
        // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-Jan-09:
        //
        // 'R'
        return MachineType::BuildCFICmd(0x52);
        
      case BuildCFICpuAddress(0x12):
        // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-Jan-09:
        //
        // 'Y'
        return MachineType::BuildCFICmd(0x59);
        
      case BuildCFICpuAddress(0x13):
        // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-Jan-09:
        //
        // bytes[0x13:0x14] - Primary vendor command set and control interface ID code.
        //                   16-bit ID code for vendor-specified algorighm.
        //
        // Intel's value = 0x0001 (16-bit)
        //
        // :NOTE: Wei 2004-Jan-09:
        //
        // AMD's value = 0x0002 (16-bit)
        
        // :NOTE: Wei 2004-May-05:
        //
        // However, because of H3600 uses 16-bit flash chip, thus the value stored in
        // CFI address 0x13 will be expanded to 0x0001 (the high byte is filled by 0x00)
        // and the value stored in CFI address 0x14 will be expanded 0x0000.
        // (the high byte is filled by 0x00, t00)
        //
        // 0x0000 0001
        //   ---- ----
        // 0x 00   01
        return MachineType::BuildCFICmd(0x1);
        
      case BuildCFICpuAddress(0x14):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x15):
        // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-May-03:
        //
        // bytes[0x15:0x16] - Extended Query Table primary algorithm address.
        return MachineType::BuildCFICmd(0x31);
        
      case BuildCFICpuAddress(0x16):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x17):
        // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-May-03:
        //
        // bytes[0x17:0x18] - Alternate vendor command set and control interface ID code.
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x18):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x19):
        // :Intel StrataFlash Memory Datasheet: p.47: Wei 2004-May-03:
        //
        // bytes[0x19:0x1A] - Secondary algorithm Extended Query Table address.
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x1A):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x1B):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // VCC logic supply minimum program/erase voltage
        return MachineType::BuildCFICmd(0x27);
        
      case BuildCFICpuAddress(0x1C):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // VCC logic supply maximum program/erase voltage
        return MachineType::BuildCFICmd(0x36);
        
      case BuildCFICpuAddress(0x1D):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // VPP [programming] supply minimum program/erase voltage
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x1E):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // VPP [programming] supply maximum program/erase voltage
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x1F):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that typical single word program time-out = 2^n us
        return MachineType::BuildCFICmd(0x08);
        
      case BuildCFICpuAddress(0x20):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that typical max. buffer write time-out = 2^n us
        return MachineType::BuildCFICmd(0x08);
        
      case BuildCFICpuAddress(0x21):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that typical block erase time-out = 2^n ms
        return MachineType::BuildCFICmd(0x0A);
        
      case BuildCFICpuAddress(0x22):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that typical full chip erase time-out = 2^n ms
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x23):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that maximum word program time-out = 2^n times typical
        return MachineType::BuildCFICmd(0x04);
        
      case BuildCFICpuAddress(0x24):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that maximum buffer write time-out = 2^n times typical
        return MachineType::BuildCFICmd(0x04);
        
      case BuildCFICpuAddress(0x25):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that maximum block erase time-out = 2^n times typical
        return MachineType::BuildCFICmd(0x04);
        
      case BuildCFICpuAddress(0x26):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that maximum chip erase time-out = 2^n times typical
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x27):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that device size = 2^n in number of bytes
        return MachineType::BuildCFICmd(0x17);
        
      case BuildCFICpuAddress(0x28):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 0x28 & 0x29: Flash device interface: x8/x16 async
        return MachineType::BuildCFICmd(0x2);
        
      case BuildCFICpuAddress(0x29):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x2A):
        // :Intel StrataFlash Memory Datasheet: p.48: Wei 2004-May-03:
        //
        // 'n' such that maximum number of bytes in write buffer = 2^n.
        //
        // (i.e. 32-byte write buffer)
        return MachineType::BuildCFICmd(0x5);
        
      case BuildCFICpuAddress(0x2B):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x2C):
        // :Intel StrataFlash Memory Datasheet: p.49: Wei 2004-May-06:
        //
        // Number of erase block regions within device:
        return MachineType::BuildCFICmd(1);
        
      case BuildCFICpuAddress(0x2D):
        // :Intel StrataFlash Memory Datasheet: p.49: Wei 2004-May-03:
        //
        // bytes[0x2D:0x30] - Erase Block Region 1 Information
        //
        // (i.e. 64 identical-size erase blocks)
        return MachineType::BuildCFICmd(0x3F);
        
      case BuildCFICpuAddress(0x2E):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x2F):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x30):
        // :Intel StrataFlash Memory Datasheet: p.49: Wei 2004-May-03:
        //
        // 128-kb erase block size.
        return MachineType::BuildCFICmd(0x2);
        
      case BuildCFICpuAddress(0x31):
        // :Intel StrataFlash Memory Datasheet: p.49: Wei 2004-May-07:
        //
        // bytes[0x31:0x33] - Primary extended query table, Unique ASCII string 'PRI'.
        return MachineType::BuildCFICmd(0x50);
        
      case BuildCFICpuAddress(0x32):
        return MachineType::BuildCFICmd(0x52);
        
      case BuildCFICpuAddress(0x33):
        return MachineType::BuildCFICmd(0x49);
        
      case BuildCFICpuAddress(0x34):
        // :Intel StrataFlash Memory Datasheet: p.49: Wei 2004-May-07:
        //
        // Major version number, ASCII
        return MachineType::BuildCFICmd(0x31);
        
      case BuildCFICpuAddress(0x35):
        // :Intel StrataFlash Memory Datasheet: p.49: Wei 2004-May-07:
        //
        // Minor version number, ASCII
        return MachineType::BuildCFICmd(0x31);
        
      case BuildCFICpuAddress(0x36):
        // :Intel StrataFlash Memory Datasheet: p.50: Wei 2004-May-07:
        //
        // bytes[0x36:0x39] - Optional feature and command support
        //
        // :NOTE: Wei 2004-May-07:
        //
        // According to the Table 26 at page 50.
        // The value at byte 0x36 is 0x0A, but the value of the bit fields
        // is 0xCE (11001110). I think 0xCE is more correct than 0x0A,
        // thus I use 0xCE as the value of byte 0x36.
        return MachineType::BuildCFICmd(0xCE);
        
      case BuildCFICpuAddress(0x37):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x38):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x39):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x3A):
        // :Intel StrataFlash Memory Datasheet: p.50: Wei 2004-May-07:
        //
        // Supported functions after suspend: read Array, Status, Query
        return MachineType::BuildCFICmd(0x1);
        
      case BuildCFICpuAddress(0x3B):
        // :Intel StrataFlash Memory Datasheet: p.50: Wei 2004-May-07:
        //
        // bytes[0x3B:0x3C] - Block status register mask
        return MachineType::BuildCFICmd(0x1);
        
      case BuildCFICpuAddress(0x3C):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x3D):
        // :Intel StrataFlash Memory Datasheet: p.50: Wei 2004-May-07:
        //
        // VCC logic supply highest performance program/erase voltage
        return MachineType::BuildCFICmd(0x33);
        
      case BuildCFICpuAddress(0x3E):
        // :Intel StrataFlash Memory Datasheet: p.50: Wei 2004-May-07:
        //
        // VPP optimum program/erase supply voltage
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x3F):
        // :Intel StrataFlash Memory Datasheet: p.51: Wei 2004-May-07:
        //
        // Number of Protection register fields in JEDEC ID space.
        return MachineType::BuildCFICmd(0x1);
        
      case BuildCFICpuAddress(0x40):
        // :Intel StrataFlash Memory Datasheet: p.51: Wei 2004-May-07:
        //
        // bytes[0x40:0x43] - Protection Field 1: Protection Description
        return MachineType::BuildCFICmd(0x80);
        
      case BuildCFICpuAddress(0x41):
        return MachineType::BuildCFICmd(0);
        
      case BuildCFICpuAddress(0x42):
        return MachineType::BuildCFICmd(0x3);
        
      case BuildCFICpuAddress(0x43):
        return MachineType::BuildCFICmd(0x3);
        
      default:
#if TRACE_FLASH
        g_log_file << std::hex << "FLASH: unknown query address = " << std::hex << address << std::endl;
#endif
        return 0; // I assume invalid address return 0
      }
      break;
      
    case MachineType::FlashInfo::READ_STATUS_REG_MODE:
#if TRACE_FLASH
      g_log_file << "FLASH: read status register" << std::endl;
#endif
      
      if (true == WSMIsReady())
      {
        // :Intel StrataFlash Memory Datasheet: p.20: Wei 2004-Jan-09:
        //
        // After writing command - READ_STATUS_REG_COMMAND,
        // all subsequent read operations output data from the Status Register
        // until another valid command is written.
        
        // :NOTE: Wei 2004-May-03:
        //
        // Because H3600 has 2 flash chips, thus I should return m_status_reg using
        // MachineType::BuildCFICmd() function.
        return MachineType::BuildCFICmd(m_status_reg);
      }
      else
      {
        assert(!"Should not reach here.");
        
        // :Intel StrataFlash Memory Datasheet: p.21: Wei 2004-May-25:
        //
        // SR[6:0] are not driven while SR7 = 0. 
        return 0;
      }
      
    case MachineType::FlashInfo::BLOCK_LOCK_BIT_MODE:
    case MachineType::FlashInfo::SET_BLOCK_LOCK_BIT_MODE:
    case MachineType::FlashInfo::CLEAR_BLOCK_LOCK_BIT_MODE:
      // :NOTE: Wei 2004-May-03:
      //
      // Because I will set the flash mode to READ_STATUS_REG_MODE right after
      // set block lock bit and clear block lock bit operation,
      // thus if some codes want to read from flash in these 4 modes,
      // it probably means there are some bugs in ARMware implementation.
      assert(!"Should not reach here.");
      return 0;
      
    case MachineType::FlashInfo::WRITE_TO_BUFFER_MODE_BEGIN:
      return MachineType::BuildCFICmd(mExtStatusReg);
      
    default:
      g_log_file << std::hex << "FLASH: unknown flash mode, address = " << address << std::endl;
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  template<>
  void
  RomFile::put_data(uint32_t const address, uint32_t const value)
  {
#if TRACE_FLASH
    g_log_file << "FLASH: put " << std::hex << value << " to address " << address << std::endl;
#endif
    
    // :NOTE: Wei 2004-May-04:
    //
    // see the comments in RomFile.hpp,
    // H3600 CPU should send an address meets 4-byte (32-bit) alignment.
    assert(0 == (address & MachineType::INVALID_ADDR_BITMASK));
    
    switch (mMode)
    {
    case MachineType::FlashInfo::BLOCK_LOCK_BIT_MODE:
      switch (value & MachineType::BANK_BITMASK)
      {
      case MachineType::FlashInfo::SET_BLOCK_LOCK_BIT_COMMAND:
        // :NOTE: Wei 2004-May-03:
        //
        // Althought at page 26 in the Intel's Datasheet doesn't specify that the
        // 'Setup' stage should provide an address.
        // However, at page 58 in the Intel's Datasheet indeed specify that the
        // 'Setup' stage should provide an address, and that address should match
        // the address provided by the 'Confirm' stage.
        //
        // According to the Linux kernel source, kernel's behavior also confirms
        // the behavior at page 58.
        //
        // As a result, I decide to make this check.
        assert(mCurrentAddress == address);
        
        // :Intel StrataFlash Memory Datasheet: p.17: Wei 2004-May-03:
        //
        // When the VPEN voltage d VPENLK, only read operations from the Status Register,
        // CFI, identifier codes, or blocks are enabled. Placing VPENH on VPEN additionally
        // enables block erase, program, and lock-bit configuration operations.
        //
        // :Intel StrataFlash Memory Datasheet: p.26: Wei 2004-May-03:
        //
        // With VPEN <= VPENLK, lock-bit contents are protected against alteration.
        if (true == mpEgpio->FlashVPenIsEnabled())
        {
          // :NOTE: Wei 2004-May-03:
          //
          // Becuase I will reset the flash mode to READ_STATUS_REG_MODE in the
          // end of clear block lock bit operation stage 2,
          // thus the following setting mode statement will have no real effect.
          //
          // Becuase of the performance, I comment out this statement to
          // decrease the instruction counts that ARMware need to preform.
          
          // mMode = MachineType::FlashInfo::SET_BLOCK_LOCK_BIT_MODE;
          
#if TRACE_FLASH
          g_log_file << std::hex << "FLASH: lock " << address << std::endl;
#endif
          
          LockBlock(address);
        }
        else // FlashVPenIsEnable()
        {
          assert(!"Should not reach here.");
          
          // :Intel StrataFlash Memory Datasheet: p.58: Wei 2004-May-03:
          //
          // SR3: 1 = Programming Voltage Error Detect
          SetStatusBit(MachineType::FlashInfo::SR3);
        }
        break;
        
      case MachineType::FlashInfo::CLEAR_BLOCK_LOCK_BIT_COMMAND:
        if (true == mpEgpio->FlashVPenIsEnabled())
        {
          // :NOTE: Wei 2004-May-03:
          //
          // Becuase I will reset the flash mode to READ_STATUS_REG_MODE in the
          // end of clear block lock bit operation stage 2,
          // thus the following setting mode statement will have no real effect.
          //
          // Becuase of the performance, I comment out this statement to
          // decrease the instruction counts that ARMware need to preform.
          
          // mMode = MachineType::FlashInfo::CLEAR_BLOCK_LOCK_BIT_MODE;
          
#if TRACE_FLASH
          g_log_file << std::hex << "FLASH: unlock " << address << std::endl;
#endif
          
          for (uint32_t i = 0; i < MachineType::BLOCK_COUNT; ++i)
          {
            mBlockStatusReg[i] &= ~MachineType::FlashInfo::LOCK_BIT;
          }
        }
        else
        {
          assert(!"Should not reach here.");
          
          SetStatusBit(MachineType::FlashInfo::SR3);
        }
        break;
        
      default:
        assert(!"Should not reach here.");
        
        // :Intel StrataFlash Memory Datasheet: p.58: Wei 2004-May-03:
        //
        // SR4 & SR5: Both 1 = Command Sequence Error
        SetStatusBit(MachineType::FlashInfo::SR4);
        SetStatusBit(MachineType::FlashInfo::SR5);
        break;
      }
      
      SetWSMReady();
      
      // :Intel StrataFlash Memory Datasheet: p.26: Wei 2004-May-03:
      //
      // After the sequence is written, the device automatically outputs
      // Status Register data when read
      // ...
      // The CUI will remain in Read Status Register mode until a new command
      // is issued.
      mMode = MachineType::FlashInfo::READ_STATUS_REG_MODE;
      break;
      
    case MachineType::FlashInfo::BYTE_WORD_PROGRAM_MODE:
      // :Intel StrataFlash Memory Datasheet: p.54: Wei 2004-May-03:
      assert(mCurrentAddress == address);
      
      if (true == IsBlockLocked(address))
      {
#if TRACE_FLASH
        g_log_file << "FLASH: block is locked when program byte/word: " << std::hex << address << std::endl;
#endif
        
        assert(!"Should not reach here.");
        
        // :Intel StrataFlash Memory Datasheet: p.22: Wei 2004-May-03:
        //
        // If a byte/word program is attempted when the corresponding block lock-bit is set,
        // SR1 and SR4 will be set.
        SetStatusBit(MachineType::FlashInfo::SR1);
        SetStatusBit(MachineType::FlashInfo::SR4);
      }
      else
      {
        if (true == mpEgpio->FlashVPenIsEnabled())
        {
#if TRACE_FLASH
          g_log_file << "FLASH: program byte/word: address: " << std::hex << address << ", value: " << value << std::endl;
#endif
          
          write_to_rom(address, value);
        }
        else
        {
          assert(!"Should not reach here.");
          
          // :Intel StrataFlash Memory Datasheet: p.22: Wei 2004-May-03:
          //
          // If a byte/word program is attempted while VPEN <= VPENLK,
          // SR4 and SR3 will be set.
          SetStatusBit(MachineType::FlashInfo::SR3);
          SetStatusBit(MachineType::FlashInfo::SR4);
        }
      }
      
      SetWSMReady();
      
      // :Intel StrataFlash Memory Datasheet: p.22: Wei 2004-May-02:
      //
      // After the program sequence is written, the device automatically outputs
      // SRD (status register data)when read.
      mMode = MachineType::FlashInfo::READ_STATUS_REG_MODE;
      break;
      
    case MachineType::FlashInfo::WRITE_TO_BUFFER_MODE_BEGIN:
      assert(address == mCurrentAddress);
      
      mDataCount = (value & MachineType::BANK_BITMASK) + 1;
      
      mMode = MachineType::FlashInfo::WRITE_TO_BUFFER_MODE_DATA;
      
      mWriteBufferCurrIndex = 0; // Initialize to 0
      break;
      
    case MachineType::FlashInfo::WRITE_TO_BUFFER_MODE_DATA:
      mWriteBufferAddr[mWriteBufferCurrIndex] = address;
      
      // :NOTE: Wei 2004-May-25:
      //
      // No need to and with MachineType::BANK_BITMASK.
      // See linux kernel source 'drivers/mtd/chips/cfi_cmdset_0001.c'::do_write_buffer()
      mWriteBufferData[mWriteBufferCurrIndex] = value;
      
#if TRACE_FLASH
      g_log_file << "FLASH: write to buffer: index = " << mWriteBufferCurrIndex
                << ", address: " << std::hex << address << ", value: " << value << std::endl;
#endif
      
      ++mWriteBufferCurrIndex;
      
      if (mWriteBufferCurrIndex == mDataCount)
      {
        mMode = MachineType::FlashInfo::WRITE_TO_BUFFER_MODE_CONFIRM;
      }
      break;
      
    case MachineType::FlashInfo::WRITE_TO_BUFFER_MODE_CONFIRM:
      // :NOTE: Wei 2004-May-25:
      //
      // I have no idea whether this assert() is necessary or not.
      assert(address == mCurrentAddress);
      
      if (MachineType::FlashInfo::WRITE_TO_BUFFER_COMMAND_CONFIRM == (value & MachineType::BANK_BITMASK))
      {
        if (true == mpEgpio->FlashVPenIsEnabled())
        {
          if (true == IsBlockLocked(mCurrentAddress))
          {
#if TRACE_FLASH
            g_log_file << "FLASH: block is locked when write to buffer: " << std::hex << mCurrentAddress << std::endl;
#endif
            
            assert(!"Should not reach here.");
            
            // :Intel StrataFlash Memory Datasheet: p.23: Wei 2004-May-25:
            //
            // If a buffered write is attempted when the corresponding block lock-bit is set,
            // SR1 and SR4 will be set.
            SetStatusBit(MachineType::FlashInfo::SR1);
            SetStatusBit(MachineType::FlashInfo::SR4);
          }
          else
          {
            if ((mWriteBufferAddr[0] & MachineType::BLOCK_ADDR_BITMASK) ==
                (mWriteBufferAddr[--mWriteBufferCurrIndex] & MachineType::BLOCK_ADDR_BITMASK))
            {
              // write data from write buffer to flash image.
              
              // :Intel StrataFlash Memory Datasheet: p.23: Wei 2004-May-26:
              //
              // Subsequent writes provide additional device addresses and data,
              // depending on the count. All subsequent addresses must lie within
              // the start address plus the count.
              
              // :WARNING: Wei 2004-May-26:
              //
              // However, it doesn't specify that the addresses have to be sequential,
              // that is to say, the given addresses could be jumping,
              // although they all are lie within the start address plus the count.
              //
              // However, according to the linux kernel source,
              // I don't see any place that the addresses could not be sequential,
              // thus for simplicity and preformance, I assume the given addresses are
              // sequential, and just use a simply memcpy() to do the job.
              
#if TRACE_FLASH
              g_log_file << "FLASH: begin write to buffer: address: " << std::hex << mWriteBufferAddr[0]
                        << ", mDataCount = " << mDataCount<< std::endl;
#endif
              
#if LOAD_ROM_INTO_MEMORY
              memcpy(&(mp_rom_data[mWriteBufferAddr[0]]),
                     &(mWriteBufferData[0]),
                     mDataCount * sizeof(MachineType::DataBusWidth));
#else
#error "TODO"
#endif
            }
            else
            {
              assert(!"Should not reach here.");
              
              // :Intel StrataFlash Memory Datasheet: p.23: Wei 2004-May-26:
              //
              // Additionally, if the user attempts to program past an erase block boundary
              // with a Write to Buffer command, the device will abort the write to buffer
              // operation. This will generate an 'Invalid Command/Sequence' error and SR5 and SR4
              // will be set.
              SetStatusBit(MachineType::FlashInfo::SR4);
              SetStatusBit(MachineType::FlashInfo::SR5);
            }
          }
        }
        else
        {
          assert(!"Should not reach here.");
          
          // :Intel StrataFlash Memory Datasheet: p.23: Wei 2004-May-25:
          //
          // If a buffered write is attempted while VPEN <= VPENLK,
          // SR4 and SR3 will be set.
          SetStatusBit(MachineType::FlashInfo::SR3);
          SetStatusBit(MachineType::FlashInfo::SR4);
        }
      }
      else
      {
        assert(!"Should not reach here.");
        
        // :Intel StrataFlash Memory Datasheet: p.23: Wei 2004-May-25:
        //
        // If a command other than Write Confirm is written to the device,
        // an 'Invalid Command/Sequence' error will be generated and SR5 and SR4 will be set.
        SetStatusBit(MachineType::FlashInfo::SR4);
        SetStatusBit(MachineType::FlashInfo::SR5);
      }
      
      SetWSMReady();
      
      mMode = MachineType::FlashInfo::READ_STATUS_REG_MODE;
      break;
      
    case MachineType::FlashInfo::BLOCK_ERASE_MODE:
      if (MachineType::FlashInfo::BLOCK_ERASE_COMMAND_CONFIRM == (value & MachineType::BANK_BITMASK))
      {
        if (true == mpEgpio->FlashVPenIsEnabled())
        {
          if (true == IsBlockLocked(mCurrentAddress))
          {
#if TRACE_FLASH
            g_log_file << "FLASH: block is locked when erasing block: " << std::hex << mCurrentAddress << std::endl;
#endif
            
            assert(!"Should not reach here.");
            
            // :Intel StrataFlash Memory Datasheet: p.24: Wei 2004-May-28:
            //
            // If block erase is attempted when the corresponding block lock-bit is set,
            // SR1 and SR5 will be set.
            SetStatusBit(MachineType::FlashInfo::SR1);
            SetStatusBit(MachineType::FlashInfo::SR5);
          }
          else
          {
            // erase begining.
            
#if TRACE_FLASH
            g_log_file << "FLASH: begin block earsing: address: " << std::hex << mCurrentAddress << std::endl;
#endif
            
            // :Intel StrataFlash Memory Datasheet: p.24: Wei 2004-May-28:
            //
            // erase changes all block data to 0xFF.
            
#if LOAD_ROM_INTO_MEMORY
            memset(&(mp_rom_data[mCurrentAddress & MachineType::BLOCK_ADDR_BITMASK]),
                   0xFF,
                   MachineType::BLOCK_SIZE);
#else
#error "TODO"
#endif
          }
        }
        else
        {
          assert(!"Should not reach here.");
          
          // :Intel StrataFlash Memory Datasheet: p.24: Wei 2004-May-28:
          //
          // If block erase is attempted while VPEN d VPENLK, SR3 and SR5 will be set.
          SetStatusBit(MachineType::FlashInfo::SR3);
          SetStatusBit(MachineType::FlashInfo::SR5);
        }
      }
      else
      {
        assert(!"Should not reach here.");
        
        // :Intel StrataFlash Memory Datasheet: p.24: Wei 2004-May-28:
        //
        // An invalid Block Erase command sequence will result in both SR4 and SR5 being set.
        SetStatusBit(MachineType::FlashInfo::SR4);
        SetStatusBit(MachineType::FlashInfo::SR5);
      }
      
      SetWSMReady();
      
      mMode = MachineType::FlashInfo::READ_STATUS_REG_MODE;
      break;
      
    default:
      // :NOTE: Wei 2004-Jan-09:
      //
      // The hardware may be paralleled by more than one flash memory bank.
      // For example, iPaq supports 32-bit wide accesses to the flash memory,
      // but the underlying Intel flash memory can only support up to 16-bit accesses,
      // thus iPaq uses 2 flash memories paralleled to accomplished this 32-bit accesses.
      //
      // Thus, on such systems, when we need to issue flash commands,
      // we need to double it's value.
      // For example, when we want to put the 2 flash memories into read array mode,
      // we cann't just write 0xFF to the flash, but we need to write 0x00FF00FF to it.
      //                                                                  ^^
      // For this reason, this function may receive such a value that ought to be a command,
      // so I preform the AND 0xFFFF operation to discard the upper 16-bit part.
      switch (value & MachineType::BANK_BITMASK)
      {
      case MachineType::FlashInfo::READ_ARRAY_COMMAND_INTEL:
        mMode = MachineType::FlashInfo::READ_ARRAY_MODE;
        return;
        
      case MachineType::FlashInfo::READ_ARRAY_COMMAND_AMD:
        // :NOTE: Wei 2004-May-02:
        //
        // AMD device recognize this command, but Intel StrataFlash doesn't.
        // Linux kernel (version 2.4.25) will issue these commands (READ_ARRAY_COMMAND_INTEL &
        // READ_ARRAY_COMMAND_AMD) to ensure the underlying flash chip recognize this READ_ARRAY
        // command.
        return;
        
      case MachineType::FlashInfo::READ_ID_CODES_COMMAND:
        mMode = MachineType::FlashInfo::READ_ID_CODES_MODE;
        return;
        
      case MachineType::FlashInfo::READ_QUERY_COMMAND:
        // :NOTE: Wei 2004-Jan-09:
        //
        // CFI specification says that 0x98 could write to the address 0x55.
        // However, Intel StrataFlash Memory Datasheet doesn't have this restriction.
        
        // :CFI Specification, v2.0: p.8: Wei 2004-May-03:
        //
        // Flash devices may or may not have address sensitive query commands.
        // Device drivers should always supply 55h on the address bus and 98h on the data
        // bus to enter query mode, however Flash devices may choose to ignore the address
        // bus and enter query mode if 98h is seen on the data bus only
        
        // :NOTE: Wei 2004-May-03:
        //
        // That is why I comment out the following assert statement.
        
        //assert(BuildCFICpuAddress(0x55) == address);
        
        mMode = MachineType::FlashInfo::READ_QUERY_MODE;
        return;
        
      case MachineType::FlashInfo::READ_STATUS_REG_COMMAND:
        mMode = MachineType::FlashInfo::READ_STATUS_REG_MODE;
        return;
        
      case MachineType::FlashInfo::CLEAR_STATUS_REG_COMMAND:
        m_status_reg = static_cast<uint8_t>(0);
        return;
        
      case MachineType::FlashInfo::BLOCK_LOCK_BIT_COMMAND:
#if TRACE_FLASH
        g_log_file << "FLASH: block lock bit command" << std::endl;
#endif
        
        if (true == WSMIsReady())
        {
          // :Intel StrataFlash Memory Datasheet: p.26: Wei 2004-May-03:
          //
          // Set block lock bit command is invalid while the WSM is running
          // or the device is suspended.
          //
          // :NOTE: Wei 2004-May-03:
          //
          // However, I don't see I would (or should ?) support the flash suspending
          // functionality in ARMware in the near future.
          SetWSMBusy();
          
          mMode = MachineType::FlashInfo::BLOCK_LOCK_BIT_MODE;
          
          // :NOTE: Wei 2004-May-05:
          //
          // Used only for set block lock bit.
          mCurrentAddress = address;
        }
        else
        {
          assert(!"Should not reach here.");
        }
        return;
        
      case MachineType::FlashInfo::BYTE_WORD_PROGRAM_COMMAND_1:
      case MachineType::FlashInfo::BYTE_WORD_PROGRAM_COMMAND_2:
#if TRACE_FLASH
        g_log_file << "FLASH: program command" << std::endl;
#endif
        
        if (true == WSMIsReady())
        {
          SetWSMBusy();
          
          mMode = MachineType::FlashInfo::BYTE_WORD_PROGRAM_MODE;
          
          mCurrentAddress = address;
        }
        else
        {
          assert(!"Should not reach here.");
        }
        return;
        
      case MachineType::FlashInfo::WRITE_TO_BUFFER_COMMAND:
#if TRACE_FLASH
        g_log_file << "FLASH: write to buffer command" << std::endl;
#endif
        
        if (true == WSMIsReady())
        {
          // :Intel StrataFlash Memory Datasheet: p.23: Wei 2004-May-27:
          //
          // Any time SR4 and/or SR5 is set, the device will not accept any more Write to Buffer commands.
          switch (m_status_reg & (MachineType::FlashInfo::SR4 | MachineType::FlashInfo::SR5))
          {
          case MachineType::FlashInfo::SR4:
          case MachineType::FlashInfo::SR5:
          case MachineType::FlashInfo::SR4 | MachineType::FlashInfo::SR5:
            assert(!"Should not reach here.");
            break;
            
          default:
            SetWSMBusy();
            
            mMode = MachineType::FlashInfo::WRITE_TO_BUFFER_MODE_BEGIN;
            
            mCurrentAddress = address;
            break;
          }
        }
        else
        {
          assert(!"Should not reach here.");
        }
        return;
        
      case MachineType::FlashInfo::BLOCK_ERASE_COMMAND:
#if TRACE_FLASH
        g_log_file << "FLASH: block erase command" << std::endl;
#endif
        
        if (true == WSMIsReady())
        {
          SetWSMBusy();
          
          mMode = MachineType::FlashInfo::BLOCK_ERASE_MODE;
          
          mCurrentAddress = address;
        }
        else
        {
          assert(!"Should not reach here.");
        }
        return;
        
      default:
#if ENABLE_INST_RECALL
        mp_core->DumpInstRecallBuffer();
#endif
        
        g_log_file << "unknown flash command: " << std::hex << value << std::endl;
        g_log_file << std::hex << " - address = " << address << std::endl;
        assert(!"Should not reach here.");
        return;
      }
    }
  }
#endif // CONFIG_MACH_H3600
}
