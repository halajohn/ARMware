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

#ifndef MachineType_hpp
#define MachineType_hpp

#include "FlashInfo.hpp"

namespace ARMware
{
  /////////////////////////////////////////////////////////////////////////////
  
  enum MachineTypeEnum
  {
    COMPAQ_IPAQ_H3600 = 0
  };
  typedef enum MachineTypeEnum MachineTypeEnum;
  
  enum MaxDataBusWidth
  {
    X8 = 1,
    X16 = 2
  };
  typedef enum MaxDataBusWidth MaxDataBusWidth;
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<MaxDataBusWidth max_data_bus_width>
  struct DeviceTypeTraits;
  
  template<>
  struct DeviceTypeTraits<X16>
  {
    // :NOTE: Wei 2004-May-04:
    //
    // This is the shift amount used to compute the 'real' byte address
    // (used by the CPU to access the flash memory) from the address
    // specified by CFI specification.
    //
    // Ex:
    //
    // in CFI, the data stored in address 0x10 will be 'Q'.
    // This is true for 8-bit flash chip. (8-bit means data bus width).
    // However, in a 16-bit flash chip, becuase of having to get a 16-bit data at once,
    // thus the value stored in address 0x11 have to be 0x00.
    //
    // However, CFI uses an alternative approach, CFI specify that 0x10 is an address of
    // 'maximum device bus-width'. That is to say, 0x10 is the 16th element of the flash
    // memory map, and each element has 'maximum device bus-width' length.
    //
    // Ex:
    //
    // In a 16-bit flash chip, an element will be 16-bit length (i.e. 2-byte),
    // thus 0x10 in CFI will means 0x20 in byte addressing.
    // (and this 0x20 indeed is the value that CPU should provided to access the data
    //  stored in CFI 0x10 address).
    //
    // I think CFI uses this approach is just becuase flash company can simplify the
    // hardware design.
    //
    // Ex:
    //
    // In a CPU with a 16-bit flash chip.
    // CPU can just ignore its A0 address line, and connect its A1 address line
    // to the A0 address line of the flash chip.
    // Thus, when CPU issue 0x...0 and 0x...1 address, the underlying flash chip will
    // give the same 16-bit value to the CPU.
    static uint32_t const SHIFT_AMOUNT = 1;
  };
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<uint32_t interleave_count>
  struct InterleaveTraits;
  
  template<>
  struct InterleaveTraits<2>
  {
    static uint32_t const VALUE = 2; // means 2 flash chips
    
    // :NOTE: Wei 2004-May-04:
    //
    // See comments about DeviceTypeTraits::SHIFT_AMOUNT first.
    //
    // Interleave will increse the data bus width.
    //
    // Ex:
    //
    // 1 CPU with 2 16-bit flash chips to form a 32-bit data bus,
    // then the CPU has to get a 32-bit data at once.
    // Thus the CPU can ingore its A0 & A1 address line, and connect its
    // A2 address line to the A0 address line of the flash chip.
    //
    // Thus, when CPU issues 0x...00, 0x...01, 0x...10, 0x...11,
    // the underlying flash chip will give it the same 32-bit data.
    static uint32_t const SHIFT_AMOUNT = 1;
  };
    
  /////////////////////////////////////////////////////////////////////////////
  
  // :NOTE: Wei 2004-May-04:
  //
  // If I want to support a new machine type, say COMPAQ_IPAQ_H3900,
  // then I just need to explicitly specialization the following template.
  //
  // i.e.
  // template<>
  // struct MachineType<COMPAQ_IPAQ_H3900>
  // {
  //   ...
  // };
  //
  // see template<> struct MachineType<COMPAQ_IPAQ_H3600> as an example.
  template<MachineTypeEnum machine_type>
  struct MachineTypeTraits;
  
  template<>
  struct MachineTypeTraits<COMPAQ_IPAQ_H3600>
  {
    typedef uint32_t DataBusWidth;
    typedef FlashInfo<INTEL_STRATA_J3> FlashInfo;
    
    typedef struct InterleaveTraits<2> Interleave;
    typedef struct DeviceTypeTraits<X16> DeviceType;
    
    static uint32_t const FLASH_SIZE = 16 * 1024 * 1024; // 16 MByte
    
    // :NOTE: Wei 2004-May-04:
    //
    // Because H3600 uses 2 16-bit data bus flash chips to form a 32-bit data bus flash,
    // thus we have to issue commands to both of them.
    //
    // This is why linux kernel will issue command 0x00980098 (note: this is a 32-bit data)
    // to the data bus (data bus is also 32-bit width) on H3600.
    //
    // However, in ARMware, I just need to know what the command is, thus I have to preform
    // a bit-field and operation to extract the real command from the value provided by
    // linux kernel.
    static uint32_t const BANK_BITMASK = 0xFFFF;
    
