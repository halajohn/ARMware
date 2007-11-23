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

#ifndef Inst_hpp
#define Inst_hpp

#include <string>

#if ENABLE_INST_RECALL
#include <cassert>
#endif

#include "../Type.hpp"

namespace ARMware
{
  // Forward declaration
  //
  
  class Core;
  
  enum DPIAddressingModeEnum
  {
    IMM,
    IMM_SFT,
    REG_SFT
  };
  typedef enum DPIAddressingModeEnum DPIAddressingModeEnum;
  
  typedef struct ImmDPI ImmDPI;
  struct ImmDPI
  {
    static DPIAddressingModeEnum const ADDR_MODE = IMM;
    
    uint32_t mImm    : 8;
    uint32_t mRotImm : 4;
    uint32_t mRd     : 4;
    uint32_t mRn     : 4;
    uint32_t mSbit   : 1;
    uint32_t mOpcode : 4;
    uint32_t mIbit   : 1; // 1
    uint32_t mPad    : 2; // 00
    uint32_t mCond   : 4;
  };
  
  typedef struct RegImmSftDPI RegImmSftDPI;
  struct RegImmSftDPI
  {
    static DPIAddressingModeEnum const ADDR_MODE = IMM_SFT;
    
    uint32_t mRm      : 4;
    uint32_t mPad2    : 1; // 0
    uint32_t mSftType : 2;
    uint32_t mSftImm  : 5;
    uint32_t mRd      : 4;
    uint32_t mRn      : 4;
    uint32_t mSbit    : 1;
    uint32_t mOpcode  : 4;
    uint32_t mIbit    : 1; // 0
    uint32_t mPad1    : 2; // 00
    uint32_t mCond    : 4;
  };
  
  typedef struct RegRegSftDPI RegRegSftDPI;
  struct RegRegSftDPI
  {
    static DPIAddressingModeEnum const ADDR_MODE = REG_SFT;
    
    uint32_t mRm      : 4;
    uint32_t mPad3    : 1; // 1
    uint32_t mSftType : 2;
    uint32_t mPad2    : 1; // 0
    uint32_t mRs      : 4;
    uint32_t mRd      : 4;
    uint32_t mRn      : 4;
    uint32_t mSbit    : 1;
    uint32_t mOpcode  : 4;
    uint32_t mIbit    : 1; // 0
    uint32_t mPad1    : 2; // 00
    uint32_t mCond    : 4;
  };
  
  typedef struct ImmLSW ImmLSW;
  struct ImmLSW
  {
    uint32_t mOff  : 12;
    uint32_t mRd   : 4;
    uint32_t mRn   : 4;
    uint32_t mLbit : 1;
    uint32_t mWbit : 1;
    uint32_t mBbit : 1;
    uint32_t mUbit : 1;
    uint32_t mPbit : 1;
    uint32_t mIbit : 1; // 0
    uint32_t mPad1 : 2; // 01
    uint32_t mCond : 4;
  };
  
  typedef struct RegLSW RegLSW;
  struct RegLSW
  {
    uint32_t mRm      : 4;
    uint32_t mPad2    : 1; // 0
    uint32_t mSftType : 2;
    uint32_t mSftImm  : 5;
    uint32_t mRd      : 4;
    uint32_t mRn      : 4;
    uint32_t mLbit    : 1;
    uint32_t mWbit    : 1;
    uint32_t mBbit    : 1;
    uint32_t mUbit    : 1;
    uint32_t mPbit    : 1;
    uint32_t mIbit    : 1; // 1
    uint32_t mPad1    : 2; // 01
    uint32_t mCond    : 4;
  };
  
  typedef struct ImmLSH ImmLSH;
  struct ImmLSH
  {
    uint32_t mImmL : 4;
    uint32_t mPad4 : 1; // 1
    uint32_t mHbit : 1;
    uint32_t mSbit : 1;
    uint32_t mPad3 : 1; // 1
    uint32_t mImmH : 4;
    uint32_t mRd   : 4;
    uint32_t mRn   : 4;
    uint32_t mLbit : 1;
    uint32_t mWbit : 1;
    uint32_t mPad2 : 1; // 1
    uint32_t mUbit : 1;
    uint32_t mPbit : 1;
    uint32_t mPad1 : 3; // 000
    uint32_t mCond : 4;
  };
  
