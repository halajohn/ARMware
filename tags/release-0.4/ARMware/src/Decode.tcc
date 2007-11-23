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
#if ENABLE_THREADED_CODE
  template<UsageEnum T_usage>
#endif
  inline ExecResultEnum
  Core::decode(Inst const inst)
  {
    // :NOTE: Wei 2004-Jan-20:
    //
    // Use bits[27:20] as the key of instruction's lookup table.
    switch (inst.mRawData & 0x0FF00000)
    {
    case 0x00000000: // 0000 0000 0000 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // AND: I[25] = 0: register form of shifter operand
        //      opcode[24:21] = 0000: AND
        //      S[20] = 0: No update CPSR
        //      Immediate shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_AND_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_AND_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // AND: I[25] = 0: register form of shifter operand
        //      opcode[24:21] = 0000: AND
        //      S[20] = 0: No update CPSR
        //      Register shift of register form (Mode 3)
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_AND_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_AND_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // MUL: S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MUL(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MUL_arglist);
          break;
        }
        break;
#endif
        
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        
        // STRH: P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 0: register offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_reg_post_down_arglist);
          break;
        }
        break;
#endif
        
#endif
        
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x00100000: // 0000 0000 0001 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // ANDS: I[25] = 0: register form of shifter operand
        //       opcode[24:21] = 0000: AND
        //       S[20] = 1: Update CPSR
        //       Immediate shift of register form (Mode 2)
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ANDS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ANDS_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // ANDS: I[25] = 0: register form of shifter operand
        //       opcode[24:21] = 0000: AND
        //       S[20] = 1: Update CPSR
        //       Register shift of register form (Mode 3)
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ANDS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ANDS_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // MULS: S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MULS(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MULS_arglist);
          break;
        }
        break;
#endif
        
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 0: register offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_reg_post_down_arglist);
          break;
        }
        break;
#endif
        
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 0: post-indexed addressing
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 0: register offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_reg_post_down_arglist);
          break;
        }
        break;
#endif
        
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 0: post-indexed addressing
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 0: register offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_reg_post_down_arglist);
          break;
        }
        break;
#endif
        
#endif
      }
      break;
      
    case 0x00200000: // 0000 0000 0010 0000 0000 0000 0000 0000
      // :ARM-DDI-0100E: p.A5-35: Wei 2004-Jan-31:
      //
      // if P[24] == 0, then the W[21] bit must be 0 or the instruction is UNPREDICTABLE.
      // Thus this bit pattern can't be a load store halfword or load signed byte instruction.
      switch (inst.mRawData & 0x90)
      {
      case 0x00: // 0000 0000
      case 0x80: // 1000 0000
        // EOR: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0001: EOR
        //      S[20] = 0: No update CPSR
        //      Immediate shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_EOR_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_EOR_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
        // EOR: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0001: EOR
        //      S[20] = 0: No update CPSR
        //      register shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_EOR_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_EOR_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // MLA: S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MLA(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MLA_arglist);
          break;
        }
        break;
#endif
      }
      break;
      
    case 0x00300000: // 0000 0000 0011 0000 0000 0000 0000 0000
      // :ARM-DDI-0100E: p.A5-35: Wei 2004-Jan-31:
      //
      // if P[24] = 0, then the W[21] bit must be 0 or the instruction is UNPREDICTABLE.
      // Thus this bit pattern can't be a load store halfword or load signed byte instruction.
      switch (inst.mRawData & 0x90)
      {
      case 0x00: // 0000 0000
      case 0x80: // 1000 0000
        // EORS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0001: EOR
        //       S[20] = 1: Update CPSR
        //       Immediate shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_EORS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_EORS_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
        // EORS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0001: EOR
        //       S[20] = 1: Update CPSR
        //       register shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_EORS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_EORS_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // MLAS: S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MLAS(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MLAS_arglist);
          break;
        }
        break;
#endif
      }
      break;
      
    case 0x00400000: // 0000 0000 0100 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // SUB: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0010: SUB
        //      S[20] = 0: No update CPSR
        //      Immediate shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SUB_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SUB_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // SUB: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0010: SUB
        //      S[20] = 0: No update CPSR
        //      Register shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SUB_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SUB_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
        
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_imm_post_down_arglist);
          break;
        }
        break;
#endif
        
#endif
        
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x00500000: // 0000 0000 0101 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // SUBS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0010: SUB
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SUBS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SUBS_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // SUBS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0010: SUB
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SUBS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SUBS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
        
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_imm_post_down_arglist);
          break;
        }
        break;
#endif
        
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 0: post-indexed addressing
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_imm_post_down_arglist);
          break;
        }
        break;
#endif
        
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 0: post-indexed addressing
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_imm_post_down_arglist);
          break;
        }
        break;
#endif
        
