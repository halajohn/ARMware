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
#if ENABLE_DYNAMIC_TRANSLATOR
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    determine_dt_func_helper(InstLSH<ImmLSH,
                             addressing_mode,
                             direction,
                             is_signed,
                             data_width_enum,
                             is_load,
                             T_usage> * const lsh)
    {
      switch (addressing_mode)
      {
      case OFFSET:
        switch (direction)
        {
        case UP:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_imm_offset_up_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_imm_offset_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_imm_offset_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_imm_offset_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case DOWN:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_imm_offset_down_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_imm_offset_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_imm_offset_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_imm_offset_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      case POST_IDX:
        switch (direction)
        {
        case UP:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_imm_post_up_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_imm_post_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_imm_post_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_imm_post_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case DOWN:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_imm_post_down_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_imm_post_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_imm_post_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_imm_post_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
                
      case PRE_IDX:
        switch (direction)
        {
        case UP:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_imm_pre_up_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_imm_pre_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_imm_pre_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_imm_pre_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case DOWN:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_imm_pre_down_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_imm_pre_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_imm_pre_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_imm_pre_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      default: assert(!"Should not reach here."); break;
      }
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    determine_dt_func_helper(InstLSH<RegLSH,
                             addressing_mode,
                             direction,
                             is_signed,
                             data_width_enum,
                             is_load,
                             T_usage> * const lsh)
    {
      switch (addressing_mode)
      {
      case OFFSET:
        switch (direction)
        {
        case UP:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_reg_offset_up_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_reg_offset_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_reg_offset_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_reg_offset_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case DOWN:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_reg_offset_down_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_reg_offset_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_reg_offset_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_reg_offset_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      case POST_IDX:
        switch (direction)
        {
        case UP:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_reg_post_up_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_reg_post_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_reg_post_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_reg_post_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case DOWN:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_reg_post_down_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_reg_post_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_reg_post_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_reg_post_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
                
      case PRE_IDX:
        switch (direction)
        {
        case UP:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_reg_pre_up_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_reg_pre_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_reg_pre_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_reg_pre_up_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        case DOWN:
          switch (is_signed)
          {
          case UNSIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_STRH_reg_pre_down_TC);
                break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRH_reg_pre_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              // :NOTE: Wei 2004-Sep-19:
              //
              // There is no 'unsigned-byte' variants in LSH instruction.
              assert(!"Should not reach here."); break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          case SIGNED:
            switch (data_width_enum)
            {
            case HALF_WORD:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSH_reg_pre_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            case BYTE:
              switch (is_load)
              {
              case STORE:
                // :NOTE: Wei 2004-Sep-19:
                //
                // There is no 'signed-store' variants in LSH instructions.
                assert(!"Should not reach here."); break;
                
              case LOAD:
                lsh->dt_func = reinterpret_cast<DTFunc_t>(exec_LDRSB_reg_pre_down_TC);
                break;
                
              default: assert(!"Should not reach here."); break;
              }
              break;
              
            default: assert(!"Should not reach here."); break;
            }
            break;
            
          default: assert(!"Should not reach here."); break;
          }
          break;
          
        default: assert(!"Should not reach here."); break;
        }
        break;
        
      default: assert(!"Should not reach here."); break;
      }
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           IsSigned is_signed,
           DataWidthEnum data_width_enum,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSH<mode, addressing_mode, direction, is_signed, data_width_enum, is_load, T_usage>::
  gen_dt_code(Inst const inst, bool const is_last)
  {
    mInst = inst;
    
    determine_dt_func_helper(this);
    
    Label * const routine_label =
      gp_chunk->add_new_label(new Label(static_cast<LabelRoutine *>(0), dt_func));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(
      new MIR(static_cast<MIRCall_1 *>(0),
              routine_label,
              gp_chunk->find_const_var(mInst.mRawData)));
    
    Variable * const tmp1 = gp_chunk->get_new_temp();
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRRetrieve *>(0), 1, tmp1));
    
    Label * const table_base_label = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0)));
    
    MIR * const jmp_mir = gp_chunk->insert_switch_statement_DT(tmp1, table_base_label);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Create jump table.
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL:
    Label * const label_ER_NORMAL = gp_chunk->add_new_label(
      new Label(static_cast<LabelMIRInst *>(0)));
    
    MIR * const table_base_mir =
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                   label_ER_NORMAL));
    
    table_base_label->set_mir_inst(table_base_mir);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_EXCEPTION:
    Label * const label_ER_EXCEPTION =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_EXCEPTION));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL_CONT_EXEC_IN_CHUNK:
    //
    // This value is not possible for LSH, thus we set the value of this label to 0.
    Label * const label_ER_NORMAL_CONT_EXEC_IN_CHUNK =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_NORMAL_CONT_EXEC_IN_CHUNK));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_NORMAL_MODIFY_PC:
    //
    // This value is not possible for LSH, thus we set the value of this label to 0.
    Label * const label_ER_NORMAL_MODIFY_PC =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_NORMAL_MODIFY_PC));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_IO_IRQ_OCCUR:
    //
    // This value is not possible for LSH, thus we set the value of this label to 0.
    Label * const label_ER_IO_IRQ_OCCUR =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_IO_IRQ_OCCUR));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_IO_FIQ_OCCUR:
    //
    // This value is not possible for LSH, thus we set the value of this label to 0.
    Label * const label_ER_IO_FIQ_OCCUR =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0), 0));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_IO_FIQ_OCCUR));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // case ER_CHUNK_DISAPPEAR:
    Label * const label_ER_CHUNK_DISAPPEAR =
      gp_chunk->add_new_label(new Label(static_cast<LabelMIRInst *>(0)));
    
    gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRData32 *>(0),
                                 label_ER_CHUNK_DISAPPEAR));
    
    // :NOTE: Wei 2004-Nov-5:
    //
    // End of the basic block for the jump table.
    gp_chunk->next_mir_is_leader();
    
    jmp_mir->add_jump_target_label(label_ER_NORMAL);
    jmp_mir->add_jump_target_label(label_ER_EXCEPTION);
    jmp_mir->add_jump_target_label(label_ER_CHUNK_DISAPPEAR);
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // Jump target implementation.
    //
    // :NOTE: Wei 2004-Oct-03:
    //
    // The possible value returned from 'LSH' are:
    //
    // (1) ER_EXCEPTION
    // (2) ER_CHUNK_DISAPPEAR
    // (3) ER_NORMAL
    //
    // if (1) or (2), then ARMware will return to the interpreter,
    // otherwise, (3) will continue execuating in this chunk.
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_EXCEPTION
    
    label_ER_EXCEPTION->set_mir_inst(gp_chunk->append_mir<MIR_FOR_NORMAL>(
                                       new MIR(static_cast<MIRReturn *>(0),
                                               gp_chunk->find_const_var(ER_EXCEPTION))));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_CHUNK_DISAPPEAR
    
    label_ER_CHUNK_DISAPPEAR->set_mir_inst(
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                   gp_chunk->find_const_var(ER_CHUNK_DISAPPEAR))));
    
    // :NOTE: Wei 2004-Nov-4:
    //
    // ER_NORMAL.
    
    if (true == is_last)
    {
      label_ER_NORMAL->set_mir_inst(
        gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                     gp_chunk->find_const_var(ER_NORMAL))));
    }
    else
    {
      label_ER_NORMAL->reset_to_arm_first_inst(gp_chunk->next_arm_inst_info());
      
      gp_chunk->add_pending_back_patch_label(label_ER_NORMAL);
    }
  }
#endif
}
