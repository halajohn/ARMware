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

#ifndef Compiler_hpp
#define Compiler_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include <cassert>

#include "../ARMware.hpp"
#include "../Type.hpp"

namespace ARMware
{
  enum ConsiderMemoryVarEnum
  {
    CONSIDER_MEMORY_VAR,
    NO_CONSIDER_MEMORY_VAR,
  };
  typedef enum ConsiderMemoryVarEnum ConsiderMemoryVarEnum;
  
  enum VariableTypeEnum
  {
    NON_MEMORY_VAR,
    MEMORY_VAR
  };
  typedef enum VariableTypeEnum VariableTypeEnum;
  
  enum ValueNumberingTypeEnum
  {
    SCALAR_OPERATION_PERFORM_SEARCHING,
    SCALAR_OPERATION_NO_PERFORM_SEARCHING,
    
    MEMORY_OPERATION_FRAME,
    MEMORY_OPERATION_JSR,
    MEMORY_OPERATION_LOAD,
    MEMORY_OPERATION_STORE,
    
    NOT_INSIDE_VALUE_NUMBERING
  };
  typedef enum ValueNumberingTypeEnum ValueNumberingTypeEnum;
  
  enum MIRAppendType
  {
    MIR_FOR_COND_CODE,
    MIR_FOR_NORMAL
  };
  typedef enum MIRAppendType MIRAppendType;
  
  enum DefUseEnum
  {
    DEF,
    USE
  };
  typedef enum DefUseEnum DefUseEnum;
  
  enum IsSSAEnum
  {
    NOT_SSA_FORM,
    IS_SSA_FORM
  };
  typedef enum IsSSAEnum IsSSAEnum;
  
  // :NOTE: Wei 2004-Nov-12:
  //
  // Used for UMUL64, SMUL64, ADD64 MIR instructions.
  enum VarAttachedTypeEnum
  {
    VAR_ATTACH_HI,
    VAR_ATTACH_LO,
    VAR_ATTACH_NORMAL
  };
  typedef enum VarAttachedTypeEnum VarAttachedTypeEnum;
  
  enum FlagEnum
  {
    SIGN_FLAG = (1 << 0),
    ZERO_FLAG = (1 << 1),
    CARRY_FLAG = (1 << 2),
    OVERFLOW_FLAG = (1 << 3)
  };
  typedef enum FlagEnum FlagEnum;
  
#if CHECK_CHUNK_CODE || PRINT_CHUNK_CODE
  enum CheckAncestorNumberTiming
  {
    BEFORE_DEAD_CODE_ELIMINATION,
    AFTER_CONSTANT_FOLDING
  };
  typedef enum CheckAncestorNumberTiming CheckAncestorNumberTiming;
#endif
  
  // MIR instruction set
  //
  
  // :NOTE: Wei 2004-Oct-04:
  //
  // If I modify this enumeration, I have to also modify the following
  // KIND_NAME[] array as well.
  enum MIRKindEnum
  {
    MIR_CALL_0,
    MIR_CALL_1,
    MIR_RETURN,
    MIR_RETRIEVE,
    
    MIR_ADD,
    MIR_ADD64,
    MIR_ADC,
    MIR_MOV,
    MIR_AND,
    MIR_OR,
    MIR_EOR,
    MIR_SUB,
    MIR_SBC,
    MIR_TST,
    MIR_CMP,
    MIR_CMP_EQ,
    MIR_NOT,
    
    MIR_MUL32,
    MIR_UMUL64,
    MIR_SMUL64,
    
    MIR_LSFT_LEFT,
    MIR_LSFT_RIGHT,
    MIR_ASFT_RIGHT,
    MIR_RRX,
    MIR_ROR,
    
    MIR_GOTO,
    
