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

#ifndef MachineDep_hpp
#define MachineDep_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include "../../ARMware.hpp"
#include "LIR.hpp"

namespace ARMware
{
  uint32_t const NEEDED_SCRATCH_REG_NUMBER = 1;
  uint32_t const STACK_VAR_OFFSET_START = 1;
  
  struct NativeSBit
  {
    static FlagEnum const KIND = SIGN_FLAG;
    static uint32_t const NATIVE_BIT_POSITION = 7;
    static uint32_t const CORRESPONDING_ARM_BIT_POSITION = NBIT_POS;
  };
  typedef struct NativeSBit NativeSBit;
  
  struct NativeZBit
  {
    static FlagEnum const KIND = ZERO_FLAG;
    static uint32_t const NATIVE_BIT_POSITION = 6;
    static uint32_t const CORRESPONDING_ARM_BIT_POSITION = ZBIT_POS;
  };
  typedef struct NativeZBit NativeZBit;
  
  struct NativeCBit
  {
    static FlagEnum const KIND = CARRY_FLAG;
    static uint32_t const NATIVE_BIT_POSITION = 0;
    static uint32_t const CORRESPONDING_ARM_BIT_POSITION = CBIT_POS;
  };
  typedef struct NativeCBit NativeCBit;
  
  struct NativeOBit
  {
    static FlagEnum const KIND = OVERFLOW_FLAG;
    static uint32_t const NATIVE_BIT_POSITION = 11;
    static uint32_t const CORRESPONDING_ARM_BIT_POSITION = VBIT_POS;
  };
  typedef struct NativeOBit NativeOBit;
  