#endif
      }
      break;
        
    case 0x00600000: // 0000 0000 0110 0000 0000 0000 0000 0000
      // :ARM-DDI-0100E: p.A5-35: Wei 2004-Jan-31:
      //
      // if P[24] == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
      // Thus this bit pattern can't be a load store halfword or load signed byte instruction.
      switch (inst.mRawData & 0x90)
      {
      case 0x00: // 0000 0000
      case 0x80: // 1000 0000
        // RSB: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0011: RSB
        //      S[20] = 0: No update CPSR
        //      Immediate shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSB_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSB_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
        // RSB: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0011: RSB
        //      S[20] = 0: No update CPSR
        //      Register shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSB_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSB_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // :ARM-DDI-0100E: p.A3-2: Wei 2004-Jan-31:
        //
        // If bits[27:25] = 000, bit[7] = 1, bit[4] = 1, then this bit pattern should be a
        // Multiplies and extra load/store instructions.
        //
        // And because of the bits[24:20] = 00110, this bit pattern should be a
        // load store halfword instruction.
        //
        // However, according to the "Addressing Mode 3 - Miscellaneous Loads and Stores"
        // if P[24] == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x00700000: // 0000 0000 0111 0000 0000 0000 0000 0000
      // :ARM-DDI-0100E: p.A5-35: Wei 2004-Jan-31:
      //
      // if P[24] == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
      // Thus this bit pattern can't be a load store halfword or load signed byte instruction.
      switch (inst.mRawData & 0x90)
      {
      case 0x00: // 0000 0000
      case 0x80: // 1000 0000
        // RSBS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0011: RSB
        //       S[20] = 1: Update CPSR
        //       Immediate shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSBS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSBS_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
        // RSBS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0011: RSB
        //       S[20] = 1: Update CPSR
        //       Register shift of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSBS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSBS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARM-DDI-0100E: p.A3-2: Wei 2004-Jan-31:
        //
        // If bits[27:25] = 000, bit[7] = 1, bit[4] = 1, then this bit pattern should be a
        // Multiplies and extra load/store instructions.
        //
        // And because of the bits[24:20] = 00110, this bit pattern should be a
        // load store halfword instruction.
        //
        // However, according to the "Addressing Mode 3 - Miscellaneous Loads and Stores"
        // if P[24] == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x00800000: // 0000 0000 1000 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // ADD: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0100: ADD
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADD_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADD_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // ADD: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0100: ADD
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADD_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADD_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // UMULL: S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UMULL(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UMULL_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added to the base
        //       I[22] = 0: register offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_reg_post_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x00900000: // 0000 0000 1001 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // ADDS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0100: ADD
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADDS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADDS_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // ADDS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0100: ADD
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADDS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADDS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // UMULLS: S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UMULLS(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UMULLS_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added to the base
        //       I[22] = 0: register offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 0: post-indexed addressing
        //        U[23] = 1: offset is added to the base
        //        I[22] = 0: register offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 0: post-indexed addressing
        //        U[23] = 1: offset is added to the base
        //        I[22] = 0: register offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_reg_post_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x00A00000: // 0000 0000 1010 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // ADC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0101: ADC
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADC_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADC_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // ADC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0101: ADC
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADC_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADC_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // UMLAL: S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UMLAL(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UMLAL_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARMARM: 2004-Jan-25:
        //
        // In load store half word instruction:
        // if P(bit 24) == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x00B00000: // 0000 0000 1011 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // ADCS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0101: ADC
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADCS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADCS_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // ADCS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0101: ADC
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ADCS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ADCS_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // UMLALS: S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UMLALS(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UMLALS_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARMARM: 2004-Jan-25:
        //
        // In load store half word instruction:
        // if P(bit 24) == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x00C00000: // 0000 0000 1100 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // SBC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0110: SBC
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SBC_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SBC_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // SBC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0110: SBC
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SBC_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SBC_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // SMULL: S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SMULL(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SMULL_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added to the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_imm_post_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x00D00000: // 0000 0000 1101 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // SBCS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0110: SBC
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SBCS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SBCS_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // SBCS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0110: SBC
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SBCS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SBCS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // SMULLS: S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SMULLS(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SMULLS_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added to the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: _must_ be 0
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_imm_post_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 0: post-indexed addressing
        //        U[23] = 1: offset is added to the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_imm_post_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 0: post-indexed addressing
        //        U[23] = 1: offset is added to the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: _must_ be 0
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_imm_post_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x00E00000: // 0000 0000 1110 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // RSC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0111: RSC
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSC_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSC_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // RSC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 0111: RSC
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSC_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSC_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // SMLAL: S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SMLAL(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SMLAL_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARMARM: 2004-Jan-25:
        //
        // In load store half word instruction:
        // if P(bit 24) == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x00F00000: // 0000 0000 1111 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // RSCS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0111: RSC
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSCS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSCS_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // RSCS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 0111: RSC
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_RSCS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_RSCS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // SMLALS: S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SMLALS(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SMLALS_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARMARM: 2004-Jan-25:
        //
        // In load store half word instruction:
        // if P(bit 24) == 0, then the W[21] must be 0 or the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01000000: // 0000 0001 0000 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
        // MRS: R[22] = 0: read from CPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MRS_cpsr(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MRS_cpsr_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x20: // 0010 0000
      case 0x30: // 0011 0000
      case 0x40: // 0100 0000
      case 0x50: // 0101 0000
      case 0x60: // 0110 0000
      case 0x70: // 0111 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // :ARMARM: 2004-Jan-25:
        //
        // In ARM architecture version 4 and above, if the decode bits
        // of an instruction are not equal to those of any defined
        // instruction, then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // SWP:
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SWP(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SWP_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 0: register offset
        //       W[21] = 0: offset addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_reg_offset_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01100000: // 0000 0001 0001 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // TST: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1000: TST
        //      S[20] = 1: Update CPSR (S[20] is always 1 in TST)
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_TST_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_TST_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // TST: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1000: TST
        //      S[20] = 1: Update CPSR (S[20] is always 1 in TST)
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_TST_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_TST_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 0: register offset
        //       W[21] = 0: offset addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_reg_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 0: register offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_reg_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 0: register offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_reg_offset_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x01200000: // 0000 0001 0010 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
        // MSR: R[22] = 0: Update CPSR
        //      Register operand
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MSR_reg_cpsr(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MSR_reg_cpsr_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
        // BX:
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_BX(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_BX_arglist);
          break;
        }
        break;
#endif
          
      case 0x30: // 0011 0000
        // BLX(2):
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_BLX_2(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_BLX_2_arglist);
          break;
        }
        break;
#endif
          
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x50: // 0101 0000
      case 0x60: // 0110 0000
      case 0x70: // 0111 0000
      case 0x80: // 1000 0000
      case 0x90: // 1001 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // :ARMARM: 2004-Jan-25:
        //
        // In ARM architecture version 4 and above, if the decode bits
        // of an instruction are not equal to those of any defined
        // instruction, then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 0: register offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_reg_pre_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01300000: // 0000 0001 0011 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // TEQ: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1001: TEQ
        //      S[20] = 1: Update CPSR (S[20] is always 1 in TEQ)
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_TEQ_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_TEQ_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // TEQ: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1001: TEQ
        //      S[20] = 1: Update CPSR (S[20] is always 1 in TEQ)
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_TEQ_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_TEQ_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 0: register offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_reg_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 0: register offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_reg_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 0: register offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_reg_pre_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x01400000: // 0000 0001 0100 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
        // MRS: R[22] = 1: read from SPSR
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MRS_spsr(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MRS_spsr_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x20: // 0010 0000
      case 0x30: // 0011 0000
      case 0x40: // 0100 0000
      case 0x50: // 0101 0000
      case 0x60: // 0110 0000
      case 0x70: // 0111 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // SWPB:
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_SWPB(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_SWPB_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: offset addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_imm_offset_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01500000: // 0000 0001 0101 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // CMP: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1010: CMP
        //      S[20] = 1: Update CPSR (S[20] is always 1 in CMP)
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CMP_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CMP_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // CMP: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1010: CMP
        //      S[20] = 1: Update CPSR (S[20] is always 1 in CMP)
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CMP_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CMP_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: offset addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_imm_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_imm_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_imm_offset_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x01600000: // 0000 0001 0110 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
        // MSR: R[22] = 1: Update SPSR
        //      Register operand
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MSR_reg_spsr(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MSR_reg_spsr_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
        // CLZ:
        // :NOTE: Wei 2004-Aug-09:
        //
        // I don't implement CLZ now.
        assert(!"Should not reach here.");
        break;
        
      case 0x20: // 0010 0000
      case 0x30: // 0011 0000
      case 0x40: // 0100 0000
      case 0x50: // 0101 0000
      case 0x60: // 0110 0000
      case 0x70: // 0111 0000
      case 0x80: // 1000 0000
      case 0x90: // 1001 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 1: immediate offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_imm_pre_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01700000: // 0000 0001 0111 0000 0000 0000 0000 0000
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // CMN: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1011: CMN
        //      S[20] = 1: Update CPSR (S[20] is always 1 in CMN)
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CMN_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CMN_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // CMN: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1011: CMN
        //      S[20] = 1: Update CPSR (S[20] is always 1 in CMN)
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CMN_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CMN_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 0: offset is subtracted from the base
        //       I[22] = 1: immediate offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_imm_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 1: immediate offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_imm_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 0: offset is subtracted from the base
        //        I[22] = 1: immediate offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_imm_pre_down_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x01800000: // 0000 0001 1000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // ORR: I[25] = 0: register form of shifter_operand
      //      opcode[24:21] = 1100: ORR
      //      S[20] = 0: No update CPSR
      //
      // STRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 0: register offset
      //       W[21] = 0: offset addressing
      //       L[20] = 0: store
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // ORR: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1100: ORR
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ORR_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ORR_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // ORR: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1100: ORR
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ORR_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ORR_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 0: register offset
        //       W[21] = 0: offset addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_reg_offset_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01900000: // 0000 0001 1001 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // ORRS: I[25] = 0: register form of shifter_operand
      //       opcode[24:21] = 1100: ORR
      //       S[20] = 1: Update CPSR
      //
      // LDRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 0: register offset
      //       W[21] = 0: offset addressing
      //       L[20] = 1: load
      //
      // LDRSH: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 0: register offset
      //        W[21] = 0: offset addressing
      //        L[20] = 1: load
      //
      // LDRSB: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 0: register offset
      //        W[21] = 0: offset addressing
      //        L[20] = 1: load
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // ORRS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1100: ORR
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ORRS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ORRS_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // ORRS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1100: ORR
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_ORRS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_ORRS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 0: register offset
        //       W[21] = 0: offset addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_reg_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 0: register offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_reg_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 0: register offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_reg_offset_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x01A00000: // 0000 0001 1010 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // MOV: I[25] = 0: register form of shifter_operand
      //      opcode[24:21] = 1101: MOV
      //      S[20] = 0: No update CPSR
      //
      // STRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 0: register offset
      //       W[21] = 1: pre-indexed addressing
      //       L[20] = 0: store
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // MOV: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1101: MOV
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MOV_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MOV_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // MOV: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1101: MOV
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MOV_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MOV_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 0: register offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_reg_pre_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01B00000: // 0000 0001 1011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // MOVS: I[25] = 0: register form of shifter_operand
      //       opcode[24:21] = 1101: MOV
      //       S[20] = 1: Update CPSR
      //
      // LDRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 0: register offset
      //       W[21] = 1: pre-indexed addressing
      //       L[20] = 1: load
      //
      // LDRSH: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 0: register offset
      //        W[21] = 1: pre-indexed addressing
      //        L[20] = 1: load
      //
      // LDRSB: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 0: register offset
      //        W[21] = 1: pre-indexed addressing
      //        L[20] = 1: load
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // MOVS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1101: MOV
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MOVS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MOVS_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // MOVS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1101: MOV
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MOVS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MOVS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 0: register offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_reg_pre_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 0: register offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_reg_pre_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 0: register offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_reg_pre_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x01C00000: // 0000 0001 1100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // BIC: I[25] = 0: register form of shifter_operand
      //      opcode[24:21] = 1110: BIC
      //      S[20] = 0: No update CPSR
      //
      // STRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 1: immediate offset
      //       W[21] = 0: offset addressing
      //       L[20] = 0: store
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // BIC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1110: BIC
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_BIC_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_BIC_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // BIC: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1110: BIC
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_BIC_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_BIC_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: offset addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_imm_offset_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
          
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
        
    case 0x01D00000: // 0000 0001 1101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // BICS: I[25] = 0: register form of shifter_operand
      //       opcode[24:21] = 1110: BIC
      //       S[20] = 1: Update CPSR
      //
      // LDRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 1: immediate offset
      //       W[21] = 0: offset addressing
      //       L[20] = 1: load
      //
      // LDRSH: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 1: immediate offset
      //        W[21] = 0: offset addressing
      //        L[20] = 1: load
      //
      // LDRSB: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 1: immediate offset
      //        W[21] = 0: offset addressing
      //        L[20] = 1: load
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // BICS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1110: BIC
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_BICS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_BICS_regimm_arglist);
          break;
        }
        break;
#endif
          
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // BICS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1110: BIC
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_BICS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_BICS_regreg_arglist);
          break;
        }
        break;
#endif
          
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 1: immediate offset
        //       W[21] = 0: offset addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_imm_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_imm_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 1: immediate offset
        //        W[21] = 0: offset addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_imm_offset_up_arglist);
          break;
        }
        break;