    // Write buffer
    
    static uint32_t const WRITE_BUFFER_SIZE = FlashInfo::WRITE_BUFFER_SIZE * Interleave::VALUE; // in bytes
    
    // :NOTE: Wei 2004-May-04:
    //
    // Because H3600 uses 2 16-bit data bus flash chips to form a 32-bit data bus flash,
    // thus the A2 address line of the CPU have to connect to the A0 address line of the flash.
    // So, the lowest 2 bit of addresses provided by the CPU is useless.
    static uint32_t const INVALID_ADDR_BITMASK = 0x3;
    
    // Block
    
#define BuildCFIFlashAddress(x) ((x) >> DeviceType::SHIFT_AMOUNT >> Interleave::SHIFT_AMOUNT)
    
    enum
    {
      // :NOTE: Wei 2004-May-25:
      //
      // Each flash chip has 128-kb size erase block.
      // H3600 has 2 chips in parallel.
      // Thus H3600's erase block size = 256-kb.
      //
      // 0x00000000
      // 0x00040000
      // ...
      
      BLOCK_0_BEGIN_LOC  = BuildCFIFlashAddress(0x00000000),
      BLOCK_1_BEGIN_LOC  = BuildCFIFlashAddress(0x00040000),
      BLOCK_2_BEGIN_LOC  = BuildCFIFlashAddress(0x00080000),
      BLOCK_3_BEGIN_LOC  = BuildCFIFlashAddress(0x000c0000),
      BLOCK_4_BEGIN_LOC  = BuildCFIFlashAddress(0x00100000),
      BLOCK_5_BEGIN_LOC  = BuildCFIFlashAddress(0x00140000),
      BLOCK_6_BEGIN_LOC  = BuildCFIFlashAddress(0x00180000),
      BLOCK_7_BEGIN_LOC  = BuildCFIFlashAddress(0x001c0000),
      BLOCK_8_BEGIN_LOC  = BuildCFIFlashAddress(0x00200000),
      BLOCK_9_BEGIN_LOC  = BuildCFIFlashAddress(0x00240000),
      BLOCK_10_BEGIN_LOC = BuildCFIFlashAddress(0x00280000),
      BLOCK_11_BEGIN_LOC = BuildCFIFlashAddress(0x002c0000),
      BLOCK_12_BEGIN_LOC = BuildCFIFlashAddress(0x00300000),
      BLOCK_13_BEGIN_LOC = BuildCFIFlashAddress(0x00340000),
      BLOCK_14_BEGIN_LOC = BuildCFIFlashAddress(0x00380000),
      BLOCK_15_BEGIN_LOC = BuildCFIFlashAddress(0x003c0000),
      BLOCK_16_BEGIN_LOC = BuildCFIFlashAddress(0x00400000),
      BLOCK_17_BEGIN_LOC = BuildCFIFlashAddress(0x00440000),
      BLOCK_18_BEGIN_LOC = BuildCFIFlashAddress(0x00480000),
      BLOCK_19_BEGIN_LOC = BuildCFIFlashAddress(0x004c0000),
      BLOCK_20_BEGIN_LOC = BuildCFIFlashAddress(0x00500000),
      BLOCK_21_BEGIN_LOC = BuildCFIFlashAddress(0x00540000),
      BLOCK_22_BEGIN_LOC = BuildCFIFlashAddress(0x00580000),
      BLOCK_23_BEGIN_LOC = BuildCFIFlashAddress(0x005c0000),
      BLOCK_24_BEGIN_LOC = BuildCFIFlashAddress(0x00600000),
      BLOCK_25_BEGIN_LOC = BuildCFIFlashAddress(0x00640000),
      BLOCK_26_BEGIN_LOC = BuildCFIFlashAddress(0x00680000),
      BLOCK_27_BEGIN_LOC = BuildCFIFlashAddress(0x006c0000),
      BLOCK_28_BEGIN_LOC = BuildCFIFlashAddress(0x00700000),
      BLOCK_29_BEGIN_LOC = BuildCFIFlashAddress(0x00740000),
      BLOCK_30_BEGIN_LOC = BuildCFIFlashAddress(0x00780000),
      BLOCK_31_BEGIN_LOC = BuildCFIFlashAddress(0x007c0000),
      BLOCK_32_BEGIN_LOC = BuildCFIFlashAddress(0x00800000),
      BLOCK_33_BEGIN_LOC = BuildCFIFlashAddress(0x00840000),
      BLOCK_34_BEGIN_LOC = BuildCFIFlashAddress(0x00880000),
      BLOCK_35_BEGIN_LOC = BuildCFIFlashAddress(0x008c0000),
      BLOCK_36_BEGIN_LOC = BuildCFIFlashAddress(0x00900000),
      BLOCK_37_BEGIN_LOC = BuildCFIFlashAddress(0x00940000),
      BLOCK_38_BEGIN_LOC = BuildCFIFlashAddress(0x00980000),
      BLOCK_39_BEGIN_LOC = BuildCFIFlashAddress(0x009c0000),
      BLOCK_40_BEGIN_LOC = BuildCFIFlashAddress(0x00a00000),
      BLOCK_41_BEGIN_LOC = BuildCFIFlashAddress(0x00a40000),
      BLOCK_42_BEGIN_LOC = BuildCFIFlashAddress(0x00a80000),
      BLOCK_43_BEGIN_LOC = BuildCFIFlashAddress(0x00ac0000),
      BLOCK_44_BEGIN_LOC = BuildCFIFlashAddress(0x00b00000),
      BLOCK_45_BEGIN_LOC = BuildCFIFlashAddress(0x00b40000),
      BLOCK_46_BEGIN_LOC = BuildCFIFlashAddress(0x00b80000),
      BLOCK_47_BEGIN_LOC = BuildCFIFlashAddress(0x00bc0000),
      BLOCK_48_BEGIN_LOC = BuildCFIFlashAddress(0x00c00000),
      BLOCK_49_BEGIN_LOC = BuildCFIFlashAddress(0x00c40000),
      BLOCK_50_BEGIN_LOC = BuildCFIFlashAddress(0x00c80000),
      BLOCK_51_BEGIN_LOC = BuildCFIFlashAddress(0x00cc0000),
      BLOCK_52_BEGIN_LOC = BuildCFIFlashAddress(0x00d00000),
      BLOCK_53_BEGIN_LOC = BuildCFIFlashAddress(0x00d40000),
      BLOCK_54_BEGIN_LOC = BuildCFIFlashAddress(0x00d80000),
      BLOCK_55_BEGIN_LOC = BuildCFIFlashAddress(0x00dc0000),
      BLOCK_56_BEGIN_LOC = BuildCFIFlashAddress(0x00e00000),
      BLOCK_57_BEGIN_LOC = BuildCFIFlashAddress(0x00e40000),
      BLOCK_58_BEGIN_LOC = BuildCFIFlashAddress(0x00e80000),
      BLOCK_59_BEGIN_LOC = BuildCFIFlashAddress(0x00ec0000),
      BLOCK_60_BEGIN_LOC = BuildCFIFlashAddress(0x00f00000),
      BLOCK_61_BEGIN_LOC = BuildCFIFlashAddress(0x00f40000),
      BLOCK_62_BEGIN_LOC = BuildCFIFlashAddress(0x00f80000),
      BLOCK_63_BEGIN_LOC = BuildCFIFlashAddress(0x00fc0000) // 16 Mbyte
    };
    