  template<typename T_native_bit>
  inline Variable *
  load_native_flag_internal(BasicBlock * const bb, Variable * const orig)
  {
    Variable * const tmp3 = gp_chunk->get_new_temp();
    Variable * const tmp4 = gp_chunk->get_new_temp();
    Variable * const tmp5 = gp_chunk->get_new_temp();
    
    bb->append_lir<true>(new LIR(static_cast<LIRAnd *>(0),
                                 gp_chunk->find_global_var(Variable::CPSR),
                                 static_cast<uint32_t>((1 << T_native_bit::CORRESPONDING_ARM_BIT_POSITION)),
                                 tmp3));
    
    // :NOTE: Wei 2004-Nov-30:
    //
    // All ARM flag bit are in the left hand side to the x86 flag bit.
    bb->append_lir<true>(new LIR(static_cast<LIRLSftRight *>(0),
                                 tmp3,
                                 T_native_bit::CORRESPONDING_ARM_BIT_POSITION -
                                 T_native_bit::NATIVE_BIT_POSITION,
                                 tmp4));
    
    bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                                 orig,
                                 tmp4,
                                 tmp5));
    
    return tmp5;
  }
  
  template<typename T_native_bit>
  inline void
  load_1_native_flag(BasicBlock * const bb)
  {
    Variable * const tmp1 = gp_chunk->get_new_temp();
    Variable * const tmp2 = gp_chunk->get_new_temp();
    
    bb->append_lir<true>(new LIR(static_cast<LIRPushF *>(0)));
    
    bb->append_lir<true>(new LIR(static_cast<LIRLoadSP *>(0), tmp1));
    
    bb->append_lir<true>(new LIR(static_cast<LIRAnd *>(0),
                                 tmp1,
                                 static_cast<uint32_t>(~(1 << T_native_bit::NATIVE_BIT_POSITION
#if DEBUG_MODE
                                                         // :NOTE: Wei 2005-Mar-31:
                                                         //
                                                         // Bit 8 of x86's eflags is TRACE flag.
                                                         // When I use gdb to trace the native code,
                                                         // gdb will raise this TRACE flag when I hit 'si' command.
                                                         // Thus, if I hit 'si' command for a PUSHF instruction,
                                                         // then the pushed value will be the real eflags register
                                                         // plus this TRACE bit.
                                                         //
                                                         // Thus, I have to turn off this TRACE bit to make
                                                         // the following debugging smoothly.
                                                         | (1 << 8)
#endif
                                                         )),
                                 tmp2));
    
    Variable * const result = load_native_flag_internal<T_native_bit>(bb, tmp2);
    
    bb->append_lir<true>(new LIR(static_cast<LIRStoreSP *>(0), result));
    
    bb->append_lir<true>(new LIR(static_cast<LIRPopF *>(0)));
  }
  
  template<typename T_native_bit_1, typename T_native_bit_2>
  inline void
  load_2_native_flag(BasicBlock * const bb)
  {
    Variable * const tmp1 = gp_chunk->get_new_temp();
    Variable * const tmp2 = gp_chunk->get_new_temp();
    
    bb->append_lir<true>(new LIR(static_cast<LIRPushF *>(0)));
    bb->append_lir<true>(new LIR(static_cast<LIRLoadSP *>(0), tmp1));
    
    bb->append_lir<true>(new LIR(static_cast<LIRAnd *>(0),
                                 tmp1,
                                 static_cast<uint32_t>(~((1 << T_native_bit_1::NATIVE_BIT_POSITION) |
                                                         (1 << T_native_bit_2::NATIVE_BIT_POSITION)
#if DEBUG_MODE
                                                         | (1 << 8)
#endif
                                                         )),
                                 tmp2));
    
    Variable *result = load_native_flag_internal<T_native_bit_1>(bb, tmp2);
    result = load_native_flag_internal<T_native_bit_2>(bb, result);
    
    bb->append_lir<true>(new LIR(static_cast<LIRStoreSP *>(0), result));
    bb->append_lir<true>(new LIR(static_cast<LIRPopF *>(0)));
  }
  
  template<typename T_native_bit_1, typename T_native_bit_2, typename T_native_bit_3>
  inline void
  load_3_native_flag(BasicBlock * const bb)
  {
    Variable * const tmp1 = gp_chunk->get_new_temp();
    Variable * const tmp2 = gp_chunk->get_new_temp();
    
    bb->append_lir<true>(new LIR(static_cast<LIRPushF *>(0)));
    bb->append_lir<true>(new LIR(static_cast<LIRLoadSP *>(0), tmp1));
    
    bb->append_lir<true>(new LIR(static_cast<LIRAnd *>(0),
                                 tmp1,
                                 static_cast<uint32_t>(~((1 << T_native_bit_1::NATIVE_BIT_POSITION) |
                                                         (1 << T_native_bit_2::NATIVE_BIT_POSITION) |
                                                         (1 << T_native_bit_3::NATIVE_BIT_POSITION)
#if DEBUG_MODE
                                                         | (1 << 8)
#endif
                                                         )),
                                 tmp2));
    
    Variable *result = load_native_flag_internal<T_native_bit_1>(bb, tmp2);
    result = load_native_flag_internal<T_native_bit_2>(bb, result);
    result = load_native_flag_internal<T_native_bit_3>(bb, result);
    
    bb->append_lir<true>(new LIR(static_cast<LIRStoreSP *>(0), result));
    bb->append_lir<true>(new LIR(static_cast<LIRPopF *>(0)));
  }
  
  template<typename T_native_bit_1, typename T_native_bit_2,
           typename T_native_bit_3, typename T_native_bit_4>
  inline void
  load_4_native_flag(BasicBlock * const bb)
  {
    Variable * const tmp1 = gp_chunk->get_new_temp();
    Variable * const tmp2 = gp_chunk->get_new_temp();
    
    bb->append_lir<true>(new LIR(static_cast<LIRPushF *>(0)));
    bb->append_lir<true>(new LIR(static_cast<LIRLoadSP *>(0), tmp1));
    
    bb->append_lir<true>(new LIR(static_cast<LIRAnd *>(0),
                                 tmp1,
                                 static_cast<uint32_t>(~((1 << T_native_bit_1::NATIVE_BIT_POSITION) |
                                                         (1 << T_native_bit_2::NATIVE_BIT_POSITION) |
                                                         (1 << T_native_bit_3::NATIVE_BIT_POSITION) |
                                                         (1 << T_native_bit_4::NATIVE_BIT_POSITION)
#if DEBUG_MODE
                                                         | (1 << 8)
#endif
                                                         )),
                                 tmp2));
    
    Variable *result = load_native_flag_internal<T_native_bit_1>(bb, tmp2);
    result = load_native_flag_internal<T_native_bit_2>(bb, result);
    result = load_native_flag_internal<T_native_bit_3>(bb, result);
    result = load_native_flag_internal<T_native_bit_4>(bb, result);
    
    bb->append_lir<true>(new LIR(static_cast<LIRStoreSP *>(0), result));
    bb->append_lir<true>(new LIR(static_cast<LIRPopF *>(0)));
  }
  
  inline void
  load_native_carry_flag(BasicBlock * const bb)
  {
    // :NOTE: Wei 2004-Nov-30:
    //
    // If we need C bit, but only C bit not reside in native status register,
    // then we can use x86's BT instruction to load C bit.
    bb->append_lir<true>(new LIR(static_cast<LIRBt *>(0),
                           gp_chunk->find_global_var(Variable::CPSR),
                           CBIT_POS));
  }
      
  template<FlagEnum T_flag>
  inline Variable *
  get_native_flag(BasicBlock * const bb)
  {
    Variable * const tmp = gp_chunk->get_new_temp();
    
    switch (T_flag)
    {
    case SIGN_FLAG:
      bb->append_lir<true>(new LIR(static_cast<LIRSetS *>(0), tmp));
      break;
      
    case ZERO_FLAG:
      bb->append_lir<true>(new LIR(static_cast<LIRSetZ *>(0), tmp));
      break;
      
    case CARRY_FLAG:
      bb->append_lir<true>(new LIR(static_cast<LIRSetC *>(0), tmp));
      break;
      
    case OVERFLOW_FLAG:
      bb->append_lir<true>(new LIR(static_cast<LIRSetO *>(0), tmp));
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    // :NOTE: Wei 2005-Apr-6:
    //
    // Because the SetCC instructions of x86 platform eat 8-bit register,
    // thus they don't eat ESI & EDI.
    tmp->add_excluded_reg(Register::ESI);
    tmp->add_excluded_reg(Register::EDI);
    
    return tmp;
  }
  
  template<typename T_native_bit>
  inline void
  store_native_flag(BasicBlock * const bb, Variable * const tmp)
  {
    Variable * const dest = gp_chunk->get_new_temp();
    
    bb->append_lir<true>(new LIR(static_cast<LIRLSftLeft *>(0),
                           tmp,
                           T_native_bit::CORRESPONDING_ARM_BIT_POSITION,
                           dest));
    
    bb->append_lir<true>(new LIR(static_cast<LIROr *>(0),
                           dest,
                           gp_chunk->find_global_var(Variable::CPSR),
                           gp_chunk->find_global_var(Variable::CPSR)));
  }
  
  inline void
  store_native_flags(BasicBlock * const bb, uint32_t const wanted_flags)
  {
    assert(0 == (wanted_flags & ~(SIGN_FLAG |
                                  ZERO_FLAG |
                                  CARRY_FLAG |
                                  OVERFLOW_FLAG)));
    
    if (wanted_flags != 0)
    {
      Variable *tmp_n = 0;
      Variable *tmp_z = 0;
      Variable *tmp_c = 0;
      Variable *tmp_v = 0;
      
      uint32_t flags_bitset = 0;
      
      if (wanted_flags & SIGN_FLAG)
      {
        tmp_n = get_native_flag<SIGN_FLAG>(bb);
        
        flags_bitset |= (1 << NativeSBit::CORRESPONDING_ARM_BIT_POSITION);
      }
      
      if (wanted_flags & ZERO_FLAG)
      {
        tmp_z = get_native_flag<ZERO_FLAG>(bb);
        
        flags_bitset |= (1 << NativeZBit::CORRESPONDING_ARM_BIT_POSITION);
      }
      
      if (wanted_flags & CARRY_FLAG)
      {
        tmp_c = get_native_flag<CARRY_FLAG>(bb);
        
        flags_bitset |= (1 << NativeCBit::CORRESPONDING_ARM_BIT_POSITION);
      }
      
      if (wanted_flags & OVERFLOW_FLAG)
      {
        tmp_v = get_native_flag<OVERFLOW_FLAG>(bb);
        
        flags_bitset |= (1 << NativeOBit::CORRESPONDING_ARM_BIT_POSITION);
      }
      
      bb->append_lir<true>(new LIR(static_cast<LIRAnd *>(0),
                                   gp_chunk->find_global_var(Variable::CPSR),
                                   ~flags_bitset,
                                   gp_chunk->find_global_var(Variable::CPSR)));
      
      if (tmp_n != 0)
      {
        ARMware::store_native_flag<NativeSBit>(bb, tmp_n);
      }
      
      if (tmp_z != 0)
      {
        ARMware::store_native_flag<NativeZBit>(bb, tmp_z);
      }
      
      if (tmp_c != 0)
      {
        ARMware::store_native_flag<NativeCBit>(bb, tmp_c);
      }
      
      if (tmp_v != 0)
      {
        ARMware::store_native_flag<NativeOBit>(bb, tmp_v);
      }
    }
  }
  
  inline void
  load_native_flags(BasicBlock * const bb, uint32_t const needed_flags)
  {
    switch (needed_flags)
    {
    case SIGN_FLAG | ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
      load_4_native_flag<NativeSBit, NativeZBit, NativeOBit, NativeCBit>(bb);
      break;
      
    case ZERO_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
      load_3_native_flag<NativeZBit, NativeOBit, NativeCBit>(bb);
      break;
      
    case SIGN_FLAG | CARRY_FLAG | OVERFLOW_FLAG:
      load_3_native_flag<NativeSBit, NativeOBit, NativeCBit>(bb);
      break;
      
    case SIGN_FLAG | ZERO_FLAG | OVERFLOW_FLAG:
      load_3_native_flag<NativeSBit, NativeZBit, NativeOBit>(bb);
      break;
      
    case SIGN_FLAG | ZERO_FLAG | CARRY_FLAG:
      load_3_native_flag<NativeSBit, NativeZBit, NativeCBit>(bb);
      break;
      
    case SIGN_FLAG | ZERO_FLAG:
      load_2_native_flag<NativeSBit, NativeZBit>(bb);
      break;
      
    case SIGN_FLAG | CARRY_FLAG:
      load_2_native_flag<NativeSBit, NativeCBit>(bb);
      break;
      
    case SIGN_FLAG | OVERFLOW_FLAG:
      load_2_native_flag<NativeSBit, NativeOBit>(bb);
      break;
      
    case ZERO_FLAG | CARRY_FLAG:
      load_2_native_flag<NativeZBit, NativeCBit>(bb);
      break;
      
    case ZERO_FLAG | OVERFLOW_FLAG:
      load_2_native_flag<NativeZBit, NativeOBit>(bb);
      break;
      
    case CARRY_FLAG | OVERFLOW_FLAG:
      load_2_native_flag<NativeOBit, NativeCBit>(bb);
      break;
      
    case SIGN_FLAG:
      load_1_native_flag<NativeSBit>(bb);
      break;
      
    case ZERO_FLAG:
      load_1_native_flag<NativeZBit>(bb);
      break;
      
    case CARRY_FLAG:
      load_native_carry_flag(bb);
      break;
      
    case OVERFLOW_FLAG:
      load_1_native_flag<NativeOBit>(bb);
      break;
      
    case 0:
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
}

#endif

#endif