#endif
        
#endif
      }
      break;
      
    case 0x01E00000: // 0000 0001 1110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // MVN: I[25] = 0: register form of shifter_operand
      //      opcode[24:21] = 1111: MVN
      //      S[20] = 0: No update CPSR
      //
      // STRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 1: immediate offset
      //       W[21] = 1: pre-indexed addressing
      //       L[20] = 0: store
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // MVN: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1111: MVN
        //      S[20] = 0: No update CPSR
        //      Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MVN_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MVN_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // MVN: I[25] = 0: register form of shifter_operand
        //      opcode[24:21] = 1111: MVN
        //      S[20] = 0: No update CPSR
        //      Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MVN_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MVN_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
        
      case 0xB0: // 1011 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        // STRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 1: immediate offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRH_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRH_imm_pre_up_arglist);
          break;
        }
        break;
#endif
        
#endif
        
      case 0xD0: // 1101 0000
      case 0xF0: // 1111 0000
        // :ARM-DDI-0100E: p.A3-18: Wei 2004-Jan-31:
        //
        // For load store half word instruction:
        // If the L(20) bit is zero and S(6) bit is one, the instruction is UNPREDICTABLE.
        assert(!"Should not reach here.");
        break;
      }
      break;
      
    case 0x01F00000: // 0000 0001 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // MVNS: I[25] = 0: register form of shifter_operand
      //       opcode[24:21] = 1111: MVN
      //       S[20] = 1: Update CPSR
      //
      // LDRH: P[24] = 1:
      //       U[23] = 1: offset is added to the base
      //       I[22] = 1: immediate offset
      //       W[21] = 1: pre-indexed addressing
      //       L[20] = 1: load
      //
      // LDRSH: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 1: immediate offset
      //        W[21] = 1: pre-indexed addressing
      //        L[20] = 1: load
      //
      // LDRSB: P[24] = 1:
      //        U[23] = 1: offset is added to the base
      //        I[22] = 1: immediate offset
      //        W[21] = 1: pre-indexed addressing
      //        L[20] = 1: load
      switch (inst.mRawData & 0xF0)
      {
      case 0x00: // 0000 0000
      case 0x20: // 0010 0000
      case 0x40: // 0100 0000
      case 0x60: // 0110 0000
      case 0x80: // 1000 0000
      case 0xA0: // 1010 0000
      case 0xC0: // 1100 0000
      case 0xE0: // 1110 0000
        // MVNS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1111: MVN
        //       S[20] = 1: Update CPSR
        //       Immediate shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MVNS_regimm(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MVNS_regimm_arglist);
          break;
        }
        break;
#endif
        
      case 0x10: // 0001 0000
      case 0x30: // 0011 0000
      case 0x50: // 0101 0000
      case 0x70: // 0111 0000
        // MVNS: I[25] = 0: register form of shifter_operand
        //       opcode[24:21] = 1111: MVN
        //       S[20] = 1: Update CPSR
        //       Register shifts of register form
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_MVNS_regreg(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_MVNS_regreg_arglist);
          break;
        }
        break;
#endif
        
      case 0x90: // 1001 0000
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
          
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      case 0xB0:
      case 0xD0:
      case 0xF0:
        // :ARM-DDI-0100E: p.A3-3: Wei 2004-Jan-31:
        //
        // UNPREDICTABLE prior to ARM architecture version 4.
        assert(!"Should not reach here.");
        break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      case 0xB0: // 1011 0000
        // LDRH: P[24] = 1:
        //       U[23] = 1: offset is added to the base
        //       I[22] = 1: immediate offset
        //       W[21] = 1: pre-indexed addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRH_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRH_imm_pre_up_arglist);
          break;
        }
        break;
#endif
          
      case 0xD0: // 1101 0000
        // LDRSB: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 1: immediate offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSB_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSB_imm_pre_up_arglist);
          break;
        }
        break;
#endif
        
      case 0xF0: // 1111 0000
        // LDRSH: P[24] = 1:
        //        U[23] = 1: offset is added to the base
        //        I[22] = 1: immediate offset
        //        W[21] = 1: pre-indexed addressing
        //        L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRSH_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRSH_imm_pre_up_arglist);
          break;
        }
        break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      }
      break;
        
    case 0x02000000: // 0000 0010 0000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // AND: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0000: AND
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_AND_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_AND_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02100000: // 0000 0010 0001 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // ANDS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0000: AND
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_ANDS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_ANDS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02200000: // 0000 0010 0010 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // EOR: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0001: EOR
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_EOR_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_EOR_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02300000: // 0000 0010 0011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // EORS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0001: EOR
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_EORS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_EORS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02400000: // 0000 0010 0100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // SUB: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0010: SUB
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_SUB_imm(inst);
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_SUB_imm_arglist);
        break;
      }
      break;