  typedef struct RegLSH RegLSH;
  struct RegLSH
  {
    uint32_t mRm   : 4;
    uint32_t mPad5 : 1; // 1
    uint32_t mHbit : 1;
    uint32_t mSbit : 1;
    uint32_t mPad4 : 1; // 1
    uint32_t mPad3 : 4; // 0000
    uint32_t mRd   : 4;
    uint32_t mRn   : 4;
    uint32_t mLbit : 1;
    uint32_t mWbit : 1;
    uint32_t mPad2 : 1; // 0
    uint32_t mUbit : 1;
    uint32_t mPbit : 1;
    uint32_t mPad1 : 3; // 000
    uint32_t mCond : 4;
  };
  
  typedef struct Multi Multi;
  struct Multi
  {
    uint32_t mRm     : 4;
    uint32_t mPad2   : 4; // 1001
    uint32_t mRs     : 4;
    uint32_t mRn     : 4;
    uint32_t mRd     : 4;
    uint32_t mSbit   : 1;
    uint32_t mOpcode : 3;
    uint32_t mPad1   : 4; // 0000
    uint32_t mCond   : 4;
  };
  
  typedef struct LSM LSM;
  struct LSM
  {
    uint32_t mRegList : 16;
    uint32_t mRn      : 4;
    uint32_t mLbit    : 1;
    uint32_t mWbit    : 1;
    uint32_t mSbit    : 1;
    uint32_t mUbit    : 1;
    uint32_t mPbit    : 1;
    uint32_t mPad     : 3; // 100
    uint32_t mCond    : 4;
  };
  
  typedef struct ImmMSR ImmMSR;
  struct ImmMSR
  {
    uint32_t mImm    : 8;
    uint32_t mRotImm : 4;
    uint32_t mPad3   : 4; // 1111
    uint32_t mField  : 4;
    uint32_t mPad2   : 2; // 10
    uint32_t mRbit   : 1;
    uint32_t mPad1   : 5; // 00110
    uint32_t mCond   : 4;
  };
  
  typedef struct RegMSR RegMSR;
  struct RegMSR
  {
    uint32_t mRm    : 4;
    uint32_t mPad5  : 4; // 0000
    uint32_t mPad4  : 4; // 0000
    uint32_t mPad3  : 4; // 1111
    uint32_t mField : 4;
    uint32_t mPad2  : 2; // 10
    uint32_t mRbit  : 1;
    uint32_t mPad1  : 5; // 00010
    uint32_t mCond  : 4;
  };
  
  typedef struct MRS MRS;
  struct MRS
  {
    uint32_t mPad4 : 12; // 0000 0000 0000
    uint32_t mRd   : 4;
    uint32_t mPad3 : 4; // 1111
    uint32_t mPad2 : 2; // 00
    uint32_t mRbit : 1;
    uint32_t mPad1 : 5; // 00010
    uint32_t mCond : 4;
  };
  
  typedef struct SWP SWP;
  struct SWP
  {
    uint32_t mRm   : 4;
    uint32_t mPad3 : 4; // 1001
    uint32_t mPad2 : 4; // 0000
    uint32_t mRd   : 4;
    uint32_t mRn   : 4;
    uint32_t mPad1 : 8; // 0001 0000 or 0001 0101
    uint32_t mCond : 4;
  };
  
  typedef struct ImmBranch ImmBranch;
  struct ImmBranch
  {
    uint32_t mImm  : 24;
    uint32_t mLbit : 1;
    uint32_t mPad1 : 3; // 101
    uint32_t mCond : 4;
  };
  
  typedef struct RegBranch RegBranch;
  struct RegBranch
  {
    uint32_t mRm   : 4;
    uint32_t mPad3 : 4; // 0001
    uint32_t mPad2 : 12; // 1111 1111 1111
    uint32_t mPad1 : 8; // 0001 0010
    uint32_t mCond : 4;
  };
  
  // MRC & MCR
  typedef struct CRT CRT;
  struct CRT
  {
    uint32_t mCRm     : 4;
    uint32_t mPad2    : 1; // 1
    uint32_t mOpcode2 : 3;
    uint32_t mCpNum   : 4;
    uint32_t mRd      : 4;
    uint32_t mCRn     : 4;
    uint32_t mLoad    : 1;
    uint32_t mOpcode1 : 3;
    uint32_t mPad1    : 4; // 1110
    uint32_t mCond    : 4;
  };
  
  typedef struct CDT CDT;
  struct CDT
  {
    uint32_t mOffset : 8;
    uint32_t mCpNum  : 4;
    uint32_t mCRd    : 4;
    uint32_t mRn     : 4;
    uint32_t mLbit   : 1;
    uint32_t mWbit   : 1;
    uint32_t mNbit   : 1;
    uint32_t mUbit   : 1;
    uint32_t mPbit   : 1;
    uint32_t mPad1   : 3; // 110
    uint32_t mCond   : 4;
  };
  