    MIR_GOTO_EQ,
    MIR_GOTO_NE,
    MIR_GOTO_CS,
    MIR_GOTO_CC,
    MIR_GOTO_MI,
    MIR_GOTO_PL,
    MIR_GOTO_VS,
    MIR_GOTO_VC,
    MIR_GOTO_HI,
    MIR_GOTO_LS,
    MIR_GOTO_GE,
    MIR_GOTO_LT,
    MIR_GOTO_GT,
    MIR_GOTO_LE,
    
    MIR_JUMP,
    
    MIR_PRODUCE_CBIT,
    MIR_CLEAR_CBIT,
    
    MIR_DATA32,
    
    MIR_LOAD_LABEL,
    MIR_LOAD,
    
    MIR_KIND_TOTAL
  };
  typedef enum MIRKindEnum MIRKindEnum;
  
  enum DAGKindEnum
  {
    DAG_VAR_TEMP,
    DAG_VAR_GLOBAL,
    DAG_VAR_CONST,
    
    DAG_CALL,
    DAG_RETURN,
    DAG_RETRIEVE,
    DAG_ADD,
    DAG_ADD64,
    DAG_ADC,
    DAG_MOV,
    DAG_AND,
    DAG_OR,
    DAG_EOR,
    DAG_SUB,
    DAG_SBC,
    DAG_TST,
    DAG_CMP,
    DAG_CMP_EQ,
    DAG_NOT,
    DAG_MUL32,
    DAG_UMUL64,
    DAG_SMUL64,
    DAG_LSFT_LEFT,
    DAG_LSFT_RIGHT,
    DAG_ASFT_RIGHT,
    DAG_RRX,
    DAG_ROR,
    
    DAG_GOTO,
    
    DAG_GOTO_EQ,
    DAG_GOTO_NE,
    DAG_GOTO_CS,
    DAG_GOTO_CC,
    DAG_GOTO_MI,
    DAG_GOTO_PL,
    DAG_GOTO_VS,
    DAG_GOTO_VC,
    DAG_GOTO_HI,
    DAG_GOTO_LS,
    DAG_GOTO_GE,
    DAG_GOTO_LT,
    DAG_GOTO_GT,
    DAG_GOTO_LE,
    
    DAG_JUMP,
    DAG_PRODUCE_CBIT,
    DAG_SET_CBIT,
    DAG_CLEAR_CBIT,
    DAG_LOAD_LABEL,
    DAG_LOAD,
    
    DAG_KIND_TOTAL
  };
  typedef enum DAGKindEnum DAGKindEnum;
  
  struct DAGVar;
  
  struct DAGCall
  {
    static DAGKindEnum const KIND = DAG_CALL;
  };
  
  struct DAGReturn
  {
    static DAGKindEnum const KIND = DAG_RETURN;
  };

  struct DAGRetrieve
  {
    static DAGKindEnum const KIND = DAG_RETRIEVE;
  };

  struct DAGAdd
  {
    static DAGKindEnum const KIND = DAG_ADD;
    
    static inline uint32_t
    constant_folding_operation(uint32_t const v1, uint32_t const v2)
    {
      return v1 + v2;
    }
  };
  
  struct DAGAdd64
  {
    static DAGKindEnum const KIND = DAG_ADD64;
  };

  struct DAGAdc
  {
    static DAGKindEnum const KIND = DAG_ADC;
  };

  struct DAGMov
  {
    static DAGKindEnum const KIND = DAG_MOV;
  };

  struct DAGAnd
  {
    static DAGKindEnum const KIND = DAG_AND;
    
    static inline uint32_t
    constant_folding_operation(uint32_t const v1, uint32_t const v2)
    {
      return v1 & v2;
    }
  };
  
  struct DAGOr
  {
    static DAGKindEnum const KIND = DAG_OR;
    
    static inline uint32_t
    constant_folding_operation(uint32_t const v1, uint32_t const v2)
    {
      return v1 | v2;
    }
  };
  
  struct DAGEor
  {
    static DAGKindEnum const KIND = DAG_EOR;
    
    static inline uint32_t
    constant_folding_operation(uint32_t const v1, uint32_t const v2)
    {
      return v1 ^ v2;
    }
  };
  
