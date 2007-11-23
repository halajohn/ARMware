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

#ifndef ARMware_hpp
#define ARMware_hpp

#include <cassert>
#include <string>
#include <vector>

#include "Type.hpp"
#include "platform_dep.hpp"

#if PRINT_CHUNK_CODE
#include "Inst/Inst.hpp"
#endif

namespace ARMware
{
  // Forward declaration
  //
  
  union Inst;
  
  // :NOTE: Wei 2004-Jan-20:
  //
  // Because the following symbolic symbols are used in a switch-case structure,
  // thus I shift them left 28 bits in compilation time rather than
  // shift the switch statement right 28 bits in run time.
  enum InstCond
  {
    EQ = static_cast<uint32_t>(( 0 << 28)),
    NE = static_cast<uint32_t>(( 1 << 28)),
    CS = static_cast<uint32_t>(( 2 << 28)),
    CC = static_cast<uint32_t>(( 3 << 28)),
    MI = static_cast<uint32_t>(( 4 << 28)),
    PL = static_cast<uint32_t>(( 5 << 28)),
    VS = static_cast<uint32_t>(( 6 << 28)),
    VC = static_cast<uint32_t>(( 7 << 28)),
    HI = static_cast<uint32_t>(( 8 << 28)),
    LS = static_cast<uint32_t>(( 9 << 28)),
    GE = static_cast<uint32_t>((10 << 28)),
    LT = static_cast<uint32_t>((11 << 28)),
    GT = static_cast<uint32_t>((12 << 28)),
    LE = static_cast<uint32_t>((13 << 28)),
    AL = static_cast<uint32_t>((14 << 28)),
    NV = static_cast<uint32_t>((15 << 28))
#if ENABLE_DYNAMIC_TRANSLATOR
    , NONE
#endif
  };
  typedef enum InstCond InstCond;
  
  enum StatusBitPos
  {
    NBIT_POS = 31,
    ZBIT_POS = 30,
    CBIT_POS = 29,
    VBIT_POS = 28
  };
  typedef enum StatusBitPos StatusBitPos;
  
  enum ExecResultEnum
  {
    ER_NORMAL,
    ER_EXCEPTION
#if ENABLE_THREADED_CODE
    , ER_NORMAL_CONT_EXEC_IN_CHUNK
    , ER_NORMAL_MODIFY_PC
    , ER_IO_IRQ_OCCUR
    , ER_IO_FIQ_OCCUR
    , ER_CHUNK_DISAPPEAR
#endif
  };
  typedef enum ExecResultEnum ExecResultEnum;
  
  enum PassCondResultEnum
  {
    PASS_TRUE,
    PASS_FALSE
#if CONFIG_ARCH_V5
    , PASS_EXCEPTION
#endif
  };
  typedef enum PassCondResultEnum PassCondResultEnum;
  
  enum UsageEnum
  {
    USAGE_NORMAL
#if ENABLE_THREADED_CODE
    , USAGE_THREADED_CODE
#endif
  };
  typedef enum UsageEnum UsageEnum;
  
  enum CoreMode
  {
    MODE_USR = 0x10, // 0b10000
    MODE_FIQ = 0x11, // 0b10001
    MODE_IRQ = 0x12, // 0b10010
    MODE_SVC = 0x13, // 0b10011
    MODE_ABT = 0x17, // 0b10111
    MODE_UND = 0x1B, // 0b11011
    MODE_SYS = 0x1F  // 0b11111
  };
  typedef enum CoreMode CoreMode;
  
  enum CoreRegNum
  {
    // :NOTE: Wei 2004-Feb-09:
    //
    // CR_0 have to be started from 0.
    // Because this behavior affects other codes.
    //
    // Ex: In 'bool LSMRegEnable(Inst const inst, CoreRegNum regNum)'
    //     I use static_cast<uint32_t>(regNum) to get the bit offset of the corresponding
    //     register.
    CR_BEGIN = 0,
    CR_0 = CR_BEGIN,
    CR_1,
    CR_2,
    CR_3,
    CR_4,
    CR_5,
    CR_6,
    CR_7,
    CR_8,
    CR_9,
    CR_10,
    CR_11,
    CR_12,
    CR_13,
    CR_14,
    CR_15,
    CR_END,
    
