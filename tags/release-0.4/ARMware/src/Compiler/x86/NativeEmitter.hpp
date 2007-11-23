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

#ifndef NativeEmitter_hpp
#define NativeEmitter_hpp

#if ENABLE_DYNAMIC_TRANSLATOR

#include "../Label.hpp"

namespace ARMware
{
  typedef struct NativeOperTraits_ADD NativeOperTraits_ADD;
  struct NativeOperTraits_ADD
  {
    static uint8_t const R_R_to_R = 0x01;
    static uint8_t const R_M_to_M = 0x01;
    static uint8_t const M_R_to_R = 0x03;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x00;
  };
  
  typedef struct NativeOperTraits_ADC NativeOperTraits_ADC;
  struct NativeOperTraits_ADC
  {
    static uint8_t const R_R_to_R = 0x11;
    static uint8_t const R_M_to_M = 0x11;
    static uint8_t const M_R_to_R = 0x13;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x02;
  };
  
  typedef struct NativeOperTraits_AND NativeOperTraits_AND;
  struct NativeOperTraits_AND
  {
    static uint8_t const R_R_to_R = 0x21;
    static uint8_t const R_M_to_M = 0x21;
    static uint8_t const M_R_to_R = 0x23;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x04;
  };
  
  typedef struct NativeOperTraits_OR NativeOperTraits_OR;
  struct NativeOperTraits_OR
  {
    static uint8_t const R_R_to_R = 0x09;
    static uint8_t const R_M_to_M = 0x09;
    static uint8_t const M_R_to_R = 0x0B;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x01;
  };
  
  typedef struct NativeOperTraits_EOR NativeOperTraits_EOR;
  struct NativeOperTraits_EOR
  {
    static uint8_t const R_R_to_R = 0x31;
    static uint8_t const R_M_to_M = 0x31;
    static uint8_t const M_R_to_R = 0x33;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x06;
  };
  
  typedef struct NativeOperTraits_SUB NativeOperTraits_SUB;
  struct NativeOperTraits_SUB
  {
    static uint8_t const R_R_to_R = 0x29;
    static uint8_t const R_M_to_M = 0x29;
    static uint8_t const M_R_to_R = 0x2B;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x05;
  };
  
  typedef struct NativeOperTraits_SBC NativeOperTraits_SBC;
  struct NativeOperTraits_SBC
  {
    static uint8_t const R_R_to_R = 0x19;
    static uint8_t const R_M_to_M = 0x19;
    static uint8_t const M_R_to_R = 0x1B;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x03;
  };
  
  typedef struct NativeOperTraits_CMP NativeOperTraits_CMP;
  struct NativeOperTraits_CMP
  {
    static uint8_t const R_R_to_R = 0x39;
    static uint8_t const R_M_to_M = 0x39;
    static uint8_t const M_R_to_R = 0x3B;
    
    static uint8_t const I_MR_to_MR = 0x81;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x07;
  };
  
  typedef struct NativeOperTraits_TST NativeOperTraits_TST;
  struct NativeOperTraits_TST
  {
    // :NOTE: Wei 2005-Mar-07:
    //
    // Because the TST operation is indeed the AND operation,
    // thus the opcodes of R_M_to_M and M_R_to_R are the same.
    static uint8_t const R_R_to_R = 0x85;
    static uint8_t const R_M_to_M = 0x85;
    static uint8_t const M_R_to_R = 0x85;
    
    static uint8_t const I_MR_to_MR = 0xF7;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x00;
  };
  
  typedef struct NativeOperTraits_LSFT_LEFT NativeOperTraits_LSFT_LEFT;
  struct NativeOperTraits_LSFT_LEFT
  {
    static uint8_t const MODRM_OPCODE_BITS = 0x04;
    
    static uint8_t const I_MR_to_MR = 0xC1;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x04;
  };
  
  typedef struct NativeOperTraits_LSFT_RIGHT NativeOperTraits_LSFT_RIGHT;
  struct NativeOperTraits_LSFT_RIGHT
  {
    static uint8_t const MODRM_OPCODE_BITS = 0x05;
    