#endif
      
    case 0x02500000: // 0000 0010 0101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // SUBS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0010: SUB
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_SUBS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_SUBS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02600000: // 0000 0010 0110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // RSB: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0011: RSB
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_RSB_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_RSB_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02700000: // 0000 0010 0111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // RSBS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0011: RSB
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_RSBS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_RSBS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02800000: // 0000 0010 1000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // ADD: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0100: ADD
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_ADD_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_ADD_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02900000: // 0000 0010 1001 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // ADDS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0100: ADD
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_ADDS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_ADDS_imm_arglist);
        break;
      }
      break;
#endif
      
    case 0x02A00000: // 0000 0010 1010 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // ADC: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0101: ADC
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_ADC_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_ADC_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02B00000: // 0000 0010 1011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // ADCS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0101: ADC
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_ADCS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_ADCS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02C00000: // 0000 0010 1100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // SBC: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0110: SBC
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_SBC_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_SBC_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02D00000: // 0000 0010 1101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // SBCS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0110: SBC
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_SBCS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_SBCS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02E00000: // 0000 0010 1110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // RSC: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 0111: RSC
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_RSC_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_RSC_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x02F00000: // 0000 0010 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // RSCS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 0111: RSC
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_RSCS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_RSCS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03000000: // 0000 0011 0000 0000 0000 0000 0000 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      // :ARMARM: Wei 2004-Jan-29:
      //
      // UNPREDICTABLE prior to ARM architecture version 4.
      assert(!"Should not reach here.");
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      // :ARMARM: Wei 2004-Jan-23:
      //
      // In ARM architecture version 4 and above, if the decode bits of
      // an instruction are not equal to those of any defined instruction,
      // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
        break;
      }
      break;
#endif

#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        
    case 0x03100000: // 0000 0011 0001 0000 0000 0000 0000 0000
      // TST: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1000: TST
      //      S[20] = 1: Update CPSR (S[20] is always 1 in TST)
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_TST_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_TST_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03200000: // 0000 0011 0010 0000 0000 0000 0000 0000
      // MSR: R[22] = 0: Update CPSR
      //      Immediate operand
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_MSR_imm_cpsr(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_MSR_imm_cpsr_arglist);
        break;
      }
      break;
#endif
        
    case 0x03300000: // 0000 0011 0011 0000 0000 0000 0000 0000
      // TEQ: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1001: TEQ
      //      S[20] = 1: Update CPSR (S[20] is always 1 in TEQ)
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_TEQ_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_TEQ_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03400000: // 0000 0011 0100 0000 0000 0000 0000 0000
#if CONFIG_ARCH_V1 || CONFIG_ARCH_V2 || CONFIG_ARCH_V3
      // :ARMARM: Wei 2004-Jan-29:
      //
      // UNPREDICTABLE prior to ARM architecture version 4.
      assert(!"Should not reach here.");
      break;
#elif CONFIG_ARCH_V4 || CONFIG_ARCH_V5
      // :ARMARM: Wei 2004-Jan-23:
      //
      // In ARM architecture version 4 and above, if the decode bits of
      // an instruction are not equal to those of any defined instruction,
      // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
        break;
      }
      break;
#endif
      
#endif // CONFIG_ARCH_V4 || CONFIG_ARCH_V5
        
    case 0x03500000: // 0000 0011 0101 0000 0000 0000 0000 0000
      // CMP: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1010: CMP
      //      S[20] = 1: Update CPSR (S[20] is always 1 in CMP)
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CMP_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CMP_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03600000: // 0000 0011 0110 0000 0000 0000 0000 0000
      // MSR: R[22] = 1: Update SPSR
      //      Immediate operand
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_MSR_imm_spsr(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_MSR_imm_spsr_arglist);
        break;
      }
      break;
#endif
        
    case 0x03700000: // 0000 0011 0111 0000 0000 0000 0000 0000
      // CMN: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1011: CMN
      //      S[20] = 1: Update CPSR (S[20] is always 1 in CMN)
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CMN_imm(inst);
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CMN_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03800000: // 0000 0011 1000 0000 0000 0000 0000 0000
      // ORR: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1100: ORR
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_ORR_imm(inst);
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_ORR_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03900000: // 0000 0011 1001 0000 0000 0000 0000 0000
      // ORRS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 1100: ORR
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_ORRS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_ORRS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03A00000: // 0000 0011 1010 0000 0000 0000 0000 0000
      // MOV: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1101: MOV
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_MOV_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_MOV_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03B00000: // 0000 0011 1011 0000 0000 0000 0000 0000
      // MOVS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 1101: MOV
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_MOVS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_MOVS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03C00000: // 0000 0011 1100 0000 0000 0000 0000 0000
      // BIC: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1110: BIC
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_BIC_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_BIC_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03D00000: // 0000 0011 1101 0000 0000 0000 0000 0000
      // BICS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 1110: BIC
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_BICS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_BICS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03E00000: // 0000 0011 1110 0000 0000 0000 0000 0000
      // MVN: I[25] = 1: immediate form of shifter_operand
      //      opcode[24:21] = 1111: MVN
      //      S[20] = 0: No update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_MVN_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_MVN_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x03F00000: // 0000 0011 1111 0000 0000 0000 0000 0000
      // MVNS: I[25] = 1: immediate form of shifter_operand
      //       opcode[24:21] = 1111: MVN
      //       S[20] = 1: Update CPSR
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_MVNS_imm(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_MVNS_imm_arglist);
        break;
      }
      break;