  struct DAGSub
  {
    static DAGKindEnum const KIND = DAG_SUB;
    
    static inline uint32_t
    constant_folding_operation(uint32_t const v1, uint32_t const v2)
    {
      return v1 - v2;
    }
  };
  
  struct DAGSbc
  {
    static DAGKindEnum const KIND = DAG_SBC;
  };

  struct DAGTst
  {
    static DAGKindEnum const KIND = DAG_TST;
  };

  struct DAGCmp
  {
    static DAGKindEnum const KIND = DAG_CMP;
  };

  struct DAGCmpEQ
  {
    static DAGKindEnum const KIND = DAG_CMP_EQ;
  };

  struct DAGNot
  {
    static DAGKindEnum const KIND = DAG_NOT;
  };

  struct DAGMul32
  {
    static DAGKindEnum const KIND = DAG_MUL32;
  };

  struct DAGUMul64
  {
    static DAGKindEnum const KIND = DAG_UMUL64;
  };

  struct DAGSMul64
  {
    static DAGKindEnum const KIND = DAG_SMUL64;
  };
  
  struct DAGLSftLeft
  {
    static DAGKindEnum const KIND = DAG_LSFT_LEFT;
    
    static inline uint32_t
    constant_folding_operation(uint32_t const v1, uint32_t const v2)
    {
      assert(v2 < 32);
      
      return (v1 << v2);
    }
  };

  struct DAGLSftRight
  {
    static DAGKindEnum const KIND = DAG_LSFT_RIGHT;
  };

  struct DAGASftRight
  {
    static DAGKindEnum const KIND = DAG_ASFT_RIGHT;
  };

  struct DAGRrx
  {
    static DAGKindEnum const KIND = DAG_RRX;
  };

  struct DAGRor
  {
    static DAGKindEnum const KIND = DAG_ROR;
  };

  struct DAGGoto
  {
    static DAGKindEnum const KIND = DAG_GOTO;
  };
  
  struct DAGGotoEQ
  {
    static DAGKindEnum const KIND = DAG_GOTO_EQ;
  };
  
  struct DAGGotoNE
  {
    static DAGKindEnum const KIND = DAG_GOTO_NE;
  };
  
  struct DAGGotoCS
  {
    static DAGKindEnum const KIND = DAG_GOTO_CS;
  };
  
  struct DAGGotoCC
  {
    static DAGKindEnum const KIND = DAG_GOTO_CC;
  };
  
  struct DAGGotoMI
  {
    static DAGKindEnum const KIND = DAG_GOTO_MI;
  };
  
  struct DAGGotoPL
  {
    static DAGKindEnum const KIND = DAG_GOTO_PL;
  };
  
  struct DAGGotoVS
  {
    static DAGKindEnum const KIND = DAG_GOTO_VS;
  };
  
  struct DAGGotoVC
  {
    static DAGKindEnum const KIND = DAG_GOTO_VC;
  };
  
  struct DAGGotoHI
  {
    static DAGKindEnum const KIND = DAG_GOTO_HI;
  };
  
  struct DAGGotoLS
  {
    static DAGKindEnum const KIND = DAG_GOTO_LS;
  };
  
  struct DAGGotoGE
  {
    static DAGKindEnum const KIND = DAG_GOTO_GE;
  };
  
  struct DAGGotoLT
  {
    static DAGKindEnum const KIND = DAG_GOTO_LT;
  };
  
  struct DAGGotoGT
  {
    static DAGKindEnum const KIND = DAG_GOTO_GT;
  };
  
  struct DAGGotoLE
  {
    static DAGKindEnum const KIND = DAG_GOTO_LE;
  };
  
  struct DAGJump
  {
    static DAGKindEnum const KIND = DAG_JUMP;
  };
  
  struct DAGProduceCBit
  {
    static DAGKindEnum const KIND = DAG_PRODUCE_CBIT;
  };
  
  struct DAGSetCBit
  {
    static DAGKindEnum const KIND = DAG_SET_CBIT;
  };
  