    static uint8_t const I_MR_to_MR = 0xC1;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x05;
  };
  
  typedef struct NativeOperTraits_ASFT_RIGHT NativeOperTraits_ASFT_RIGHT;
  struct NativeOperTraits_ASFT_RIGHT
  {
    static uint8_t const MODRM_OPCODE_BITS = 0x07;
    
    static uint8_t const I_MR_to_MR = 0xC1;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x07;
  };
  
  typedef struct NativeOperTraits_ROR NativeOperTraits_ROR;
  struct NativeOperTraits_ROR
  {
    static uint8_t const MODRM_OPCODE_BITS = 0x01;
    
    static uint8_t const I_MR_to_MR = 0xC1;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x01;
  };
  
  typedef struct NativeOperTraits_RRX NativeOperTraits_RRX;
  struct NativeOperTraits_RRX
  {
    static uint8_t const MODRM_OPCODE_BITS = 0x03;
    
    static uint8_t const I_MR_to_MR = 0xC1;
    static uint8_t const I_MR_to_MR_OPCODE_2 = 0x03;
  };
  
  typedef struct NativeOperTraits_SETS NativeOperTraits_SETS;
  struct NativeOperTraits_SETS
  {
    static uint8_t const SET_BYTE_OPCODE = 0x98;
  };
  
  typedef struct NativeOperTraits_SETZ NativeOperTraits_SETZ;
  struct NativeOperTraits_SETZ
  {
    static uint8_t const SET_BYTE_OPCODE = 0x94;
  };
  
  typedef struct NativeOperTraits_SETC NativeOperTraits_SETC;
  struct NativeOperTraits_SETC
  {
    static uint8_t const SET_BYTE_OPCODE = 0x92;
  };
  
  typedef struct NativeOperTraits_SETO NativeOperTraits_SETO;
  struct NativeOperTraits_SETO
  {
    static uint8_t const SET_BYTE_OPCODE = 0x90;
  };
  
  typedef struct NativeOperTraits_MUL32 NativeOperTraits_MUL32;
  struct NativeOperTraits_MUL32
  {
    static uint8_t const OPCODE_1 = 0xF7;
    static uint8_t const OPCODE_2 = 0x04;
  };
  
  typedef struct NativeOperTraits_IMUL32 NativeOperTraits_IMUL32;
  struct NativeOperTraits_IMUL32
  {
    static uint8_t const OPCODE_1 = 0xF7;
    static uint8_t const OPCODE_2 = 0x05;
  };
  
  // :NOTE: Wei 2005-Mar-06:
  //
  // Addressing mode
  
  inline void
  addressing_mode_r_r(std::vector<uint8_t> &native_code_buffer,
                      uint8_t const operand,
                      uint8_t const dest)
  {
    native_code_buffer.push_back(0xC0 | (static_cast<uint8_t>(operand) << 3) | dest);
  }
  
  inline void
  addressing_mode_memory_r_r(std::vector<uint8_t> &native_code_buffer,
                             uint8_t const reg,
                             uint8_t const mem_reg)
  {
    native_code_buffer.push_back((static_cast<uint8_t>(reg) << 3) | mem_reg);
  }
  
  inline void
  addressing_mode_memory_sp_r(std::vector<uint8_t> &native_code_buffer,
                              uint8_t const reg)
  {
    native_code_buffer.push_back((static_cast<uint8_t>(reg) << 3) | 0x04);
    native_code_buffer.push_back(0x24);
  }
  