#endif
        
    case 0x04000000: // 0000 0100 0000 0000 0000 0000 0000 0000
      // STR: I[25] = 0: Immediate offset
      //      P[24] = 0: post-indexed addressing
      //      U[23] = 0: offset is subtracted from the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STR_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STR_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04100000: // 0000 0100 0001 0000 0000 0000 0000 0000
      // LDR: I[25] = 0: Immediate offset
      //      P[24] = 0: post-indexed addressing
      //      U[23] = 0: offset is subtracted from the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDR_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDR_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04200000: // 0000 0100 0010 0000 0000 0000 0000 0000
      // STRT: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 0: offset is subtracted from the base
      //       B[22] = 0: word access
      //       W[21] = 1: user mode memory access
      //       L[20] = 0: store
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRT_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRT_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04300000: // 0000 0100 0011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // LDRT: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 0: offset is subtracted from the base
      //       B[22] = 0: word access
      //       W[21] = 1: user mode memory access
      //       L[20] = 1: load
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRT_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRT_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04400000: // 0000 0100 0100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // STRB: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 0: offset is subtracted from the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access
      //       L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRB_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRB_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04500000: // 0000 0100 0101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // LDRB: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 0: offset is subtracted from the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access
      //       L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRB_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04600000: // 0000 0100 0110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // STRBT: I[25] = 0: Immediate offset
      //        P[24] = 0: post-indexed addressing
      //        U[23] = 0: offset is subtracted from the base
      //        B[22] = 1: byte access
      //        W[21] = 1: user mode memory access
      //        L[20] = 0: store
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRBT_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRBT_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04700000: // 0000 0100 0111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // LDRBT: I[25] = 0: Immediate offset
      //        P[24] = 0: post-indexed addressing
      //        U[23] = 0: offset is subtracted from the base
      //        B[22] = 1: byte access
      //        W[21] = 1: user mode memory access
      //        L[20] = 1: load
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRBT_imm_post_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRBT_imm_post_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x04800000: // 0000 0100 1000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // STR: I[25] = 0: Immediate offset
      //      P[24] = 0: post-indexed addressing
      //      U[23] = 1: offset is added to the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STR_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STR_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x04900000: // 0000 0100 1001 0000 0000 0000 0000 0000
      // LDR: I[25] = 0: Immediate offset
      //      P[24] = 0: post-indexed addressing
      //      U[23] = 1: offset is added to the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDR_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDR_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x04A00000: // 0000 0100 1010 0000 0000 0000 0000 0000
      // STRT: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 1: offset is added from the base
      //       B[22] = 0: word access
      //       W[21] = 1: user mode memory access
      //       L[20] = 0: store
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRT_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRT_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x04B00000: // 0000 0100 1011 0000 0000 0000 0000 0000
      // LDRT: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 1: offset is added from the base
      //       B[22] = 0: word access
      //       W[21] = 1: user mode memory access
      //       L[20] = 1: load
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRT_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRT_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x04C00000: // 0000 0100 1100 0000 0000 0000 0000 0000
      // STRB: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 1: offset is added from the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access
      //       L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRB_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRB_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x04D00000: // 0000 0100 1101 0000 0000 0000 0000 0000
      // LDRB: I[25] = 0: Immediate offset
      //       P[24] = 0: post-indexed addressing
      //       U[23] = 1: offset is added from the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access
      //       L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRB_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x04E00000: // 0000 0100 1110 0000 0000 0000 0000 0000
      // STRBT: I[25] = 0: Immediate offset
      //        P[24] = 0: post-indexed addressing
      //        U[23] = 1: offset is added from the base
      //        B[22] = 1: byte access
      //        W[21] = 1: user mode memory access
      //        L[20] = 0: store
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRBT_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRBT_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x04F00000: // 0000 0100 1111 0000 0000 0000 0000 0000
      // LDRBT: I[25] = 0: Immediate offset
      //        P[24] = 0: post-indexed addressing
      //        U[23] = 1: offset is added from the base
      //        B[22] = 1: byte access
      //        W[21] = 1: user mode memory access
      //        L[20] = 1: load
        
      // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
      //
      // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
      // They use a minor modification of the normal post-indexed bit pattern,
      // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRBT_imm_post_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRBT_imm_post_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x05000000: // 0000 0101 0000 0000 0000 0000 0000 0000
      // STR: I[25] = 0: Immediate offset
      //      P[24] = 1:
      //      U[23] = 0: offset is subtracted to the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access, offset addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STR_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STR_imm_offset_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05100000: // 0000 0101 0001 0000 0000 0000 0000 0000
      // LDR: I[25] = 0: Immediate offset
      //      P[24] = 1:
      //      U[23] = 0: offset is subtracted to the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access, offset addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDR_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDR_imm_offset_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05200000: // 0000 0101 0010 0000 0000 0000 0000 0000
      // STR: I[25] = 0: Immediate offset
      //      P[24] = 1:
      //      U[23] = 0: offset is subtracted to the base
      //      B[22] = 0: word access
      //      W[21] = 1: normal memory access, pre-indexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STR_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STR_imm_pre_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05300000: // 0000 0101 0011 0000 0000 0000 0000 0000
      // LDR: I[25] = 0: Immediate offset
      //      P[24] = 1:
      //      U[23] = 0: offset is subtracted to the base
      //      B[22] = 0: word access
      //      W[21] = 1: normal memory access, pre-indexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDR_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDR_imm_pre_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05400000: // 0000 0101 0100 0000 0000 0000 0000 0000
      // STRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 0: offset is subtracted to the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access, offset addressing
      //       L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRB_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRB_imm_offset_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05500000: // 0000 0101 0101 0000 0000 0000 0000 0000
      // LDRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 0: offset is subtracted to the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access, offset addressing
      //       L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRB_imm_offset_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_imm_offset_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05600000: // 0000 0101 0110 0000 0000 0000 0000 0000
      // STRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 0: offset is subtracted to the base
      //       B[22] = 1: byte access
      //       W[21] = 1: normal memory access, pre-indexed addressing
      //       L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRB_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRB_imm_pre_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05700000: // 0000 0101 0111 0000 0000 0000 0000 0000
      // LDRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 0: offset is subtracted to the base
      //       B[22] = 1: byte access
      //       W[21] = 1: normal memory access, pre-indexed addressing
      //       L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRB_imm_pre_down(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_imm_pre_down_arglist);
        break;
      }
      break;
#endif
        
    case 0x05800000: // 0000 0101 1000 0000 0000 0000 0000 0000
      // STR: I[25] = 0: Immediate offset
      //      P[24] = 1: 
      //      U[23] = 1: offset is added from the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access, offset addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STR_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STR_imm_offset_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x05900000: // 0000 0101 1001 0000 0000 0000 0000 0000
      // LDR: I[25] = 0: Immediate offset
      //      P[24] = 1: 
      //      U[23] = 1: offset is added from the base
      //      B[22] = 0: word access
      //      W[21] = 0: normal memory access, offset addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDR_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDR_imm_offset_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x05A00000: // 0000 0101 1010 0000 0000 0000 0000 0000
      // STR: I[25] = 0: Immediate offset
      //      P[24] = 1: 
      //      U[23] = 1: offset is added from the base
      //      B[22] = 0: word access
      //      W[21] = 1: normal memory access, pre-indexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STR_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STR_imm_pre_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x05B00000: // 0000 0101 1011 0000 0000 0000 0000 0000
      // LDR: I[25] = 0: Immediate offset
      //      P[24] = 1: 
      //      U[23] = 1: offset is added from the base
      //      B[22] = 0: word access
      //      W[21] = 1: normal memory access, pre-indexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDR_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDR_imm_pre_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x05C00000: // 0000 0101 1100 0000 0000 0000 0000 0000
      // STRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 1: offset is added from the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access, offset addressing
      //       L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRB_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRB_imm_offset_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x05D00000: // 0000 0101 1101 0000 0000 0000 0000 0000
      // LDRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 1: offset is added from the base
      //       B[22] = 1: byte access
      //       W[21] = 0: normal memory access, offset addressing
      //       L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRB_imm_offset_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_imm_offset_up_arglist);
        break;
      }
      break;
#endif
      
    case 0x05E00000: // 0000 0101 1110 0000 0000 0000 0000 0000
      // STRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 1: offset is added from the base
      //       B[22] = 1: byte access
      //       W[21] = 1: normal memory access, pre-indexed addressing
      //       L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STRB_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STRB_imm_pre_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x05F00000: // 0000 0101 1111 0000 0000 0000 0000 0000
      // LDRB: I[25] = 0: Immediate offset
      //       P[24] = 1:
      //       U[23] = 1: offset is added from the base
      //       B[22] = 1: byte access
      //       W[21] = 1: normal memory access, pre-indexed addressing
      //       L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDRB_imm_pre_up(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_imm_pre_up_arglist);
        break;
      }
      break;