    static uint32_t const BLOCK_COUNT = 64;
    static uint32_t const BLOCK_SIZE = 256 * 1024; // 256 kbytes
    static uint32_t const BLOCK_ADDR_BITMASK = 0x00FC0000;
    
    static inline uint32_t
    BlockAddrToIndex(uint32_t const address)
    {
      return (address >> 18);
    }
    
    // CFI
    
    static inline DataBusWidth
    BuildCFICmd(uint8_t const value)
    {
      return ((value << 16) | value);
    }
    
    // :NOTE: Wei 2004-May-04:
    //
    // Becuase in the C++ standard now, there is no function calls to be considered as a constant expression,
    // thus if I use the following 'BuildCFICpuAddress()' function in the switch-case label,
    // the C++ compiler will emit an error.
    // As a result, I use BuildCFICpuAddress 'macro' rather than a function.
    //
    // After seeking solutions using Google, I found that some people also notice this problem,
    // and they have already present a proposal to the standard committee.
    // However, I don't know when the committee will accept this cool feature,
    // and I don't know when GCC will implement this feature too.
    //
    // Thus, I choose to implement BuildCFICpuAddress as a macro rather than a function.
    
    // inline uint32_t
    // BuildCFICpuAddress(uint32_t const cmd_offset)
    // {
    //   return (cmd_offset << DeviceType::SHIFT_AMOUNT << Interleave::SHIFT_AMOUNT);
    // }
#define BuildCFICpuAddress(x) ((x) << MachineType::DeviceType::SHIFT_AMOUNT << MachineType::Interleave::SHIFT_AMOUNT)
    
    // Dynamic ram
    
    static uint32_t const DRAM_BASE = 0xC0000000;
    static uint32_t const DRAM_SIZE = 0x1FFFFFF; // 32 MBytes
  };
  
#if CONFIG_MACH_H3600
  typedef MachineTypeTraits<COMPAQ_IPAQ_H3600> MachineType;
#endif
}

#endif