  inline void
  addressing_mode_stack_r(std::vector<uint8_t> &native_code_buffer,
                          uint32_t const bp_offset,
                          uint8_t const reg)
  {
    native_code_buffer.push_back(0x80 | (static_cast<uint8_t>(reg) << 3) | 0x05);
    
    // :NOTE: Wei 2005-Mar-06:
    //
    // Little endian means little end first.
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 24));
  }
  
  inline void
  addressing_mode_global_r(std::vector<uint8_t> &native_code_buffer,
                           uint32_t const addr,
                           uint8_t const reg)
  {
    native_code_buffer.push_back((static_cast<uint8_t>(reg) << 3) | 0x05);
    
    native_code_buffer.push_back(static_cast<uint8_t>(addr));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 24));
  }
  
  inline void
  addressing_mode_bit_test_stack(std::vector<uint8_t> &native_code_buffer,
                                 uint32_t const bp_offset,
                                 uint8_t const bit_position)
  {
    assert(bit_position <= 31);
    
    native_code_buffer.push_back(0x80 | (0x04 << 3) | 0x05);
    
    // :NOTE: Wei 2005-Mar-06:
    //
    // Little endian means little end first.
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 24));
    
    native_code_buffer.push_back(bit_position);
  }
  
  inline void
  addressing_mode_bit_test_global(std::vector<uint8_t> &native_code_buffer,
                                  uint32_t const addr,
                                  uint8_t const bit_position)
  {
    assert(bit_position <= 31);
    
    native_code_buffer.push_back((0x04 << 3) | 0x05);
    
    native_code_buffer.push_back(static_cast<uint8_t>(addr));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 24));
    
    native_code_buffer.push_back(bit_position);
  }
  
  inline void
  imm_field(std::vector<uint8_t> &native_code_buffer,
            uint32_t const imm)
  {
    native_code_buffer.push_back(static_cast<uint8_t>(imm));
    native_code_buffer.push_back(static_cast<uint8_t>(imm >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(imm >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(imm >> 24));
  }
  
  // :NOTE: Wei 2005-Mar-06:
  //
  // Operation for ADD_RR...etc
  
  inline void
  two_operand_operation_r_r(std::vector<uint8_t> &native_code_buffer,
                            uint8_t const opcode,
                            Register::RegNativeRep const operand,
                            Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(opcode);
    
    addressing_mode_r_r(native_code_buffer, operand, dest);
  }
  
  inline void
  two_operand_operation_stack_r(std::vector<uint8_t> &native_code_buffer,
                                uint8_t const opcode,
                                uint32_t const bp_offset,
                                Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(opcode);
    
    addressing_mode_stack_r(native_code_buffer, bp_offset, dest);
  }
  
  inline void
  two_operand_operation_global_r(std::vector<uint8_t> &native_code_buffer,
                                 uint8_t const opcode,
                                 uint32_t const addr,
                                 Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(opcode);
    
    addressing_mode_global_r(native_code_buffer, addr, dest);
  }
  
  // :NOTE: Wei 2005-Mar-06:
  //
  // Operation for ADD_RI...etc
  
  template<bool T_is_shift>
  inline void
  two_operand_operation_one_imm_final(std::vector<uint8_t> &native_code_buffer,
                                      uint32_t const imm)
  {
    if (true == T_is_shift)
    {
      native_code_buffer.push_back(static_cast<uint8_t>(imm));
    }
    else
    {
      imm_field(native_code_buffer, imm);
    }
  }
  
  template<bool T_is_shift>
  inline void
  two_operand_operation_r_imm(std::vector<uint8_t> &native_code_buffer,
                              uint8_t const opcode,
                              uint8_t const opcode2,
                              uint32_t const imm,
                              Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(opcode);
    
    addressing_mode_r_r(native_code_buffer, opcode2, dest);
    
    two_operand_operation_one_imm_final<T_is_shift>(native_code_buffer, imm);
  }
  
  template<bool T_is_shift>
  inline void
  two_operand_operation_stack_imm(std::vector<uint8_t> &native_code_buffer,
                                  uint8_t const opcode,
                                  uint8_t const opcode2,
                                  uint32_t const imm,
                                  uint32_t const bp_offset)
  {
    native_code_buffer.push_back(opcode);
    
    addressing_mode_stack_r(native_code_buffer, bp_offset, opcode2);
    
    two_operand_operation_one_imm_final<T_is_shift>(native_code_buffer, imm);
  }
  
  template<bool T_is_shift>
  inline void
  two_operand_operation_global_imm(std::vector<uint8_t> &native_code_buffer,
                                   uint8_t const opcode,
                                   uint8_t const opcode2,
                                   uint32_t const imm,
                                   uint32_t const addr)
  {
    native_code_buffer.push_back(opcode);
    
    addressing_mode_global_r(native_code_buffer, addr, opcode2);
    
    two_operand_operation_one_imm_final<T_is_shift>(native_code_buffer, imm);
  }
  
  // :NOTE: Wei 2005-Mar-06:
  //
  // Operation for MOV
  
  inline void
  mov_r_r(std::vector<uint8_t> &native_code_buffer,
          Register::RegNativeRep const operand,
          Register::RegNativeRep const dest)
  {
    assert(operand != dest);
    
    native_code_buffer.push_back(0x89);
    
    addressing_mode_r_r(native_code_buffer, operand, dest);
  }
  
  inline void
  mov_from_memory_r_r(std::vector<uint8_t> &native_code_buffer,
                      Register::RegNativeRep const reg,
                      Register::RegNativeRep const mem_reg)
  {
    native_code_buffer.push_back(0x8B);
    
    addressing_mode_memory_r_r(native_code_buffer, reg, mem_reg);
  }
  
  inline void
  mov_from_memory_sp_r(std::vector<uint8_t> &native_code_buffer,
                       Register::RegNativeRep const reg)
  {
    native_code_buffer.push_back(0x8B);
    
    addressing_mode_memory_sp_r(native_code_buffer, reg);
  }
  
  inline void
  mov_to_memory_r_sp(std::vector<uint8_t> &native_code_buffer,
                     Register::RegNativeRep const reg)
  {
    native_code_buffer.push_back(0x89);
    
    addressing_mode_memory_sp_r(native_code_buffer, reg);
  }
  
  inline void
  mov_r_stack(std::vector<uint8_t> &native_code_buffer,
              Register::RegNativeRep const operand,
              uint32_t const bp_offset)
  {
    native_code_buffer.push_back(0x89);
    
    addressing_mode_stack_r(native_code_buffer, bp_offset, operand);
  }
  
  inline void
  mov_r_global(std::vector<uint8_t> &native_code_buffer,
               Register::RegNativeRep const operand,
               uint32_t const addr)
  {
    native_code_buffer.push_back(0x89);
    
    addressing_mode_global_r(native_code_buffer, addr, operand);
  }
  
  inline void
  mov_stack_r(std::vector<uint8_t> &native_code_buffer,
              uint32_t const bp_offset,
              Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(0x8B);
    
    addressing_mode_stack_r(native_code_buffer, bp_offset, dest);
  }
  
  inline void
  mov_global_r(std::vector<uint8_t> &native_code_buffer,
               uint32_t const addr,
               Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(0x8B);
    
    addressing_mode_global_r(native_code_buffer, addr, dest);
  }
  
  inline void
  mov_label_r(std::vector<uint8_t> &native_code_buffer,
              Label * const label,
              Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(0xC7);
    
    addressing_mode_r_r(native_code_buffer, 0x00, dest);
    
    uint32_t const offset = native_code_buffer.size();
    
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    
    label->add_back_patch_info(Label::ABSOLUTE, offset);
  }
  
  inline void
  mov_label_global(std::vector<uint8_t> &native_code_buffer,
                   Label * const label,
                   uint32_t const addr)
  {
    native_code_buffer.push_back(0xC7);
    
    addressing_mode_global_r(native_code_buffer, addr, 0x00);
    
    uint32_t const offset = native_code_buffer.size();
    
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    
    label->add_back_patch_info(Label::ABSOLUTE, offset);
  }
  
  inline void
  mov_label_stack(std::vector<uint8_t> &native_code_buffer,
                  Label * const label,
                  uint32_t const bp_offset)
  {
    native_code_buffer.push_back(0xC7);
    
    addressing_mode_stack_r(native_code_buffer, bp_offset, 0x00);
    
    uint32_t const offset = native_code_buffer.size();
    
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    
    label->add_back_patch_info(Label::ABSOLUTE, offset);
  }
  
  inline void
  mov_imm_r(std::vector<uint8_t> &native_code_buffer,
            uint32_t const imm,
            Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(0xC7);
    
    addressing_mode_r_r(native_code_buffer, 0x00, dest);
    
    imm_field(native_code_buffer, imm);
  }
  
  inline void
  mov_imm_global(std::vector<uint8_t> &native_code_buffer,
                 uint32_t const imm,
                 uint32_t const addr)
  {
    native_code_buffer.push_back(0xC7);
    
    addressing_mode_global_r(native_code_buffer, addr, 0x00);
    
    imm_field(native_code_buffer, imm);
  }
  
  inline void
  mov_imm_stack(std::vector<uint8_t> &native_code_buffer,
                uint32_t const imm,
                uint32_t const bp_offset)
  {
    native_code_buffer.push_back(0xC7);
    
    addressing_mode_stack_r(native_code_buffer, bp_offset, 0x00);
    
    imm_field(native_code_buffer, imm);
  }
  
  inline void
  mov_memory_memory(std::vector<uint8_t> &native_code_buffer,
                    Variable const * const src,
                    Variable const * const dest)
  {
    Register::RegNativeRep const scratch_reg =
      Register::convert_reg_kind_to_its_native_rep(Register::get_one_scratch_reg());
    
    mov_r_stack(native_code_buffer,
                scratch_reg,
                gp_chunk->scratch_reg_stack_offset());
    
    switch (src->kind())
    {
    case Variable::GLOBAL:
      mov_global_r(native_code_buffer,
                   src->mem_loc_var()->memory_addr(),
                   scratch_reg);
      break;
      
    case Variable::TEMP:
      mov_stack_r(native_code_buffer,
                  src->mem_loc_var()->stack_offset(),
                  scratch_reg);
      break;
      
    case Variable::CONST:
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    switch (dest->kind())
    {
    case Variable::GLOBAL:
      mov_r_global(native_code_buffer,
                   scratch_reg,
                   dest->mem_loc_var()->memory_addr());
      break;
      
    case Variable::TEMP:
      mov_r_stack(native_code_buffer,
                  scratch_reg,
                  dest->mem_loc_var()->stack_offset());
      break;
      
    case Variable::CONST:
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    mov_stack_r(native_code_buffer,
                gp_chunk->scratch_reg_stack_offset(),
                scratch_reg);
  }
  
  // :NOTE: Wei 2005-Mar-06:
  //
  // Operation for Shift
  
  inline void
  shift_operation_r_r(std::vector<uint8_t> &native_code_buffer,
                      uint32_t const mod_rm_opcode_bits,
                      Register::RegNativeRep const dest)
  {
    assert(dest != Register::ECX_NATIVE_REP);
    
    native_code_buffer.push_back(0xD3);
    
    native_code_buffer.push_back(0xC0 | (mod_rm_opcode_bits << 3) | dest);
  }
  
  inline void
  shift_operation_global_r(std::vector<uint8_t> &native_code_buffer,
                           uint32_t const mod_rm_opcode_bits,
                           uint32_t const addr)
  {
    native_code_buffer.push_back(0xD3);
    
    native_code_buffer.push_back((mod_rm_opcode_bits << 3) | 0x05);
    
    native_code_buffer.push_back(static_cast<uint8_t>(addr));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(addr >> 24));
  }
  
  inline void
  shift_operation_stack_r(std::vector<uint8_t> &native_code_buffer,
                          uint32_t const mod_rm_opcode_bits,
                          uint32_t const bp_offset)
  {
    native_code_buffer.push_back(0xD3);
    
    native_code_buffer.push_back(0x80 | (mod_rm_opcode_bits << 3) | 0x05);
    
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(bp_offset >> 24));
  }
  
  // :NOTE: Wei 2005-Mar-15:
  //
  // Not operation
  
  inline void
  not_r(std::vector<uint8_t> &native_code_buffer,
        Register::RegNativeRep const dest)
  {
    native_code_buffer.push_back(0xF7);
    
    addressing_mode_r_r(native_code_buffer, 0x2, dest);
  }
  
  inline void
  not_global(std::vector<uint8_t> &native_code_buffer,
             uint32_t const addr)
  {
    native_code_buffer.push_back(0xF7);
    
    addressing_mode_global_r(native_code_buffer, addr, 0x2);
  }
  
  inline void
  not_stack(std::vector<uint8_t> &native_code_buffer,
            uint32_t const bp_offset)
  {
    native_code_buffer.push_back(0xF7);
    
    addressing_mode_stack_r(native_code_buffer, bp_offset, 0x2);
  }
  
  // :NOTE: Wei 2005-Mar-16:
  //
  // Jump instruction
  inline void
  jmp(std::vector<uint8_t> &native_code_buffer,
      uint8_t const opcode,
      Label * const label)
  {
    assert(Label::BASIC_BLOCK == label->kind());
    
    native_code_buffer.push_back(0x0F);
    native_code_buffer.push_back(opcode);
    
    uint32_t const offset = native_code_buffer.size();
    
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    
    label->add_back_patch_info(Label::RELATIVE_TO_NEXT, offset);
  }
  
  inline void
  jmp_to_label(std::vector<uint8_t> &native_code_buffer,
               Label * const label)
  {
    native_code_buffer.push_back(0xE9);
    
    uint32_t const offset = native_code_buffer.size();
    
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    native_code_buffer.push_back(0);
    
    assert(Label::BASIC_BLOCK == label->kind());
    
    label->add_back_patch_info(Label::RELATIVE_TO_NEXT, offset);
  }
  
  // :NOTE: Wei 2005-Mar-19:
  //
  // Prologue
  inline void
  emit_native_prologue_codes(std::vector<uint8_t> &native_code_buffer)
  {
    // :NOTE: Wei 2005-Mar-19:
    //
    // pushl %ebp
    native_code_buffer.push_back(0x50 | Register::EBP_NATIVE_REP);
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // movl %esp, %ebp
    mov_r_r(native_code_buffer, Register::ESP_NATIVE_REP, Register::EBP_NATIVE_REP);
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // subl $?, %esp
    native_code_buffer.push_back(0x81);
    addressing_mode_r_r(native_code_buffer, 0x05, Register::ESP_NATIVE_REP);
    
    uint32_t const stack_size = gp_chunk->stack_size();
    
    native_code_buffer.push_back(static_cast<uint8_t>(stack_size));
    native_code_buffer.push_back(static_cast<uint8_t>(stack_size >> 8));
    native_code_buffer.push_back(static_cast<uint8_t>(stack_size >> 16));
    native_code_buffer.push_back(static_cast<uint8_t>(stack_size >> 24));
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // The callee-save registers are EDI, ESI, EBX
    //
    // pushl %edi
    native_code_buffer.push_back(0x50 | Register::EDI_NATIVE_REP);
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // pushl %esi
    native_code_buffer.push_back(0x50 | Register::ESI_NATIVE_REP);
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // pushl %ebx
    native_code_buffer.push_back(0x50 | Register::EBX_NATIVE_REP);
  }
  
  // :NOTE: Wei 2005-Mar-19:
  //
  // Epilogue
  inline void
  emit_native_epilogue_codes(std::vector<uint8_t> &native_code_buffer)
  {
    // :NOTE: Wei 2005-Mar-19:
    //
    // The callee-save registers are EDI, ESI, EBX
    //
    // popl %ebx
    native_code_buffer.push_back(0x58 | Register::EBX_NATIVE_REP);
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // popl %esi
    native_code_buffer.push_back(0x58 | Register::ESI_NATIVE_REP);
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // popl %edi
    native_code_buffer.push_back(0x58 | Register::EDI_NATIVE_REP);
    
    // :NOTE: Wei 2005-Mar-19:
    //
    // leave
    native_code_buffer.push_back(0xC9);
    
    // :NOTE: Wei 2005-Mar-30:
    //
    // ret
    native_code_buffer.push_back(0xC3);
  }
}

#endif

#endif