#endif
        
    case 0x06000000: // 0000 0110 0000 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STR: I[25] = 1: register offset
        //      P[24] = 0: post-indexed addressing
        //      U[23] = 0: offset is subtracted from the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access
        //      L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STR_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STR_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06100000: // 0000 0110 0001 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDR: I[25] = 1: register offset
        //      P[24] = 0: post-indexed addressing
        //      U[23] = 0: offset is subtracted from the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access
        //      L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDR_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDR_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06200000: // 0000 0110 0010 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRT: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       B[22] = 0: word access
        //       W[21] = 1: user mode memory access
        //       L[20] = 0: store
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRT_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRT_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06300000: // 0000 0110 0011 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRT: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       B[22] = 0: word access
        //       W[21] = 1: user mode memory access
        //       L[20] = 1: load
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRT_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRT_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06400000: // 0000 0110 0100 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRB: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRB_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRB_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06500000: // 0000 0110 0101 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRB: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 0: offset is subtracted from the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRB_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06600000: // 0000 0110 0110 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRBT: I[25] = 1: register offset
        //        P[24] = 0: post-indexed addressing
        //        U[23] = 0: offset is subtracted from the base
        //        B[22] = 1: byte access
        //        W[21] = 1: user mode memory access
        //        L[20] = 0: store
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRBT_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRBT_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06700000: // 0000 0110 0111 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRBT: I[25] = 1: register offset
        //        P[24] = 0: post-indexed addressing
        //        U[23] = 0: offset is subtracted from the base
        //        B[22] = 1: byte access
        //        W[21] = 1: user mode memory access
        //        L[20] = 1: load
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRBT_reg_post_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRBT_reg_post_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06800000: // 0000 0110 1000 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STR: I[25] = 1: register offset
        //      P[24] = 0: post-indexed addressing
        //      U[23] = 1: offset is added to the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access
        //      L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STR_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STR_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06900000: // 0000 0110 1001 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDR: I[25] = 1: register offset
        //      P[24] = 0: post-indexed addressing
        //      U[23] = 1: offset is added to the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access
        //      L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDR_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDR_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06A00000: // 0000 0110 1010 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRT: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added from the base
        //       B[22] = 0: word access
        //       W[21] = 1: user mode memory access
        //       L[20] = 0: store
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRT_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRT_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06B00000: // 0000 0110 1011 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRT: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added from the base
        //       B[22] = 0: word access
        //       W[21] = 1: user mode memory access
        //       L[20] = 1: load
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRT_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRT_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06C00000: // 0000 0110 1100 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRB: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added from the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRB_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRB_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06D00000: // 0000 0110 1101 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRB: I[25] = 1: register offset
        //       P[24] = 0: post-indexed addressing
        //       U[23] = 1: offset is added from the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRB_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06E00000: // 0000 0110 1110 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRBT: I[25] = 1: register offset
        //        P[24] = 0: post-indexed addressing
        //        U[23] = 1: offset is added from the base
        //        B[22] = 1: byte access
        //        W[21] = 1: user mode memory access
        //        L[20] = 0: store
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRBT_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRBT_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x06F00000: // 0000 0110 1111 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRBT: I[25] = 1: register offset
        //        P[24] = 0: post-indexed addressing
        //        U[23] = 1: offset is added from the base
        //        B[22] = 1: byte access
        //        W[21] = 1: user mode memory access
        //        L[20] = 1: load
          
        // :ARM-DDI-0100E: p.A5-29: Wei 2004-Feb-07:
        //
        // LDRBT, LDRT, STRBT, and STRT only support post-indexed addressing modes.
        // They use a minor modification of the normal post-indexed bit pattern,
        // where W[21] is 1, not 0.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRBT_reg_post_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRBT_reg_post_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07000000: // 0000 0111 0000 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STR: I[25] = 1: register offset
        //      P[24] = 1:
        //      U[23] = 0: offset is subtracted to the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access, offset addressing
        //      L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STR_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STR_reg_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07100000: // 0000 0111 0001 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDR: I[25] = 1: register offset
        //      P[24] = 1:
        //      U[23] = 0: offset is subtracted to the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access, offset addressing
        //      L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDR_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDR_reg_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07200000: // 0000 0111 0010 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STR: I[25] = 1: register offset
        //      P[24] = 1:
        //      U[23] = 0: offset is subtracted to the base
        //      B[22] = 0: word access
        //      W[21] = 1: normal memory access, pre-indexed addressing
        //      L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STR_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STR_reg_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07300000: // 0000 0111 0011 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDR: I[25] = 1: register offset
        //      P[24] = 1:
        //      U[23] = 0: offset is subtracted to the base
        //      B[22] = 0: word access
        //      W[21] = 1: normal memory access, pre-indexed addressing
        //      L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDR_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDR_reg_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07400000: // 0000 0111 0100 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 0: offset is subtracted to the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access, offset addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRB_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRB_reg_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07500000: // 0000 0111 0101 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 0: offset is subtracted to the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access, offset addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRB_reg_offset_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_reg_offset_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07600000: // 0000 0111 0110 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 0: offset is subtracted to the base
        //       B[22] = 1: byte access
        //       W[21] = 1: normal memory access, pre-indexed addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRB_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRB_reg_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07700000: // 0000 0111 0111 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 0: offset is subtracted to the base
        //       B[22] = 1: byte access
        //       W[21] = 1: normal memory access, pre-indexed addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRB_reg_pre_down(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_reg_pre_down_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07800000: // 0000 0111 1000 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STR: I[25] = 1: register offset
        //      P[24] = 1: 
        //      U[23] = 1: offset is added from the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access, offset addressing
        //      L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STR_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STR_reg_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07900000: // 0000 0111 1001 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDR: I[25] = 1: register offset
        //      P[24] = 1: 
        //      U[23] = 1: offset is added from the base
        //      B[22] = 0: word access
        //      W[21] = 0: normal memory access, offset addressing
        //      L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDR_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDR_reg_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07A00000: // 0000 0111 1010 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STR: I[25] = 1: register offset
        //      P[24] = 1: 
        //      U[23] = 1: offset is added from the base
        //      B[22] = 0: word access
        //      W[21] = 1: normal memory access, pre-indexed addressing
        //      L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STR_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STR_reg_pre_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07B00000: // 0000 0111 1011 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDR: I[25] = 1: register offset
        //      P[24] = 1: 
        //      U[23] = 1: offset is added from the base
        //      B[22] = 0: word access
        //      W[21] = 1: normal memory access, pre-indexed addressing
        //      L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDR_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDR_reg_pre_up_arglist);
          break;
        }
        break;
#endif

      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07C00000: // 0000 0111 1100 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 1: offset is added from the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access, offset addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRB_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRB_reg_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07D00000: // 0000 0111 1101 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 1: offset is added from the base
        //       B[22] = 1: byte access
        //       W[21] = 0: normal memory access, offset addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRB_reg_offset_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_reg_offset_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07E00000: // 0000 0111 1110 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // STRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 1: offset is added from the base
        //       B[22] = 1: byte access
        //       W[21] = 1: normal memory access, pre-indexed addressing
        //       L[20] = 0: store
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_STRB_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_STRB_reg_pre_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x07F00000: // 0000 0111 1111 0000 0000 0000 0000 0000
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // LDRB: I[25] = 1: register offset
        //       P[24] = 1:
        //       U[23] = 1: offset is added from the base
        //       B[22] = 1: byte access
        //       W[21] = 1: normal memory access, pre-indexed addressing
        //       L[20] = 1: load
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_LDRB_reg_pre_up(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_LDRB_reg_pre_up_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // :ARMARM: Wei 2004-Jan-23:
        //
        // In ARM architecture version 4 and above, if the decode bits of
        // an instruction are not equal to those of any defined instruction,
        // then the instruction is UNDEFINED.
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_UND(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x08000000: // 0000 1000 0000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMDA: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_DA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_DA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08100000: // 0000 1000 0001 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMDA: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_DA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_DA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08200000: // 0000 1000 0010 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMDA: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_DA_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_DA_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x08300000: // 0000 1000 0011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMDA: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_DA_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_DA_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x08400000: // 0000 1000 0100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_2)
      // STMDA: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_2_DA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_2_DA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08500000: // 0000 1000 0101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_2 or LDM_3)
      // LDMDA: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_DA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_DA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08600000: // 0000 1000 0110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-09:
      //
      // This should be STM(2)
      // However...
      //
      // :ARM-DDI-0100E: Wei 2004-Feb-09:
      //
      // Setting bit 21 (the W bit) has UNPREDICTABLE results.
#if CHECK_UNPREDICTABLE
      assert(!"Should not reach here.");
#endif
      break;
        
    case 0x08700000: // 0000 1000 0111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-10:
      //
      // (LDM_3)
      // LDMDA: S[22] = 1: user mode register bank
      //        W[21] = 1: write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_DA_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_DA_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x08800000: // 0000 1000 1000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMIA: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_IA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_IA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08900000: // 0000 1000 1001 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMIA: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_IA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_IA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08A00000: // 0000 1000 1010 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMIA: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_IA_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_IA_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x08B00000: // 0000 1000 1011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMIA: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_IA_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_IA_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x08C00000: // 0000 1000 1100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_2)
      // STMIA: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_2_IA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_2_IA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08D00000: // 0000 1000 1101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_2 or LDM_3)
      // LDMIA: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_IA(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_IA_arglist);
        break;
      }
      break;
#endif
        
    case 0x08E00000: // 0000 1000 1110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-09:
      //
      // This should be STM(2)
      // However...
      //
      // :ARM-DDI-0100E: Wei 2004-Feb-09:
      //
      // Setting bit 21 (the W bit) has UNPREDICTABLE results.
#if CHECK_UNPREDICTABLE
      assert(!"Should not reach here.");
#endif
      break;
        
    case 0x08F00000: // 0000 1000 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-10:
      //
      // (LDM_3)
      // LDMIA: S[22] = 1: user mode register bank
      //        W[21] = 1: write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_IA_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_IA_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x09000000: // 0000 1001 0000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMDB: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_DB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_DB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09100000: // 0000 1001 0001 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMDB: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_DB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_DB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09200000: // 0000 1001 0010 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMDB: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_DB_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_DB_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x09300000: // 0000 1001 0011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMDB: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_DB_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_DB_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x09400000: // 0000 1001 0100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_2)
      // STMDB: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_2_DB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_2_DB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09500000: // 0000 1001 0101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_2 or LDM_3)
      // LDMDB: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_DB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_DB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09600000: // 0000 1001 0110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-09:
      //
      // This should be STM(2)
      // However...
      //
      // :ARM-DDI-0100E: Wei 2004-Feb-09:
      //
      // Setting bit 21 (the W bit) has UNPREDICTABLE results.
#if CHECK_UNPREDICTABLE
      assert(!"Should not reach here.");
#endif
      break;
        
    case 0x09700000: // 0000 1001 0111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-10:
      //
      // (LDM_3)
      // LDMDB: S[22] = 1: user mode register bank
      //        W[21] = 1: write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_DB_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_DB_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x09800000: // 0000 1001 1000 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMIB: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_IB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_IB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09900000: // 0000 1001 1001 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMIB: S[22] = 0: current register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_IB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_IB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09A00000: // 0000 1001 1010 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_1)
      // STMIB: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_1_IB_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_1_IB_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x09B00000: // 0000 1001 1011 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_1)
      // LDMIB: S[22] = 0: current register bank
      //        W[21] = 1: Write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_1_IB_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_1_IB_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x09C00000: // 0000 1001 1100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (STM_2)
      // STMIB: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_STM_2_IB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_STM_2_IB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09D00000: // 0000 1001 1101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // (LDM_2 or LDM_3)
      // LDMIB: S[22] = 1: user mode register bank
      //        W[21] = 0: No write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_IB(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_IB_arglist);
        break;
      }
      break;