  struct DAGClearCBit
  {
    static DAGKindEnum const KIND = DAG_CLEAR_CBIT;
  };
  
  struct DAGLoadLabel
  {
    static DAGKindEnum const KIND = DAG_LOAD_LABEL;
  };

  struct DAGLoad
  {
    static DAGKindEnum const KIND = DAG_LOAD;
  };
  
  // :NOTE: Wei 2004-Nov-3:
  //
  // I decide to have a 'MIRCall' instruction to
  // abstract different calling conventions on different
  // platforms.
  struct MIRCall_0
  {
    static MIRKindEnum const KIND = MIR_CALL_0;
    
    typedef DAGCall DAGNodeType;
  };
  
  struct MIRCall_1
  {
    static MIRKindEnum const KIND = MIR_CALL_1;
    
    typedef DAGCall DAGNodeType;
  };
  
  struct MIRReturn
  {
    static MIRKindEnum const KIND = MIR_RETURN;
    
    typedef DAGReturn DAGNodeType;
  };
  
  struct MIRRetrieve
  {
    static MIRKindEnum const KIND = MIR_RETRIEVE;
    
    typedef DAGRetrieve DAGNodeType;
  };
  
  struct MIRAdd
  {
    static MIRKindEnum const KIND = MIR_ADD;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGAdd DAGNodeType;
  };
  
  struct MIRAdd64
  {
    static MIRKindEnum const KIND = MIR_ADD64;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGAdd64 DAGNodeType;
  };
  
  struct MIRAdc
  {
    static MIRKindEnum const KIND = MIR_ADC;
    
    typedef DAGAdc DAGNodeType;
  };
  
  struct MIRMov
  {
    static MIRKindEnum const KIND = MIR_MOV;
    
    typedef DAGMov DAGNodeType;
  };
  
  struct MIRAnd
  {
    static MIRKindEnum const KIND = MIR_AND;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGAnd DAGNodeType;
  };
  
  struct MIROr
  {
    static MIRKindEnum const KIND = MIR_OR;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGOr DAGNodeType;
  };
  
  struct MIREor
  {
    static MIRKindEnum const KIND = MIR_EOR;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGEor DAGNodeType;
  };
  
  struct MIRSub
  {
    static MIRKindEnum const KIND = MIR_SUB;
    static bool const OPERAND_EXCHANGABLE = false;
    
    typedef DAGSub DAGNodeType;
  };
  
  struct MIRSbc
  {
    static MIRKindEnum const KIND = MIR_SBC;
    
    typedef DAGSbc DAGNodeType;
  };
  
  struct MIRTst
  {
    static MIRKindEnum const KIND = MIR_TST;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGTst DAGNodeType;
  };
  
  struct MIRCmp
  {
    static MIRKindEnum const KIND = MIR_CMP;
    static bool const OPERAND_EXCHANGABLE = false;
    
    typedef DAGCmp DAGNodeType;
  };
  
  struct MIRCmpEQ
  {
    static MIRKindEnum const KIND = MIR_CMP_EQ;
    
    typedef DAGCmpEQ DAGNodeType;
  };
  
  struct MIRNot
  {
    static MIRKindEnum const KIND = MIR_NOT;
    
    typedef DAGNot DAGNodeType;
  };
  
  struct MIRMul32
  {
    static MIRKindEnum const KIND = MIR_MUL32;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGMul32 DAGNodeType;
  };
  
  struct MIRUMul64
  {
    static MIRKindEnum const KIND = MIR_UMUL64;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGUMul64 DAGNodeType;
  };
  
  struct MIRSMul64
  {
    static MIRKindEnum const KIND = MIR_SMUL64;
    static bool const OPERAND_EXCHANGABLE = true;
    
    typedef DAGSMul64 DAGNodeType;
  };
  
  struct MIRLSftLeft
  {
    static MIRKindEnum const KIND = MIR_LSFT_LEFT;
    static bool const OPERAND_EXCHANGABLE = false;
    