  typedef struct CDP CDP;
  struct CDP
  {
    uint32_t mCRm     : 4;
    uint32_t mPad2    : 1; // 0
    uint32_t mOpcode2 : 3;
    uint32_t mCpNum   : 4;
    uint32_t mCRd     : 4;
    uint32_t mCRn     : 4;
    uint32_t mOpcode1 : 4;
    uint32_t mPad1    : 4; // 1110
    uint32_t mCond    : 4;
  };
  
  typedef union Inst Inst;
  union Inst
  {
    uint32_t      mRawData;
    ImmDPI        mImmDPI;
    RegImmSftDPI  mRegImmSftDPI;
    RegRegSftDPI  mRegRegSftDPI;
    ImmLSW        mImmLSW;
    RegLSW        mRegLSW;
    ImmLSH        mImmLSH;
    RegLSH        mRegLSH;
    LSM           mLSM;
    Multi         mMulti;
    ImmMSR        mImmMSR;
    RegMSR        mRegMSR;
    MRS           mMRS;
    SWP           mSWP;
    ImmBranch     mImmBranch;
    RegBranch     mRegBranch;
    CRT           mCRT;
    CDT           mCDT;
    CDP           mCDP;
    
    Inst()
    { }
    
    Inst(uint32_t const inst)
      : mRawData(inst)
    { }
  };
  
  enum FaultStatus
  {
    FS_TERMINAL = 0x2, // 0b0010
    FS_VECTOR = 0, // 0b0000
    
    // :ARM-DDI-0100E: p.B3-19: Wei 2004-May-14:
    //
    // Alignment faults can write either 0b0001 or 0b0011 into FS[3:0].
    //
    // :NOTE: Wei 2004-May-14:
    //
    // I use 0b0001.
    FS_ALIGNMENT = 0x1, // 0b0001
    
    FS_EXTERNAL_TRANSLATION_1_LEVEL = 0xC, // 0b1100
    FS_EXTERNAL_TRANSLATION_2_LEVEL = 0xE, // 0b1110
    
    FS_TRANSLATION_SECTION = 0x5, // 0b0101
    FS_TRANSLATION_PAGE = 0x7, // 0b0111
    
    FS_DOMAIN_SECTION = 0x9, // 0b1001
    FS_DOMAIN_PAGE = 0xB, // 0b1011
    
    FS_PERMISSION_SECTION = 0xD, // 0b1101
    FS_PERMISSION_PAGE = 0xF, // 0b1111
    
    FS_EXTERNAL_LINEFATCH_SECTION = 0x4, // 0b0100
    FS_EXTERNAL_LINEFATCH_PAGE = 0x6, // 0b0110
    
    FS_EXTERNAL_NON_LINEFETCH_SECTION = 0x8, // 0b1000
    FS_EXTERNAL_NON_LINEFETCH_PAGE = 0xA // 0b1010
  };
  typedef enum FaultStatus FaultStatus;
  
  template<typename T>
  struct UpdateStatusCategory
  {
    struct UpdateStatusCategoryId {};
    
    virtual ~UpdateStatusCategory();
  };
  
  template<typename T>
  struct OperandNumberCategory
  {
    struct OperandNumberCategoryId {};
    
    virtual ~OperandNumberCategory();
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // UpdateStatusCategory
  
  struct OpcodeMayNotUpdateStatus
    : private UpdateStatusCategory<OpcodeMayNotUpdateStatus>
  {
    using UpdateStatusCategory<OpcodeMayNotUpdateStatus>::UpdateStatusCategoryId;
    
    virtual ~OpcodeMayNotUpdateStatus();
  };
  