#endif
        
    case 0x09E00000: // 0000 1001 1110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-09:
      //
      // This should be STM(2)
      // However...
      //
      // :ARM-DDI-0100E: Wei 2004-Feb-09:
      //
      // Setting bit 21 (the W bit) has UNPREDICTABLE results.
#if CHECK_UNPREDICTABLE
      assert(!"Should not reach here.");
#endif
      break;
        
    case 0x09F00000: // 0000 1001 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Feb-10:
      //
      // (LDM_3)
      // LDMIB: S[22] = 1: user mode register bank
      //        W[21] = 1: write back to base register
      //        L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_LDM_23_IB_w(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_LDM_23_IB_w_arglist);
        break;
      }
      break;
#endif
        
    case 0x0A000000: // 0000 1010 0000 0000 0000 0000 0000 0000
    case 0x0A100000: // 0000 1010 0001 0000 0000 0000 0000 0000
    case 0x0A200000: // 0000 1010 0010 0000 0000 0000 0000 0000
    case 0x0A300000: // 0000 1010 0011 0000 0000 0000 0000 0000
    case 0x0A400000: // 0000 1010 0100 0000 0000 0000 0000 0000
    case 0x0A500000: // 0000 1010 0101 0000 0000 0000 0000 0000
    case 0x0A600000: // 0000 1010 0110 0000 0000 0000 0000 0000
    case 0x0A700000: // 0000 1010 0111 0000 0000 0000 0000 0000
    case 0x0A800000: // 0000 1010 1000 0000 0000 0000 0000 0000
    case 0x0A900000: // 0000 1010 1001 0000 0000 0000 0000 0000
    case 0x0AA00000: // 0000 1010 1010 0000 0000 0000 0000 0000
    case 0x0AB00000: // 0000 1010 1011 0000 0000 0000 0000 0000
    case 0x0AC00000: // 0000 1010 1100 0000 0000 0000 0000 0000
    case 0x0AD00000: // 0000 1010 1101 0000 0000 0000 0000 0000
    case 0x0AE00000: // 0000 1010 1110 0000 0000 0000 0000 0000
    case 0x0AF00000: // 0000 1010 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // B: L[24] = 0: simply branches without storing a return address
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
#if CONFIG_ARCH_V5
        switch (inst.mRawData & 0xF0000000)
        {
        case NV:
          return exec_BLX_1(inst);
          
        default:
#endif
          return exec_B(inst);
#if CONFIG_ARCH_V5
        }
#endif
        
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
#if CONFIG_ARCH_V5
        switch (inst.mRawData & 0xF0000000)
        {
        case NV:
          mp_curr_chunk->tc_buffer_append(inst, &g_BLX_1_arglist);
          break;
          
        default:
#endif
          mp_curr_chunk->tc_buffer_append(inst, &g_B_arglist);
          break;
#if CONFIG_ARCH_V5
        }
#endif
      }
      break;
#endif
      
    case 0x0B000000: // 0000 1011 0000 0000 0000 0000 0000 0000
    case 0x0B100000: // 0000 1011 0001 0000 0000 0000 0000 0000
    case 0x0B200000: // 0000 1011 0010 0000 0000 0000 0000 0000
    case 0x0B300000: // 0000 1011 0011 0000 0000 0000 0000 0000
    case 0x0B400000: // 0000 1011 0100 0000 0000 0000 0000 0000
    case 0x0B500000: // 0000 1011 0101 0000 0000 0000 0000 0000
    case 0x0B600000: // 0000 1011 0110 0000 0000 0000 0000 0000
    case 0x0B700000: // 0000 1011 0111 0000 0000 0000 0000 0000
    case 0x0B800000: // 0000 1011 1000 0000 0000 0000 0000 0000
    case 0x0B900000: // 0000 1011 1001 0000 0000 0000 0000 0000
    case 0x0BA00000: // 0000 1011 1010 0000 0000 0000 0000 0000
    case 0x0BB00000: // 0000 1011 1011 0000 0000 0000 0000 0000
    case 0x0BC00000: // 0000 1011 1100 0000 0000 0000 0000 0000
    case 0x0BD00000: // 0000 1011 1101 0000 0000 0000 0000 0000
    case 0x0BE00000: // 0000 1011 1110 0000 0000 0000 0000 0000
    case 0x0BF00000: // 0000 1011 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-20:
      //
      // BL: L[24] = 1: stores a return address in the link register
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
#if CONFIG_ARCH_V5
        switch (inst.mRawData & 0xF0000000)
        {
        case NV:
          return exec_BLX_1(inst);
          
        default:
#endif
          return exec_BL(inst);
#if CONFIG_ARCH_V5
        }
#endif
        
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
#if CONFIG_ARCH_V5
        switch (inst.mRawData & 0xF0000000)
        {
        case NV:
          mp_curr_chunk->tc_buffer_append(inst, &g_BLX_1_arglist);
          break;
          
        default:
#endif
          mp_curr_chunk->tc_buffer_append(inst, &g_BL_arglist);
          break;
#if CONFIG_ARCH_V5
        }
#endif
      }
      break;