    typedef DAGLSftLeft DAGNodeType;
  };
  
  struct MIRLSftRight
  {
    static MIRKindEnum const KIND = MIR_LSFT_RIGHT;
    static bool const OPERAND_EXCHANGABLE = false;
    
    typedef DAGLSftRight DAGNodeType;
  };
  
  struct MIRASftRight
  {
    static MIRKindEnum const KIND = MIR_ASFT_RIGHT;
    static bool const OPERAND_EXCHANGABLE = false;
    
    typedef DAGASftRight DAGNodeType;
  };
  
  struct MIRRrx
  {
    static MIRKindEnum const KIND = MIR_RRX;
    
    typedef DAGRrx DAGNodeType;
  };
  
  struct MIRRor
  {
    static MIRKindEnum const KIND = MIR_ROR;
    static bool const OPERAND_EXCHANGABLE = false;
    
    typedef DAGRor DAGNodeType;
  };
  
  struct MIRGoto
  {
    static MIRKindEnum const KIND = MIR_GOTO;
    
    typedef DAGGoto DAGNodeType;
  };
  
  struct MIRGotoEQ
  {
    static MIRKindEnum const KIND = MIR_GOTO_EQ;
    
    typedef DAGGotoEQ DAGNodeType;
  };
  
  struct MIRGotoNE
  {
    static MIRKindEnum const KIND = MIR_GOTO_NE;
    
    typedef DAGGotoNE DAGNodeType;
  };
  
  struct MIRGotoCS
  {
    static MIRKindEnum const KIND = MIR_GOTO_CS;
    
    typedef DAGGotoCS DAGNodeType;
  };
  
  struct MIRGotoCC
  {
    static MIRKindEnum const KIND = MIR_GOTO_CC;
    
    typedef DAGGotoCC DAGNodeType;
  };
  
  struct MIRGotoMI
  {
    static MIRKindEnum const KIND = MIR_GOTO_MI;
    
    typedef DAGGotoMI DAGNodeType;
  };
  
  struct MIRGotoPL
  {
    static MIRKindEnum const KIND = MIR_GOTO_PL;
    
    typedef DAGGotoPL DAGNodeType;
  };
  
  struct MIRGotoVS
  {
    static MIRKindEnum const KIND = MIR_GOTO_VS;
    
    typedef DAGGotoVS DAGNodeType;
  };
  
  struct MIRGotoVC
  {
    static MIRKindEnum const KIND = MIR_GOTO_VC;
    
    typedef DAGGotoVC DAGNodeType;
  };
  
  struct MIRGotoHI
  {
    static MIRKindEnum const KIND = MIR_GOTO_HI;
    
    typedef DAGGotoHI DAGNodeType;
  };
  
  struct MIRGotoLS
  {
    static MIRKindEnum const KIND = MIR_GOTO_LS;
    
    typedef DAGGotoLS DAGNodeType;
  };
  
  struct MIRGotoGE
  {
    static MIRKindEnum const KIND = MIR_GOTO_GE;
    
    typedef DAGGotoGE DAGNodeType;
  };
  
  struct MIRGotoLT
  {
    static MIRKindEnum const KIND = MIR_GOTO_LT;
    
    typedef DAGGotoLT DAGNodeType;
  };
  
  struct MIRGotoGT
  {
    static MIRKindEnum const KIND = MIR_GOTO_GT;
    
    typedef DAGGotoGT DAGNodeType;
  };
  
  struct MIRGotoLE
  {
    static MIRKindEnum const KIND = MIR_GOTO_LE;
    
    typedef DAGGotoLE DAGNodeType;
  };
  
  struct MIRJump
  {
    static MIRKindEnum const KIND = MIR_JUMP;
    
    typedef DAGJump DAGNodeType;
  };
  
  struct MIRProduceCBit
  {
    static MIRKindEnum const KIND = MIR_PRODUCE_CBIT;
    
    typedef DAGProduceCBit DAGNodeType;
  };
  
