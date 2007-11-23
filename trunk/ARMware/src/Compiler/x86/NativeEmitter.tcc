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

#if ENABLE_DYNAMIC_TRANSLATOR

namespace ARMware
{
  template<typename T_NativeOperTraits, bool T_operand_exchangable>
  inline void
  emit_two_operand_operation_RR(std::vector<uint8_t> &native_code_buffer,
                                Variable const * const first_operand,
                                Variable const * const second_operand,
                                Variable const * const dest)
  {
    assert(first_operand != 0);
    assert(second_operand != 0);
    assert(dest != 0);
    
    assert(first_operand->join_var()->live_interval() != 0);
    assert(second_operand->join_var()->live_interval() != 0);
    
    Register const * const first_operand_reg = first_operand->join_var()->live_interval()->preallocated_reg();
    Register const * const second_operand_reg = second_operand->join_var()->live_interval()->preallocated_reg();
    
    if (dest->join_var()->live_interval()->preallocated_reg() != 0)
    {
      Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
      
      // :NOTE: Wei 2005-Mar-06:
      //
      // Destination is in register.
      if (first_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 1st operand is in register.
        if (second_operand_reg != 0)
        {
          // :NOTE: Wei 2005-Mar-06:
          //
          // 2nd operand is in register.
          //
          // All 3 variables are reside in registers.
          if (first_operand_reg->kind() == dest_reg->kind())
          {
            two_operand_operation_r_r(native_code_buffer,
                                      T_NativeOperTraits::R_R_to_R,
                                      second_operand_reg->native_represent(),
                                      dest_reg->native_represent());
          }
          else if (second_operand_reg->kind() == dest_reg->kind())
          {
            mov_r_stack(native_code_buffer,
                        first_operand_reg->native_represent(),
                        gp_chunk->scratch_reg_stack_offset());
            
            two_operand_operation_r_r(native_code_buffer,
                                      T_NativeOperTraits::R_R_to_R,
                                      second_operand_reg->native_represent(),
                                      first_operand_reg->native_represent());
            
            mov_r_r(native_code_buffer,
                    first_operand_reg->native_represent(),
                    dest_reg->native_represent());
            
            mov_stack_r(native_code_buffer,
                        gp_chunk->scratch_reg_stack_offset(),
                        first_operand_reg->native_represent());
          }
          else
          {
            // :NOTE: Wei 2005-Mar-06:
            //
            // All three registers are different.
            
            // :NOTE: Wei 2005-Mar-06:
            //
            // Mov to the real destination register first.
            mov_r_r(native_code_buffer,
                    first_operand_reg->native_represent(),
                    dest_reg->native_represent());
            
            two_operand_operation_r_r(native_code_buffer,
                                      T_NativeOperTraits::R_R_to_R,
                                      second_operand_reg->native_represent(),
                                      dest_reg->native_represent());
          }
        }
        else
        {
          // :NOTE: Wei 2005-Mar-04:
          //
          // 2nd operand is in memory
          //
          // Dest, 1st are reside in registers.
          if (first_operand_reg->kind() != dest_reg->kind())
          {
            mov_r_r(native_code_buffer,
                    first_operand_reg->native_represent(),
                    dest_reg->native_represent());
          }
          
          switch (second_operand->kind())
          {
          case Variable::GLOBAL:
            two_operand_operation_global_r(native_code_buffer,
                                           T_NativeOperTraits::M_R_to_R,
                                           second_operand->mem_loc_var()->memory_addr(),
                                           dest_reg->native_represent());
            break;
            
          case Variable::TEMP:
            two_operand_operation_stack_r(native_code_buffer,
                                          T_NativeOperTraits::M_R_to_R,
                                          second_operand->mem_loc_var()->stack_offset(),
                                          dest_reg->native_represent());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 1st operand is in memory
        if (second_operand_reg != 0)
        {
          // :NOTE: Wei 2005-Mar-06:
          //
          // 2nd operand is in register.
          //
          // Dest is in register, 1st operand is in memory.
          if (true == T_operand_exchangable)
          {
            if (second_operand_reg->kind() != dest_reg->kind())
            {
              mov_r_r(native_code_buffer,
                      second_operand_reg->native_represent(),
                      dest_reg->native_represent());
            }
            
            switch (first_operand->kind())
            {
            case Variable::GLOBAL:
              two_operand_operation_global_r(native_code_buffer,
                                             T_NativeOperTraits::M_R_to_R,
                                             first_operand->mem_loc_var()->memory_addr(),
                                             dest_reg->native_represent());
              break;
              
            case Variable::TEMP:
              two_operand_operation_stack_r(native_code_buffer,
                                            T_NativeOperTraits::M_R_to_R,
                                            first_operand->mem_loc_var()->stack_offset(),
                                            dest_reg->native_represent());
              break;
              
            case Variable::CONST:
            default:
              assert(!"Should not reach here.");
              break;
            }
          }
          else
          {
            if (second_operand_reg->kind() != dest_reg->kind())
            {
              switch (first_operand->kind())
              {
              case Variable::GLOBAL:
                mov_global_r(native_code_buffer,
                             first_operand->mem_loc_var()->memory_addr(),
                             dest_reg->native_represent());
                break;
                
              case Variable::TEMP:
                mov_stack_r(native_code_buffer,
                            first_operand->mem_loc_var()->stack_offset(),
                            dest_reg->native_represent());
                break;
                
              case Variable::CONST:
              default:
                assert(!"Should not reach here.");
                break;
              }
              
              two_operand_operation_r_r(native_code_buffer,
                                        T_NativeOperTraits::R_R_to_R,
                                        second_operand_reg->native_represent(),
                                        dest_reg->native_represent());
            }
            else
            {
              Register::RegNativeRep const scratch_reg =
                Register::convert_reg_kind_to_its_native_rep(
                  Register::get_one_scratch_reg(second_operand_reg->kind()));
              
              mov_r_stack(native_code_buffer,
                          scratch_reg,
                          gp_chunk->scratch_reg_stack_offset());
              
              switch (first_operand->kind())
              {
              case Variable::GLOBAL:
                mov_global_r(native_code_buffer,
                             first_operand->mem_loc_var()->memory_addr(),
                             scratch_reg);
                break;
                
              case Variable::TEMP:
                mov_stack_r(native_code_buffer,
                            first_operand->mem_loc_var()->stack_offset(),
                            scratch_reg);
                break;
                
              case Variable::CONST:
              default:
                assert(!"Should not reach here.");
                break;
              }
              
              two_operand_operation_r_r(native_code_buffer,
                                        T_NativeOperTraits::R_R_to_R,
                                        second_operand_reg->native_represent(),
                                        scratch_reg);
              
              mov_r_r(native_code_buffer,
                      scratch_reg,
                      dest_reg->native_represent());
              
              mov_stack_r(native_code_buffer,
                          gp_chunk->scratch_reg_stack_offset(),
                          scratch_reg);
            }
          }
        }
        else
        {
          // :NOTE: Wei 2005-Mar-04:
          //
          // 2nd operand is in memory
          //
          // Dest is in register, 1st operand is in memory.
          switch (first_operand->kind())
          {
          case Variable::GLOBAL:
            mov_global_r(native_code_buffer,
                         first_operand->mem_loc_var()->memory_addr(),
                         dest_reg->native_represent());
            break;
            
          case Variable::TEMP:
            mov_stack_r(native_code_buffer,
                        first_operand->mem_loc_var()->stack_offset(),
                        dest_reg->native_represent());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          switch (second_operand->kind())
          {
          case Variable::GLOBAL:
            two_operand_operation_global_r(native_code_buffer,
                                           T_NativeOperTraits::M_R_to_R,
                                           second_operand->mem_loc_var()->memory_addr(),
                                           dest_reg->native_represent());
            break;
            
          case Variable::TEMP:
            two_operand_operation_stack_r(native_code_buffer,
                                          T_NativeOperTraits::M_R_to_R,
                                          second_operand->mem_loc_var()->stack_offset(),
                                          dest_reg->native_represent());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
      }
    }
    else
    {
      // :NOTE: Wei 2005-Mar-04:
      //
      // Destination is in memory
      if (first_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 1st operand is in register.
        if (second_operand_reg != 0)
        {
          // :NOTE: Wei 2005-Mar-06:
          //
          // 2nd operand is in register.
          //
          // Dest is in memory, 1st operand is in register.
          switch (dest->kind())
          {
          case Variable::GLOBAL:
            mov_r_global(native_code_buffer,
                         first_operand_reg->native_represent(),
                         dest->mem_loc_var()->memory_addr());
            
            two_operand_operation_global_r(native_code_buffer,
                                           T_NativeOperTraits::R_M_to_M,
                                           dest->mem_loc_var()->memory_addr(),
                                           second_operand_reg->native_represent());
            break;
            
          case Variable::TEMP:
            mov_r_stack(native_code_buffer,
                        first_operand_reg->native_represent(),
                        dest->mem_loc_var()->stack_offset());
            
            two_operand_operation_stack_r(native_code_buffer,
                                          T_NativeOperTraits::R_M_to_M,
                                          dest->mem_loc_var()->stack_offset(),
                                          second_operand_reg->native_represent());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
        }
        else
        {
          // :NOTE: Wei 2005-Mar-04:
          //
          // 2nd operand is in memory
          //
          // Dest is in memory, 1st operand is in register.
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Free a scratch register.
          mov_r_stack(native_code_buffer,
                      first_operand_reg->native_represent(),
                      gp_chunk->scratch_reg_stack_offset());
          
          switch (second_operand->kind())
          {
          case Variable::GLOBAL:
            two_operand_operation_global_r(native_code_buffer,
                                           T_NativeOperTraits::M_R_to_R,
                                           second_operand->mem_loc_var()->memory_addr(),
                                           first_operand_reg->native_represent());
            break;
            
          case Variable::TEMP:
            two_operand_operation_stack_r(native_code_buffer,
                                          T_NativeOperTraits::M_R_to_R,
                                          second_operand->mem_loc_var()->stack_offset(),
                                          first_operand_reg->native_represent());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Move the scratch reg to the dest mem.
          switch (dest->kind())
          {
          case Variable::GLOBAL:
            mov_r_global(native_code_buffer,
                         first_operand_reg->native_represent(),
                         dest->mem_loc_var()->memory_addr());
            break;
            
          case Variable::TEMP:
            mov_r_stack(native_code_buffer,
                        first_operand_reg->native_represent(),
                        dest->mem_loc_var()->stack_offset());
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Reload the scratch register.
          mov_stack_r(native_code_buffer,
                      gp_chunk->scratch_reg_stack_offset(),
                      first_operand_reg->native_represent());
        }
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 1st operand is in memory
        if (second_operand_reg != 0)
        {
          // :NOTE: Wei 2005-Mar-06:
          //
          // 2nd operand is in register.
          //
          // Dest, 1st operand are in memory.
            
          // :NOTE: Wei 2005-Mar-06:
          //
          // Free a scratch register.
          Register::RegNativeRep const scratch_reg =
            Register::convert_reg_kind_to_its_native_rep(
              Register::get_one_scratch_reg(second_operand_reg->kind()));
            
          mov_r_stack(native_code_buffer,
                      scratch_reg,
                      gp_chunk->scratch_reg_stack_offset());
            
          // :NOTE: Wei 2005-Mar-06:
          //
          // Load 1st operand to scratch reg.
          switch (first_operand->kind())
          {
          case Variable::GLOBAL:
            mov_global_r(native_code_buffer,
                         first_operand->mem_loc_var()->memory_addr(),
                         scratch_reg);
            break;
              
          case Variable::TEMP:
            mov_stack_r(native_code_buffer,
                        first_operand->mem_loc_var()->stack_offset(),
                        scratch_reg);
            break;
              
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Do the operation with the scratch reg.
          two_operand_operation_r_r(native_code_buffer,
                                    T_NativeOperTraits::R_R_to_R,
                                    second_operand_reg->native_represent(),
                                    scratch_reg);
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Move the scratch reg to the dest mem.
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
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Reload the scratch register.
          mov_stack_r(native_code_buffer, gp_chunk->scratch_reg_stack_offset(), scratch_reg);
        }
        else
        {
          // :NOTE: Wei 2005-Mar-04:
          //
          // All three variables are in memory
            
          // :NOTE: Wei 2005-Mar-06:
          //
          // Free a scratch register.
          Register::RegNativeRep const scratch_reg =
            Register::convert_reg_kind_to_its_native_rep(Register::get_one_scratch_reg());
            
          mov_r_stack(native_code_buffer, scratch_reg, gp_chunk->scratch_reg_stack_offset());
            
          // :NOTE: Wei 2005-Mar-06:
          //
          // Load 1st operand to scratch reg.
          switch (first_operand->kind())
          {
          case Variable::GLOBAL:
            mov_global_r(native_code_buffer,
                         first_operand->mem_loc_var()->memory_addr(),
                         scratch_reg);
            break;
              
          case Variable::TEMP:
            mov_stack_r(native_code_buffer,
                        first_operand->mem_loc_var()->stack_offset(),
                        scratch_reg);
            break;
              
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
            
          // :NOTE: Wei 2005-Mar-06:
          //
          // Do the operation with the scratch reg.
          switch (second_operand->kind())
          {
          case Variable::GLOBAL:
            two_operand_operation_global_r(native_code_buffer,
                                           T_NativeOperTraits::M_R_to_R,
                                           second_operand->mem_loc_var()->memory_addr(),
                                           scratch_reg);
            break;
            
          case Variable::TEMP:
            two_operand_operation_stack_r(native_code_buffer,
                                          T_NativeOperTraits::M_R_to_R,
                                          second_operand->mem_loc_var()->stack_offset(),
                                          scratch_reg);
            break;
            
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Move the scratch reg to the dest mem.
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
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Reload the scratch register.
          mov_stack_r(native_code_buffer, gp_chunk->scratch_reg_stack_offset(), scratch_reg);
        }
      }
    }
  }
  
  template<typename T_NativeOperTraits, bool T_is_shift>
  inline void
  emit_two_operand_operation_RI(std::vector<uint8_t> &native_code_buffer,
                                Variable const * const first_operand,
                                uint32_t const imm,
                                Variable const * const dest)
  {
#if CHECK_CHUNK_CODE
    if (true == T_is_shift)
    {
      assert(imm < ((1 << 8) - 1));
    }
#endif
    
    assert(first_operand != 0);
    assert(dest != 0);
    
    assert(first_operand->join_var()->live_interval() != 0);
    
    Register const * const first_operand_reg = first_operand->join_var()->live_interval()->preallocated_reg();
    
    if (dest->join_var()->live_interval()->preallocated_reg() != 0)
    {
      Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
      
      // :NOTE: Wei 2005-Mar-06:
      //
      // Destination is in register.
      if (first_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 1st operand is in register.
        //
        // All 2 variables are reside in registers.
        if (first_operand_reg->kind() == dest_reg->kind())
        {
          two_operand_operation_r_imm<T_is_shift>(native_code_buffer,
                                                  T_NativeOperTraits::I_MR_to_MR,
                                                  T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                                  imm,
                                                  dest_reg->native_represent());
        }
        else
        {
          // :NOTE: Wei 2005-Mar-06:
          //
          // All 2 registers are different.
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Mov to the real destination register first.
          mov_r_r(native_code_buffer,
                  first_operand_reg->native_represent(),
                  dest_reg->native_represent());
          
          two_operand_operation_r_imm<T_is_shift>(native_code_buffer,
                                                  T_NativeOperTraits::I_MR_to_MR,
                                                  T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                                  imm,
                                                  dest_reg->native_represent());
        }
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 1st operand is in memory
        //
        // Dest is in register.
        switch (first_operand->kind())
        {
        case Variable::GLOBAL:
          mov_global_r(native_code_buffer,
                       first_operand->mem_loc_var()->memory_addr(),
                       dest_reg->native_represent());
          break;
          
        case Variable::TEMP:
          mov_stack_r(native_code_buffer,
                      first_operand->mem_loc_var()->stack_offset(),
                      dest_reg->native_represent());
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        two_operand_operation_r_imm<T_is_shift>(native_code_buffer,
                                                T_NativeOperTraits::I_MR_to_MR,
                                                T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                                imm,
                                                dest_reg->native_represent());
      }
    }
    else
    {
      // :NOTE: Wei 2005-Mar-04:
      //
      // Destination is in memory
      if (first_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 1st operand is in register.
        //
        // Dest is in memory, 1st operand is in register.
        switch (dest->kind())
        {
        case Variable::GLOBAL:
          mov_r_global(native_code_buffer,
                       first_operand_reg->native_represent(),
                       dest->mem_loc_var()->memory_addr());
          
          two_operand_operation_global_imm<T_is_shift>(native_code_buffer,
                                                       T_NativeOperTraits::I_MR_to_MR,
                                                       T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                                       imm,
                                                       dest->mem_loc_var()->memory_addr());
          break;
          
        case Variable::TEMP:
          mov_r_stack(native_code_buffer,
                      first_operand_reg->native_represent(),
                      dest->mem_loc_var()->stack_offset());
          
          two_operand_operation_stack_imm<T_is_shift>(native_code_buffer,
                                                      T_NativeOperTraits::I_MR_to_MR,
                                                      T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                                      imm,
                                                      dest->mem_loc_var()->stack_offset());
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 1st operand is in memory
        //
        // Dest, 1st operand are in memory.
          
        // :TODO: Wei 2005-Mar-07:
        //
        // Add codes to simply the condition of 'Dest' & '1st operand' are in
        // the same memory location.
          
        // :NOTE: Wei 2005-Mar-06:
        //
        // Free a scratch register.
        Register::RegNativeRep const scratch_reg =
          Register::convert_reg_kind_to_its_native_rep(Register::get_one_scratch_reg());
          
        mov_r_stack(native_code_buffer,
                    scratch_reg,
                    gp_chunk->scratch_reg_stack_offset());
          
        // :NOTE: Wei 2005-Mar-06:
        //
        // Load 1st operand to scratch reg.
        switch (first_operand->kind())
        {
        case Variable::GLOBAL:
          mov_global_r(native_code_buffer,
                       first_operand->mem_loc_var()->memory_addr(),
                       scratch_reg);
          break;
          
        case Variable::TEMP:
          mov_stack_r(native_code_buffer,
                      first_operand->mem_loc_var()->stack_offset(),
                      scratch_reg);
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // Do the operation with the scratch reg.
        two_operand_operation_r_imm<T_is_shift>(native_code_buffer,
                                                T_NativeOperTraits::I_MR_to_MR,
                                                T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                                imm,
                                                scratch_reg);
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // Move the scratch reg to the dest mem.
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
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // Reload the scratch register.
        mov_stack_r(native_code_buffer, gp_chunk->scratch_reg_stack_offset(), scratch_reg);
      }
    }
  }
  
  template<typename T_NativeOperTraits>
  inline void
  emit_two_operand_operation_without_dest_RI(std::vector<uint8_t> &native_code_buffer,
                                             Variable const * const first_operand,
                                             uint32_t const imm)
  {
    assert(first_operand != 0);
    
    assert(first_operand->join_var()->live_interval() != 0);
    
    Register const * const first_operand_reg = first_operand->join_var()->live_interval()->preallocated_reg();
    
    if (first_operand_reg != 0)
    {
      // :NOTE: Wei 2005-Mar-06:
      //
      // 1st operand is in register.
      //
      // Because only CMP & TST will use this method,
      // thus I can pass 'false' as the template argument here.
      two_operand_operation_r_imm<false>(native_code_buffer,
                                         T_NativeOperTraits::I_MR_to_MR,
                                         T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                         imm,
                                         first_operand_reg->native_represent());
    }
    else
    {
      // :NOTE: Wei 2005-Mar-04:
      //
      // 1st operand is in memory
      switch (first_operand->kind())
      {
      case Variable::GLOBAL:
        two_operand_operation_global_imm<false>(native_code_buffer,
                                                T_NativeOperTraits::I_MR_to_MR,
                                                T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                                imm,
                                                first_operand->mem_loc_var()->memory_addr());
        break;
        
      case Variable::TEMP:
        two_operand_operation_stack_imm<false>(native_code_buffer,
                                               T_NativeOperTraits::I_MR_to_MR,
                                               T_NativeOperTraits::I_MR_to_MR_OPCODE_2,
                                               imm,
                                               first_operand->mem_loc_var()->stack_offset());
        break;
        
      case Variable::CONST:
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
  }
  
  template<typename T_NativeOperTraits>
  inline void
  emit_two_operand_operation_without_dest_RR(std::vector<uint8_t> &native_code_buffer,
                                             Variable const * const first_operand,
                                             Variable const * const second_operand)
  {
    assert(first_operand != 0);
    assert(second_operand != 0);
    
    assert(first_operand->join_var()->live_interval() != 0);
    assert(second_operand->join_var()->live_interval() != 0);
    
    Register const * const first_operand_reg = first_operand->join_var()->live_interval()->preallocated_reg();
    Register const * const second_operand_reg = second_operand->join_var()->live_interval()->preallocated_reg();
    
    if (first_operand_reg != 0)
    {
      // :NOTE: Wei 2005-Mar-06:
      //
      // 1st operand is in register.
      if (second_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 2nd operand is in register.
        //
        // All 2 variables are reside in registers.
        two_operand_operation_r_r(native_code_buffer,
                                  T_NativeOperTraits::R_R_to_R,
                                  second_operand_reg->native_represent(),
                                  first_operand_reg->native_represent());
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 2nd operand is in memory
        //
        // 1st operand is in registers.
        switch (second_operand->kind())
        {
        case Variable::GLOBAL:
          two_operand_operation_global_r(native_code_buffer,
                                         T_NativeOperTraits::M_R_to_R,
                                         second_operand->mem_loc_var()->memory_addr(),
                                         first_operand_reg->native_represent());
          break;
          
        case Variable::TEMP:
          two_operand_operation_stack_r(native_code_buffer,
                                        T_NativeOperTraits::M_R_to_R,
                                        second_operand->mem_loc_var()->stack_offset(),
                                        first_operand_reg->native_represent());
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
    }
    else
    {
      // :NOTE: Wei 2005-Mar-04:
      //
      // 1st operand is in memory
      if (second_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 2nd operand is in register.
        //
        // 1st operand is in memory.
        switch (first_operand->kind())
        {
        case Variable::GLOBAL:
          two_operand_operation_global_r(native_code_buffer,
                                         T_NativeOperTraits::R_M_to_M,
                                         first_operand->mem_loc_var()->memory_addr(),
                                         second_operand_reg->native_represent());
          break;
          
        case Variable::TEMP:
          two_operand_operation_stack_r(native_code_buffer,
                                        T_NativeOperTraits::R_M_to_M,
                                        first_operand->mem_loc_var()->stack_offset(),
                                        second_operand_reg->native_represent());
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 2nd operand is in memory
        //
        // 1st operand is in memory.
        Register::RegNativeRep const scratch_reg =
          Register::convert_reg_kind_to_its_native_rep(Register::get_one_scratch_reg());
        
        mov_r_stack(native_code_buffer,
                    scratch_reg,
                    gp_chunk->scratch_reg_stack_offset());
        
        switch (first_operand->kind())
        {
        case Variable::GLOBAL:
          mov_global_r(native_code_buffer,
                       first_operand->mem_loc_var()->memory_addr(),
                       scratch_reg);
          break;
          
        case Variable::TEMP:
          mov_stack_r(native_code_buffer,
                      first_operand->mem_loc_var()->stack_offset(),
                      scratch_reg);
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        switch (second_operand->kind())
        {
        case Variable::GLOBAL:
          two_operand_operation_global_r(native_code_buffer,
                                         T_NativeOperTraits::M_R_to_R,
                                         second_operand->mem_loc_var()->memory_addr(),
                                         scratch_reg);
          break;
          
        case Variable::TEMP:
          two_operand_operation_stack_r(native_code_buffer,
                                        T_NativeOperTraits::M_R_to_R,
                                        second_operand->mem_loc_var()->stack_offset(),
                                        scratch_reg);
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
    }
  }
  
  template<typename T_NativeOperTraits>
  inline void
  emit_shift_operation_RR(std::vector<uint8_t> &native_code_buffer,
                          Variable const * const first_operand,
                          Variable const * const dest)
  {
    assert(first_operand != 0);
    assert(dest != 0);
    
    assert(first_operand->join_var()->live_interval() != 0);
    
    Register const * const first_operand_reg = first_operand->join_var()->live_interval()->preallocated_reg();
      
    if (dest->join_var()->live_interval()->preallocated_reg() != 0)
    {
      Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
      
      // :NOTE: Wei 2005-Mar-06:
      //
      // Destination is in register.
      if (first_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 1st operand is in register.
        //
        // 2nd operand is in register.
        //
        // All 3 variables are reside in registers.
        if (first_operand_reg->kind() == dest_reg->kind())
        {
          shift_operation_r_r(native_code_buffer,
                              T_NativeOperTraits::MODRM_OPCODE_BITS,
                              dest_reg->native_represent());
        }
        else if (Register::ECX == dest_reg->kind())
        {
          mov_r_stack(native_code_buffer,
                      first_operand_reg->native_represent(),
                      gp_chunk->scratch_reg_stack_offset());
          
          shift_operation_r_r(native_code_buffer,
                              T_NativeOperTraits::MODRM_OPCODE_BITS,
                              first_operand_reg->native_represent());
          
          mov_r_r(native_code_buffer,
                  first_operand_reg->native_represent(),
                  dest_reg->native_represent());
          
          mov_stack_r(native_code_buffer,
                      gp_chunk->scratch_reg_stack_offset(),
                      first_operand_reg->native_represent());
        }
        else
        {
          // :NOTE: Wei 2005-Mar-06:
          //
          // All three registers are different.
          
          // :NOTE: Wei 2005-Mar-06:
          //
          // Mov to the real destination register first.
          mov_r_r(native_code_buffer,
                  first_operand_reg->native_represent(),
                  dest_reg->native_represent());
          
          shift_operation_r_r(native_code_buffer,
                              T_NativeOperTraits::MODRM_OPCODE_BITS,
                              dest_reg->native_represent());
        }
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 1st operand is in memory
        //
        // 2nd operand is in register.
        //
        // Dest is in register, 1st operand is in memory.
        if (Register::ECX != dest_reg->kind())
        {
          switch (first_operand->kind())
          {
          case Variable::GLOBAL:
            mov_global_r(native_code_buffer,
                         first_operand->mem_loc_var()->memory_addr(),
                         dest_reg->native_represent());
            break;
              
          case Variable::TEMP:
            mov_stack_r(native_code_buffer,
                        first_operand->mem_loc_var()->stack_offset(),
                        dest_reg->native_represent());
            break;
              
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
            
          shift_operation_r_r(native_code_buffer,
                              T_NativeOperTraits::MODRM_OPCODE_BITS,
                              dest_reg->native_represent());
        }
        else
        {
          Register::RegNativeRep const scratch_reg =
            Register::convert_reg_kind_to_its_native_rep(Register::get_one_scratch_reg(Register::ECX));
            
          mov_r_stack(native_code_buffer,
                      scratch_reg,
                      gp_chunk->scratch_reg_stack_offset());
            
          switch (first_operand->kind())
          {
          case Variable::GLOBAL:
            mov_global_r(native_code_buffer,
                         first_operand->mem_loc_var()->memory_addr(),
                         scratch_reg);
            break;
              
          case Variable::TEMP:
            mov_stack_r(native_code_buffer,
                        first_operand->mem_loc_var()->stack_offset(),
                        scratch_reg);
            break;
              
          case Variable::CONST:
          default:
            assert(!"Should not reach here.");
            break;
          }
            
          shift_operation_r_r(native_code_buffer,
                              T_NativeOperTraits::MODRM_OPCODE_BITS,
                              scratch_reg);
            
          mov_r_r(native_code_buffer,
                  scratch_reg,
                  dest_reg->native_represent());
          
          mov_stack_r(native_code_buffer,
                      gp_chunk->scratch_reg_stack_offset(),
                      scratch_reg);
        }
      }
    }
    else
    {
      // :NOTE: Wei 2005-Mar-04:
      //
      // Destination is in memory
      if (first_operand_reg != 0)
      {
        // :NOTE: Wei 2005-Mar-06:
        //
        // 1st operand is in register.
        assert(first_operand_reg->kind() != Register::ECX);
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // 2nd operand is in register.
        //
        // Dest is in memory, 1st operand is in register.
        switch (dest->kind())
        {
        case Variable::GLOBAL:
          mov_r_global(native_code_buffer,
                       first_operand_reg->native_represent(),
                       dest->mem_loc_var()->memory_addr());
          
          shift_operation_global_r(native_code_buffer,
                                   T_NativeOperTraits::MODRM_OPCODE_BITS,
                                   dest->mem_loc_var()->memory_addr());
          break;
          
        case Variable::TEMP:
          mov_r_stack(native_code_buffer,
                      first_operand_reg->native_represent(),
                      dest->mem_loc_var()->stack_offset());
          
          shift_operation_stack_r(native_code_buffer,
                                  T_NativeOperTraits::MODRM_OPCODE_BITS,
                                  dest->mem_loc_var()->stack_offset());
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
      }
      else
      {
        // :NOTE: Wei 2005-Mar-04:
        //
        // 1st operand is in memory
        //
        // 2nd operand is in register.
        //
        // Dest, 1st operand are in memory.
          
        // :NOTE: Wei 2005-Mar-06:
        //
        // Free a scratch register.
        Register::RegNativeRep const scratch_reg =
          Register::convert_reg_kind_to_its_native_rep(Register::get_one_scratch_reg(Register::ECX));
          
        mov_r_stack(native_code_buffer,
                    scratch_reg,
                    gp_chunk->scratch_reg_stack_offset());
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // Load 1st operand to scratch reg.
        switch (first_operand->kind())
        {
        case Variable::GLOBAL:
          mov_global_r(native_code_buffer,
                       first_operand->mem_loc_var()->memory_addr(),
                       scratch_reg);
          break;
          
        case Variable::TEMP:
          mov_stack_r(native_code_buffer,
                      first_operand->mem_loc_var()->stack_offset(),
                      scratch_reg);
          break;
          
        case Variable::CONST:
        default:
          assert(!"Should not reach here.");
          break;
        }
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // Do the operation with the scratch reg.
        shift_operation_r_r(native_code_buffer,
                            T_NativeOperTraits::MODRM_OPCODE_BITS,
                            scratch_reg);
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // Move the scratch reg to the dest mem.
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
        
        // :NOTE: Wei 2005-Mar-06:
        //
        // Reload the scratch register.
        mov_stack_r(native_code_buffer, gp_chunk->scratch_reg_stack_offset(), scratch_reg);
      }
    }
  }
  
  template<typename T_NativeOperTraits>
  inline void
  emit_set_byte_operation(std::vector<uint8_t> &native_code_buffer,
                          Variable const * const dest)
  {
    assert(dest != 0);
    
    Register const * const dest_reg = dest->join_var()->live_interval()->preallocated_reg();
    
    // :NOTE: Wei 2005-Apr-6:
    //
    // Becuase SetCC only change the low 8-bit of the destination register,
    // but I need the whole destination register be affected,
    // thus I have to clear the destination register to 0 first.
    if (dest_reg != 0)
    {
      mov_imm_r(native_code_buffer, 0, dest_reg->native_represent());
    }
    else
    {
      switch (dest->kind())
      {
      case Variable::GLOBAL:
        mov_imm_global(native_code_buffer, 0, dest->mem_loc_var()->memory_addr());
        break;
        
      case Variable::TEMP:
        mov_imm_stack(native_code_buffer, 0, dest->mem_loc_var()->stack_offset());
        break;
        
      case Variable::CONST:
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    native_code_buffer.push_back(0x0F);
    
    // :NOTE: Wei 2005-Mar-20:
    //
    // Because std::vector::push_back(T const &) takes an reference argument,
    // thus the argument must be a lvalue, i.e. the argument must have its own memory space.
    //
    // If the argument is a rvalue, then the C++ compiler will make a unnamed temporary variable
    // contains the rvalue for me, althought according to the C++ standard,
    // a unnamed temporary variable is also a rvalue, however, it can indeed assign to a const
    // reference.
    //
    // According to the C++ standard, a variable which has a name must be a lvalue,
    // thus T_NativeOperTraits::SET_BYTE_OPCODE is a lvalue, althought it is a const variable.
    // Thus the C++ compiler will not make any temporary variable for me, and intends that
    // 'T_NativeOperTraits::SET_BYTE_OPCODE' has its own memory space, so that the compiler
    // can take its memory address as the argument.
    //
    // Thus, I have to provide a memory space for it when I write the following codes:
    //
    // native_code_buffer.push_back(T_NativeOperTraits::SET_BYTE_OPCODE);
    //
    // If I don't want to provide a memory space for it, then I need to do a cast,
    // because a casting will result a unnamed temporary variable generated for me,
    // thus a unnamed temporary variable (a rvalue) can be assign to a const reference.
    native_code_buffer.push_back(static_cast<uint8_t>(T_NativeOperTraits::SET_BYTE_OPCODE));
    
    if (dest_reg != 0)
    {
      assert(dest_reg->kind() != Register::ESI);
      assert(dest_reg->kind() != Register::EDI);
      
      // :NOTE: Wei 2005-Mar-18:
      //
      // Althought the Intel's manual doesn't say about the value of the middle opcode field of ModR/M,
      // I think it should be 0.
      addressing_mode_r_r(native_code_buffer, 0, dest_reg->native_represent());
    }
    else
    {
      switch (dest->kind())
      {
      case Variable::GLOBAL:
        addressing_mode_global_r(native_code_buffer, dest->mem_loc_var()->memory_addr(), 0);
        break;
        
      case Variable::TEMP:
        addressing_mode_stack_r(native_code_buffer, dest->mem_loc_var()->stack_offset(), 0);
        break;
        
      case Variable::CONST:
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
  }
  
  template<typename T_NativeOperTraits>
  inline void
  emit_mul_operation(std::vector<uint8_t> &native_code_buffer,
#ifndef NDEBUG
                     Variable const * const hi_dest,
                     Variable const * const lo_dest,
#endif
                     Variable const * const operand_1
#ifndef NDEBUG
                     , Variable const * const operand_2
#endif
                     )
  {
    assert(Register::EDX == hi_dest->join_var()->live_interval()->preallocated_reg()->kind());
    assert(Register::EAX == lo_dest->join_var()->live_interval()->preallocated_reg()->kind());
      
    assert(Register::EAX == operand_2->join_var()->live_interval()->preallocated_reg()->kind());
      
    native_code_buffer.push_back(static_cast<uint8_t>(T_NativeOperTraits::OPCODE_1));
      
    if (operand_1->join_var()->live_interval()->preallocated_reg() != 0)
    {
      Register const * const operand_1_reg =
        operand_1->join_var()->live_interval()->preallocated_reg();
        
      addressing_mode_r_r(native_code_buffer, T_NativeOperTraits::OPCODE_2, operand_1_reg->native_represent());
    }
    else
    {
      switch (operand_1->kind())
      {
      case Variable::GLOBAL:
        addressing_mode_global_r(native_code_buffer,
                                 operand_1->mem_loc_var()->memory_addr(),
                                 T_NativeOperTraits::OPCODE_2);
        break;
          
      case Variable::TEMP:
        addressing_mode_stack_r(native_code_buffer,
                                operand_1->mem_loc_var()->stack_offset(),
                                T_NativeOperTraits::OPCODE_2);
        break;
        
      case Variable::CONST:
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
  }
}

#endif