#endif
      
    case 0x0C000000: // 0000 1100 0000 0000 0000 0000 0000 0000
    case 0x0C400000: // 0000 1100 0100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 0:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 0: unindexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
      
    case 0x0C100000: // 0000 1100 0001 0000 0000 0000 0000 0000
    case 0x0C500000: // 0000 1100 0101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // LDC: P[24] = 0:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 0: unindexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0C200000: // 0000 1100 0010 0000 0000 0000 0000 0000
    case 0x0C600000: // 0000 1100 0110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 0:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 1: post-indexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0C300000: // 0000 1100 0011 0000 0000 0000 0000 0000
    case 0x0C700000: // 0000 1100 0111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // LDC: P[24] = 0:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 1: post-indexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0C800000: // 0000 1100 1000 0000 0000 0000 0000 0000
    case 0x0CC00000: // 0000 1100 1100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 0:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 0: unindexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0C900000: // 0000 1100 1001 0000 0000 0000 0000 0000
    case 0x0CD00000: // 0000 1100 1101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // LDC: P[24] = 0:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 0: unindexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0CA00000: // 0000 1100 1010 0000 0000 0000 0000 0000
    case 0x0CE00000: // 0000 1100 1110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 0:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 1: post-indexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0CB00000: // 0000 1100 1011 0000 0000 0000 0000 0000
    case 0x0CF00000: // 0000 1100 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // LDC: P[24] = 0:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 1: post-indexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0D000000: // 0000 1101 0000 0000 0000 0000 0000 0000
    case 0x0D400000: // 0000 1101 0100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 1:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 0: offset addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0D100000: // 0000 1101 0001 0000 0000 0000 0000 0000
    case 0x0D500000: // 0000 1101 0101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // LDC: P[24] = 1:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 0: offset addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0D200000: // 0000 1101 0010 0000 0000 0000 0000 0000
    case 0x0D600000: // 0000 1101 0110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 1:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 1: pre-indexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0D300000: // 0000 1101 0011 0000 0000 0000 0000 0000
    case 0x0D700000: // 0000 1101 0111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // LDC: P[24] = 1:
      //      U[23] = 0: the offset is subtracted from the base
      //      W[21] = 1: pre-indexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0D800000: // 0000 1101 1000 0000 0000 0000 0000 0000
    case 0x0DC00000: // 0000 1101 1100 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 1:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 0: offset addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0D900000: // 0000 1101 1001 0000 0000 0000 0000 0000
    case 0x0DD00000: // 0000 1101 1101 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 1:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 0: offset addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0DA00000: // 0000 1101 1010 0000 0000 0000 0000 0000
    case 0x0DE00000: // 0000 1101 1110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // STC: P[24] = 1:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 1: pre-indexed addressing
      //      L[20] = 0: store
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
        
    case 0x0DB00000: // 0000 1101 1011 0000 0000 0000 0000 0000
    case 0x0DF00000: // 0000 1101 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // LDC: P[24] = 1:
      //      U[23] = 1: the offset is added to the base
      //      W[21] = 1: pre-indexed addressing
      //      L[20] = 1: load
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_CDT(inst);
#if ENABLE_THREADED_CODE
          
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_CDT_arglist);
        break;
      }
      break;
#endif
      
    case 0x0E000000: // 0000 1110 0000 0000 0000 0000 0000 0000
    case 0x0E200000: // 0000 1110 0010 0000 0000 0000 0000 0000
    case 0x0E400000: // 0000 1110 0100 0000 0000 0000 0000 0000
    case 0x0E600000: // 0000 1110 0110 0000 0000 0000 0000 0000
    case 0x0E800000: // 0000 1110 1000 0000 0000 0000 0000 0000
    case 0x0EA00000: // 0000 1110 1010 0000 0000 0000 0000 0000
    case 0x0EC00000: // 0000 1110 1100 0000 0000 0000 0000 0000
    case 0x0EE00000: // 0000 1110 1110 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // MRC:
      //
      // CDP:
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // CDP:
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CDP(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CDP_arglist);
          break;
        }
        break;
#endif
        
      case (1 << 4):
        // MCR:
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CRT<STORE>(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CRT_STORE_arglist);
          break;
        }
        break;
#endif
      }
      break;
        
    case 0x0E100000: // 0000 1110 0000 0000 0000 0000 0000 0000
    case 0x0E300000: // 0000 1110 0011 0000 0000 0000 0000 0000
    case 0x0E500000: // 0000 1110 0101 0000 0000 0000 0000 0000
    case 0x0E700000: // 0000 1110 0111 0000 0000 0000 0000 0000
    case 0x0E900000: // 0000 1110 1001 0000 0000 0000 0000 0000
    case 0x0EB00000: // 0000 1110 1011 0000 0000 0000 0000 0000
    case 0x0ED00000: // 0000 1110 1101 0000 0000 0000 0000 0000
    case 0x0EF00000: // 0000 1110 1111 0000 0000 0000 0000 0000
      // :NOTE: Wei 2004-Jan-21:
      //
      // MCR:
      //
      // CDP:
      switch (inst.mRawData & (1 << 4))
      {
      case 0:
        // CDP:
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CDP(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CDP_arglist);
          break;
        }
        break;
#endif
          
      case (1 << 4):
        // MRC:
#if ENABLE_THREADED_CODE
        switch (T_usage)
        {
        case USAGE_NORMAL:
#endif
          return exec_CRT<LOAD>(inst);
#if ENABLE_THREADED_CODE
          
        case USAGE_THREADED_CODE:
          mp_curr_chunk->tc_buffer_append(inst, &g_CRT_LOAD_arglist);
          break;
        }
        break;
#endif
      }
      break;
      
    case 0x0F000000: // 1111 0000 0000 0000 0000 0000 0000
    case 0x0F100000: // 1111 0001 0000 0000 0000 0000 0000
    case 0x0F200000: // 1111 0010 0000 0000 0000 0000 0000
    case 0x0F300000: // 1111 0011 0000 0000 0000 0000 0000
    case 0x0F400000: // 1111 0100 0000 0000 0000 0000 0000
    case 0x0F500000: // 1111 0101 0000 0000 0000 0000 0000
    case 0x0F600000: // 1111 0110 0000 0000 0000 0000 0000
    case 0x0F700000: // 1111 0111 0000 0000 0000 0000 0000
    case 0x0F800000: // 1111 1000 0000 0000 0000 0000 0000
    case 0x0F900000: // 1111 1001 0000 0000 0000 0000 0000
    case 0x0FA00000: // 1111 1010 0000 0000 0000 0000 0000
    case 0x0FB00000: // 1111 1011 0000 0000 0000 0000 0000
    case 0x0FC00000: // 1111 1100 0000 0000 0000 0000 0000
    case 0x0FD00000: // 1111 1101 0000 0000 0000 0000 0000
    case 0x0FE00000: // 1111 1110 0000 0000 0000 0000 0000
    case 0x0FF00000: // 1111 1111 0000 0000 0000 0000 0000
      // SWI:
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
        // Non-sequential next PC
        finalize_chunk();
#endif
        return exec_SWI(inst);
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_SWI_arglist);
        break;
      }
      break;
#endif
      
    default:
#if ENABLE_THREADED_CODE
      switch (T_usage)
      {
      case USAGE_NORMAL:
#endif
        return exec_UND(inst);
#if ENABLE_THREADED_CODE
        
      case USAGE_THREADED_CODE:
        mp_curr_chunk->tc_buffer_append(inst, &g_UND_arglist);
        break;
      }
      break;
#endif
    }
    
#if ENABLE_THREADED_CODE
    switch (T_usage)
    {
    case USAGE_NORMAL:
      // :NOTE: Wei 2004-Aug-10:
      //
      // When executing each instruction, 
      assert(!"Should not reach here.");
      break;
      
    case USAGE_THREADED_CODE:
      return ER_NORMAL;
    }
#endif
    
    assert(!"Should not reach here.");
    return ER_NORMAL;
  }
}