  struct OpcodeHasToUpdateStatus
    : private UpdateStatusCategory<OpcodeHasToUpdateStatus>
  {
    using UpdateStatusCategory<OpcodeHasToUpdateStatus>::UpdateStatusCategoryId;
    
    virtual ~OpcodeHasToUpdateStatus();
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // OperandNumberCategory
  
  struct OpcodeNeedThreeOperand
    : private OperandNumberCategory<OpcodeNeedThreeOperand>
  {
    using OperandNumberCategory<OpcodeNeedThreeOperand>::OperandNumberCategoryId;
    
    virtual ~OpcodeNeedThreeOperand();
  };
  
  struct OpcodeNoNeedRd
    : private OperandNumberCategory<OpcodeNoNeedRd>
  {
    using OperandNumberCategory<OpcodeNoNeedRd>::OperandNumberCategoryId;
    
    virtual ~OpcodeNoNeedRd();
  };
  
  struct OpcodeNoNeedRn
    : private OperandNumberCategory<OpcodeNoNeedRn>
  {
    using OperandNumberCategory<OpcodeNoNeedRn>::OperandNumberCategoryId;
    
    virtual ~OpcodeNoNeedRn();
  };
  
  enum DPIOpcodeEnum
  {
    DPI_OP_AND,
    DPI_OP_EOR,
    DPI_OP_SUB,
    DPI_OP_RSB,
    DPI_OP_ADD,
    DPI_OP_ADC,
    DPI_OP_SBC,
    DPI_OP_RSC,
    DPI_OP_ORR,
    DPI_OP_BIC,
    DPI_OP_MOV,
    DPI_OP_MVN,
    DPI_OP_TST,
    DPI_OP_TEQ,
    DPI_OP_CMP,
    DPI_OP_CMN
  };
  typedef enum DPIOpcodeEnum DPIOpcodeEnum;
  
  struct AND : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_AND;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct EOR : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_EOR;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct SUB : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_SUB;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct RSB : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_RSB;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct ADD : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_ADD;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct ADC : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_ADC;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct SBC : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_SBC;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct RSC : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_RSC;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct ORR : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_ORR;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct BIC : public OpcodeMayNotUpdateStatus, public OpcodeNeedThreeOperand
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_BIC;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct MOV : public OpcodeMayNotUpdateStatus, public OpcodeNoNeedRn
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_MOV;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct MVN : public OpcodeMayNotUpdateStatus, public OpcodeNoNeedRn
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_MVN;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct TST : public OpcodeHasToUpdateStatus, public OpcodeNoNeedRd
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_TST;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct TEQ : public OpcodeHasToUpdateStatus, public OpcodeNoNeedRd
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_TEQ;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = true;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct CMP : public OpcodeHasToUpdateStatus, public OpcodeNoNeedRd
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_CMP;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  struct CMN : public OpcodeHasToUpdateStatus, public OpcodeNoNeedRd
  {
    static DPIOpcodeEnum const OPCODE = DPI_OP_CMN;
    static bool const SHIFTER_SHELL_PRODUCE_C_BIT = false;
    
#if PRINT_CHUNK_CODE
    static std::string const NAME;
#endif
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // Whether Update CPSR
  
  enum IsUpdateStatus
  {
    UPDATE_STATUS,
    NO_UPDATE_STATUS
  };
  typedef enum IsUpdateStatus IsUpdateStatus;
  
  enum UseWhichStatus
  {
    USE_CPSR,
    USE_CURRENT_SPSR
  };
  typedef enum UseWhichStatus UseWhichStatus;
  
  /////////////////////////////////////////////////////////////////////////////
  // Long multi or normal multi
  
  enum IsLong
  {
    NORMAL,
    LONG
  };
  typedef enum IsLong IsLong;
  
  /////////////////////////////////////////////////////////////////////////////
  // preform accumulation or not
  
  enum IsAccumulate
  {
    ACCUMULATE,
    NO_ACCUMULATE
  };
  typedef enum IsAccumulate IsAccumulate;
  
  enum IsSigned
  {
    SIGNED,
    UNSIGNED
  };
  typedef enum IsSigned IsSigned;
  
  /////////////////////////////////////////////////////////////////////////////
  // Used in LSW & LSH
  
  enum AddressingMode
  {
    OFFSET,
    PRE_IDX,
    POST_IDX
  };
  typedef enum AddressingMode AddressingMode;
  
  /////////////////////////////////////////////////////////////////////////////
  // Used in LSW
  
  enum DesiredMode
  {
    USER_MODE,
    CURRENT_MODE
  };
  typedef enum DesiredMode DesiredMode;
  
  enum DesiredBank
  {
    USER_BANK,
    CURRENT_BANK
  };
  typedef enum DesiredBank DesiredBank;
  
  enum IsLoad
  {
    LOAD,
    STORE
  };
  typedef enum IsLoad IsLoad;
  
  enum IsWriteBack
  {
    WRITE_BACK,
    NO_WRITE_BACK
  };
  typedef enum IsWriteBack IsWriteBack;
  
  enum IsLink
  {
    LINK,
    NO_LINK
  };
  typedef enum IsLink IsLink;
  
  enum ExchangeToThumb
  {
    EXCHANGE,
    NO_EXCHANGE
  };
  typedef enum ExchangeToThumb ExchangeToThumb;
  
  enum StartFrom
  {
    BEFORE,
    AFTER
  };
  typedef enum StartFrom StartFrom;
  
  /////////////////////////////////////////////////////////////////////////////
  // Used in LSW
  
  enum Direction
  {
    UP,
    DOWN
  };
  typedef enum Direction Direction;
  
  template<Direction direction>
  uint32_t Operation(uint32_t const src1, uint32_t const src2);
  
  template<>
  inline uint32_t
  Operation<UP>(uint32_t const src1, uint32_t const src2)
  {
    return src1 + src2;
  }
  
  template<>
  inline uint32_t
  Operation<DOWN>(uint32_t const src1, uint32_t const src2)
  {
    return src1 - src2;
  }
  
#if ENABLE_COND_CODES_ASM
  enum IA32Eflags
  {
    IA32_EFLAGS_CARRY = (1 << 0),
    IA32_EFLAGS_ZERO = (1 << 6),
    IA32_EFLAGS_SIGN = (1 << 7),
    IA32_EFLAGS_OVER = (1 << 11)
  };
  typedef enum IA32Eflags IA32Eflags;
#endif
  
#if ENABLE_INST_RECALL
  enum InstNameTag
  {
    // DPI
    INST_AND,
    INST_EOR,
    INST_SUB,
    INST_RSB,
    INST_ADD,
    INST_ADC,
    INST_SBC,
    INST_RSC,
    INST_TST,
    INST_TEQ,
    INST_CMP,
    INST_CMN,
    INST_ORR,
    INST_MOV,
    INST_BIC,
    INST_MVN,
    
    // Multi
    INST_SMULL,
    INST_SMLAL,
    INST_UMULL,
    INST_UMLAL,
    INST_MUL,
    INST_MLA,
    
    //
    INST_MRS,
    INST_MSR,
    
    // swap
    INST_SWP,
    
    // load
    INST_LDR,
    INST_LDRB,
    INST_LDRH,
    
    // store
    INST_STR,
    INST_STRB,
    INST_STRH,
    
    // load multi
    INST_LDM_1,
    INST_LDM_2,
    INST_LDM_3,
    
    // store multi
    INST_STM_1,
    INST_STM_2,
    
    // branch
    INST_BRANCH,
    
    // co-processor
    INST_MCR,
    INST_MRC
  };
  typedef enum InstNameTag InstNameTag;
  
  inline char const *
  InstName(InstNameTag const type)
  {
    switch (type)
    {
      // DPI
    case INST_AND: return "AND";
    case INST_EOR: return "EOR";
    case INST_SUB: return "SUB";
    case INST_RSB: return "RSB";
    case INST_ADD: return "ADD";
    case INST_ADC: return "ADC";
    case INST_SBC: return "SBC";
    case INST_RSC: return "RSC";
    case INST_TST: return "TST";
    case INST_TEQ: return "TEQ";
    case INST_CMP: return "CMP";
    case INST_CMN: return "CMN";
    case INST_ORR: return "ORR";
    case INST_MOV: return "MOV";
    case INST_BIC: return "BIC";
    case INST_MVN: return "MVN";
      
      // Multi
    case INST_SMULL: return "SMULL";
    case INST_SMLAL: return "SMLAL";
    case INST_UMULL: return "UMULL";
    case INST_UMLAL: return "UMLAL";
    case INST_MUL: return "MUL";
    case INST_MLA: return "MLA";
      
      //
    case INST_MRS: return "MRS";
    case INST_MSR: return "MSR";
      
      // swap
    case INST_SWP: return "SWP";
      
      // load
    case INST_LDR: return "LDR";
    case INST_LDRB: return "LDRB";
    case INST_LDRH: return "LDRH";
      
      // store
    case INST_STR: return "STR";
    case INST_STRB: return "STRB";
    case INST_STRH: return "STRH";
      
      // load multi
    case INST_LDM_1: return "LDM_1";
    case INST_LDM_2: return "LDM_2";
    case INST_LDM_3: return "LDM_3";
      
      // store multi
    case INST_STM_1: return "STM_1";
    case INST_STM_2: return "STM_2";
      
      // branch
    case INST_BRANCH: return "BRANCH";
      
      // co-processor
    case INST_MCR: return "MCR";
    case INST_MRC: return "MRC";
      
    default:
      assert(!"Should not reach here.");
      return "error";
    }
  }
  
  enum InstTypeTag
  {
    IMM,
    REG_IMM_SFT,
    REG_REG_SFT
  };
  typedef enum InstTypeTag InstTypeTag;
  
  inline char const *
  InstType(InstTypeTag const type)
  {
    switch (type)
    {
    case IMM: return "IMM";
    case REG_IMM_SFT: return "REG_IMM_SFT";
    case REG_REG_SFT: return "REG_REG_SFT";
      
    default:
      assert(!"Should not reach here.");
      return "error";
    }
  }
#endif
}

#endif
