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
#if PRINT_CHUNK_CODE
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  print_sft_field() const
  {
    switch (which_sft_type())
    {
    case LSL:
      g_log_file << ", LSL #"
                << sft_imm();
      break;
      
    case LSR:
      g_log_file << ", LSR #"
                << sft_imm();
      break;
      
    case ASR:
      g_log_file << ", ASR #"
                << sft_imm();
      break;
      
    case ROR:
      if (0 == sft_imm())
      {
        g_log_file << ", RRX";
      }
      else
      {
        g_log_file << ", ROR #"
                  << sft_imm();
      }
      break;
      
    case RRX:
    default:
      assert(!"Should not reach here.");
      break;
    }
  }
  
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    print_code_helper(InstLSW<ImmLSW,
                      addressing_mode,
                      direction,
                      data_width_enum,
                      desired_mode,
                      is_load,
                      T_usage> const * const lsw)
    {
      switch (addressing_mode)
      {
      case OFFSET:
        g_log_file << "["
                  << CoreRegName(lsw->rn_num())
                  << ", #"
                  << ((UP == direction) ? "+" : "-")
                  << static_cast<int32_t>(lsw->offset_without_check())
                  << "]"
                  << std::endl;
        break;
        
      case PRE_IDX:
        g_log_file << "["
                  << CoreRegName(lsw->rn_num())
                  << ", #"
                  << ((UP == direction) ? "+" : "-")
                  << static_cast<int32_t>(lsw->offset_without_check())
                  << "]!"
                  << std::endl;
        break;
        
      case POST_IDX:
        g_log_file << "["
                  << CoreRegName(lsw->rn_num())
                  << "], #"
                  << ((UP == direction) ? "+" : "-")
                  << static_cast<int32_t>(lsw->offset_without_check())
                  << std::endl;
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             DataWidthEnum data_width_enum,
             DesiredMode desired_mode,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    print_code_helper(InstLSW<RegLSW,
                      addressing_mode,
                      direction,
                      data_width_enum,
                      desired_mode,
                      is_load,
                      T_usage> const * const lsw)
    {
      switch (addressing_mode)
      {
      case OFFSET:
        g_log_file << "["
                  << CoreRegName(lsw->rn_num())
                  << ", "
                  << ((UP == direction) ? "+" : "-")
                  << CoreRegName(lsw->rm_num());
        
        lsw->print_sft_field();
        
        g_log_file << "]";
        break;
        
      case PRE_IDX:
        g_log_file << "["
                  << CoreRegName(lsw->rn_num())
                  << ", "
                  << ((UP == direction) ? "+" : "-")
                  << CoreRegName(lsw->rm_num());
        
        lsw->print_sft_field();
        
        g_log_file << "]!";
        break;
        
      case POST_IDX:
        g_log_file << "["
                  << CoreRegName(lsw->rn_num())
                  << "], "
                  << ((UP == direction) ? "+" : "-")
                  << CoreRegName(lsw->rm_num())
                  << ", ";
        
        lsw->print_sft_field();
        
        g_log_file << std::endl;
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      g_log_file << std::endl;
    }
  }
  
  template<typename mode,
           AddressingMode addressing_mode,
           Direction direction,
           DataWidthEnum data_width_enum,
           DesiredMode desired_mode,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSW<mode, addressing_mode, direction, data_width_enum, desired_mode, is_load, T_usage>::
  print_code(Inst const inst)
  {
    m_inst = inst;
    
    if (LOAD == is_load)
    {
      g_log_file << "LDR"
                << CondName(m_inst);
      
      if (BYTE == data_width_enum)
      {
        if (USER_MODE)
        {
          // LDRBT
          
          g_log_file << "BT";
        }
        else
        {
          // LDRB
          
          g_log_file << "B";
        }
      }
      else
      {
        assert(WORD == data_width_enum);
        
        if (USER_MODE)
        {
          // LDRT
          
          g_log_file << "T";
        }
        else
        {
          // LDR
        }
      }
    }
    else
    {
      g_log_file << "STR"
                << CondName(m_inst);
      
      if (BYTE == data_width_enum)
      {
        if (USER_MODE)
        {
          // STRBT
          
          g_log_file << "BT";
        }
        else
        {
          // STRB
          
          g_log_file << "B";
        }
      }
      else
      {
        assert(WORD == data_width_enum);
        
        if (USER_MODE)
        {
          // STRT
          
          g_log_file << "T";
        }
        else
        {
          // STR
        }
      }
    }
    
    g_log_file << " "
               << CoreRegName(rd_num())
               << ", ";
    
    print_code_helper(this);
  }
#endif
}