  struct MIRClearCBit
  {
    static MIRKindEnum const KIND = MIR_CLEAR_CBIT;
    
    typedef DAGClearCBit DAGNodeType;
  };
  
  struct MIRData32
  {
    static MIRKindEnum const KIND = MIR_DATA32;
  };
  
  struct MIRLoadLabel
  {
    static MIRKindEnum const KIND = MIR_LOAD_LABEL;
    
    typedef DAGLoadLabel DAGNodeType;
  };
  
  struct MIRLoad
  {
    static MIRKindEnum const KIND = MIR_LOAD;
    
    typedef DAGLoad DAGNodeType;
  };
  
  // :NOTE: Wei 2005-Mar-09:
  //
  // CondCodeTranslationTraits
  
  template<InstCond T_cond>
  struct CondCodeTranslationTraits;
  
  template<>
  struct CondCodeTranslationTraits<EQ>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = ZERO_FLAG;
    typedef MIRGotoNE ReverseMIRType;
    typedef MIRGotoEQ MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<NE>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = ZERO_FLAG;
    typedef MIRGotoEQ ReverseMIRType;
    typedef MIRGotoNE MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<CS>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = CARRY_FLAG;
    typedef MIRGotoCC ReverseMIRType;
    typedef MIRGotoCS MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<CC>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = CARRY_FLAG;
    typedef MIRGotoCS ReverseMIRType;
    typedef MIRGotoCC MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<MI>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = SIGN_FLAG;
    typedef MIRGotoPL ReverseMIRType;
    typedef MIRGotoMI MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<PL>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = SIGN_FLAG;
    typedef MIRGotoMI ReverseMIRType;
    typedef MIRGotoPL MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<VS>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = OVERFLOW_FLAG;
    typedef MIRGotoVC ReverseMIRType;
    typedef MIRGotoVS MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<VC>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = OVERFLOW_FLAG;
    typedef MIRGotoVS ReverseMIRType;
    typedef MIRGotoVC MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<HI>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = CARRY_FLAG | ZERO_FLAG;
    typedef MIRGotoLS ReverseMIRType;
    typedef MIRGotoHI MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<LS>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = CARRY_FLAG | ZERO_FLAG;
    typedef MIRGotoHI ReverseMIRType;
    typedef MIRGotoLS MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<GE>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = SIGN_FLAG | OVERFLOW_FLAG;
    typedef MIRGotoLT ReverseMIRType;
    typedef MIRGotoGE MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<LT>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = SIGN_FLAG | OVERFLOW_FLAG;
    typedef MIRGotoGE ReverseMIRType;
    typedef MIRGotoLT MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<GT>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = SIGN_FLAG | ZERO_FLAG | OVERFLOW_FLAG;
    typedef MIRGotoLE ReverseMIRType;
    typedef MIRGotoGT MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<LE>
  {
    static uint32_t const PRELOAD_STATUS_FLAG = SIGN_FLAG | ZERO_FLAG | OVERFLOW_FLAG;
    typedef MIRGotoGT ReverseMIRType;
    typedef MIRGotoLE MIRType;
  };
  
  template<>
  struct CondCodeTranslationTraits<AL>
  {
    // :NOTE: Wei 2004-Nov-1:
    //
    // Fake value.
    static uint32_t const PRELOAD_STATUS_FLAG = 0;
    
    // :NOTE: Wei 2004-Dec-9:
    //
    // Fake type.
    typedef MIRGotoEQ ReverseMIRType;
    typedef MIRGotoEQ MIRType;
  };
  
  // :NOTE: Wei 2004-Sep-20:
  //
  // Because ISO C++ forbids casting between pointer-to-function and
  // pointer-to-object, thus I can't casting 'ExecResultEnum (*)(Inst const)'
  // to 'void *'.
  //
  // The only way is casting every function pointers to the following
  // 'pointer-to-function'.
  typedef void (*DTFunc_t)();
  
  uint32_t const RED_ZONE_SIZE_IN_BYTE = 100;
}

#endif

#endif