    CR_SP = CR_13, // Stack pointer
    CR_LR = CR_14, // Link reguster
    CR_PC = CR_15  // Program counter
  };
  typedef enum CoreRegNum CoreRegNum;
  
  inline CoreRegNum &
  operator++(CoreRegNum &regNum) // prefix
  {
    return regNum = static_cast<CoreRegNum>(regNum + 1);
  }
  
  inline CoreRegNum &
  operator--(CoreRegNum &regNum) // prefix
  {
    return regNum = static_cast<CoreRegNum>(regNum - 1);
  }
  
#if ENABLE_INST_RECALL || (ENABLE_DYNAMIC_TRANSLATOR && PRINT_CHUNK_CODE)
  inline char const *
  CoreRegName(CoreRegNum const i)
  {
    switch (i)
    {
    case CR_0: return "R0";
    case CR_1: return "R1";
    case CR_2: return "R2";
    case CR_3: return "R3";
    case CR_4: return "R4";
    case CR_5: return "R5";
    case CR_6: return "R6";
    case CR_7: return "R7";
    case CR_8: return "R8";
    case CR_9: return "R9";
    case CR_10: return "R10";
    case CR_11: return "R11";
    case CR_12: return "R12";
    case CR_13: return "R13";
    case CR_14: return "R14";
    case CR_15: return "R15";
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
#endif
  
#if PRINT_CHUNK_CODE
  inline char const *
  CondName(InstCond const cond)
  {
    switch (cond)
    {
    case EQ: return "EQ";
    case NE: return "NE";
    case CS: return "CS";
    case CC: return "CC";
    case MI: return "MI";
    case PL: return "PL";
    case VS: return "VS";
    case VC: return "VC";
    case HI: return "HI";
    case LS: return "LS";
    case GE: return "GE";
    case LT: return "LT";
    case GT: return "GT";
    case LE: return "LE";
      
      // :ARM-DDI-0100E: 2004-Sep-02:
      //
      // If <cond> is omitted, the AL (always) condition is used.
    case AL: return "";
      
    case NV: return "NV";
    case NONE: return "NONE";
      
    default:
      assert(!"Should not reach here.");
      return 0;
    }
  }
  
  inline char const *
  CondName(Inst const inst)
  {
    return CondName(static_cast<InstCond>(inst.mRawData & 0xF0000000));
  }
#endif
  
  enum CoProcNum
  {
    CP_0  = 0,
    CP_1  = 1,
    CP_2  = 2,
    CP_3  = 3,
    CP_4  = 4,
    CP_5  = 5,
    CP_6  = 6,
    CP_7  = 7,
    CP_8  = 8,
    CP_9  = 9,
    CP_10 = 10,
    CP_11 = 11,
    CP_12 = 12,
    CP_13 = 13,
    CP_14 = 14,
    CP_15 = 15
  };
  typedef enum CoProcNum CoProcNum;
  
  enum CoProcRegNum
  {
    CPR_0  = 0,
    CPR_1  = 1,
    CPR_2  = 2,
    CPR_3  = 3,
    CPR_4  = 4,
    CPR_5  = 5,
    CPR_6  = 6,
    CPR_7  = 7,
    CPR_8  = 8,
    CPR_9  = 9,
    CPR_10 = 10,
    CPR_11 = 11,
    CPR_12 = 12,
    CPR_13 = 13,
    CPR_14 = 14,
    CPR_15 = 15
  };
  typedef enum CoProcRegNum CoProcRegNum;
  
  enum SftType
  {
    LSL, // Logical Shift Left
    LSR, // Logical Shift Right
    ASR, // Arthimetical Shift Right
    ROR, // Rotate Right
    RRX  // Rotate Right with Extend
  };
  typedef enum SftType SftType;
  
  inline std::string const
  SftTypeName(SftType const type)
  {
    switch (type)
    {
    case LSL: return "LSL";
    case LSR: return "LSR";
    case ASR: return "ASR";
    case ROR: return "ROR";
    case RRX: return "RRX";
      
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  enum AccessTypeEnum
  {
    INST,
    DATA
  };
  typedef enum AccessTypeEnum AccessTypeEnum;
  
  void DiagnosticErrorWithInst(Inst const &inst,
                               char const * const filename,
                               int const line) NORETURN;
  
  /////////////////////////////////////////////////////////////////////////////
  
  enum SizeAbbr
  {
    SIZE_1K = 1 * 1024,
    SIZE_4K = 4 * 1024
  };
  typedef enum SizeAbbr SizeAbbr;
  
  /////////////////////////////////////////////////////////////////////////////
  // Data Width
  
  enum DataWidthEnum
  {
    WORD,
    HALF_WORD,
    BYTE
  };
  typedef enum DataWidthEnum DataWidthEnum;
  
  template<DataWidthEnum dataWidthEnum>
  struct AlignNeeds;
  
  template<>
  struct AlignNeeds<WORD>
  {
    static uint32_t const IGNORE_BITMASK = 0x3; // 0b11
  };
  
  template<>
  struct AlignNeeds<HALF_WORD>
  {
    static uint32_t const IGNORE_BITMASK = 0x1; // 0b01
  };
  
  template<>
  struct AlignNeeds<BYTE>
  {
    static uint32_t const IGNORE_BITMASK = 0x0; // 0b00
  };
  
  template<DataWidthEnum dataWidthEnum>
  class DataWidthTraits;
  
  template<>
  struct DataWidthTraits<BYTE>
  {
    typedef uint8_t UnsignedType;
    typedef int8_t  SignedType;
    
    typedef AlignNeeds<BYTE> AlignNeeds;
    
    static std::string const DESC_STRING;
  };
  
  template<>
  struct DataWidthTraits<HALF_WORD>
  {
    typedef uint16_t UnsignedType;
    typedef int16_t  SignedType;
    
    typedef AlignNeeds<HALF_WORD> AlignNeeds;
    
    static std::string const DESC_STRING;
  };
  
  template<>
  struct DataWidthTraits<WORD>
  {
    typedef uint32_t UnsignedType;
    typedef int32_t  SignedType;
    
    typedef AlignNeeds<WORD> AlignNeeds;
    
    static std::string const DESC_STRING;
  };
  
  /////////////////////////////////////////////////////////////////////////////
  
  enum RWType
  {
    READ,
    WRITE
  };
  typedef enum RWType RWType;
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<typename T_type>
  std::string const type_name();
  
  template<>
  inline std::string const
  type_name<uint32_t>()
  { return "uint32_t"; }
  
  template<>
  inline std::string const
  type_name<uint16_t>()
  { return "uint16_t"; }
  
  template<>
  inline std::string const
  type_name<uint8_t>()
  { return "uint8_t"; }
  
#if ENABLE_THREADED_CODE
  uint32_t const NATIVE_PAGE_SIZE = 4096;
  
  inline uint32_t
  page_aligned_size(uint32_t const size)
  {
    return (size + (NATIVE_PAGE_SIZE - 1)) & ~(NATIVE_PAGE_SIZE - 1);
  }
  
  inline void *
  page_aligned_addr(void * const address)
  {
    return reinterpret_cast<void *>(reinterpret_cast<uint32_t>(address) & ~(NATIVE_PAGE_SIZE - 1));
  }
#endif
  
  /////////////////////////////////////////////////////////////////////////////
  
  template<typename T>
  inline void
  add_to_vector_without_duplicate(std::vector<T *> &vect, T * const t)
  {
    for (typename std::template vector<T *>::iterator iter = vect.begin();
         iter != vect.end();
         ++iter)
    {
      if ((*iter) == t)
      {
        return;
      }
    }
    
    vect.push_back(t);
  }
  
  template<typename T>
  struct del_fun_t
  {
    inline void
    operator()(T* p)
    {
      delete p;
    }
  };
  
  template<typename T>
  inline del_fun_t<T>
  del_fun()
  {
    return del_fun_t<T>();
  }
} // namespace ARMware

#endif // ARMware_hpp
