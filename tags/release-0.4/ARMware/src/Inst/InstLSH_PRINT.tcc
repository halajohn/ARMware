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
  namespace
  {
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    print_code_helper(InstLSH<ImmLSH,
                      addressing_mode,
                      direction,
                      is_signed,
                      data_width_enum,
                      is_load,
                      T_usage> const * const lsh)
    {
      switch (addressing_mode)
      {
      case OFFSET:
        g_log_file << "["
                   << CoreRegName(lsh->RnNum())
                   << ", #"
                   << ((UP == direction) ? "+" : "-")
                   << static_cast<int32_t>(lsh->Offset())
                   << "]"
                   << std::endl;
        break;
        
      case PRE_IDX:
        g_log_file << "["
                   << CoreRegName(lsh->RnNum())
                   << ", #"
                   << ((UP == direction) ? "+" : "-")
                   << static_cast<int32_t>(lsh->Offset())
                   << "]!"
                   << std::endl;
        break;
        
      case POST_IDX:
        g_log_file << "["
                   << CoreRegName(lsh->RnNum())
                   << "], #"
                   << ((UP == direction) ? "+" : "-")
                   << static_cast<int32_t>(lsh->Offset())
                   << std::endl;
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
    }
    
    template<AddressingMode addressing_mode,
             Direction direction,
             IsSigned is_signed,
             DataWidthEnum data_width_enum,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    print_code_helper(InstLSH<RegLSH,
                      addressing_mode,
                      direction,
                      is_signed,
                      data_width_enum,
                      is_load,
                      T_usage> const * const lsh)
    {
      switch (addressing_mode)
      {
      case OFFSET:
        g_log_file << "["
                   << CoreRegName(lsh->RnNum())
                   << ", "
                   << ((UP == direction) ? "+" : "-")
                   << CoreRegName(lsh->RmNum())
                   << "]"
                   << std::endl;
        break;
        
      case PRE_IDX:
        g_log_file << "["
                   << CoreRegName(lsh->RnNum())
                   << ", "
                   << ((UP == direction) ? "+" : "-")
                   << CoreRegName(lsh->RmNum())
                   << "]!"
                   << std::endl;
        break;
        
      case POST_IDX:
        g_log_file << "["
                   << CoreRegName(lsh->RnNum())
                   << "], "
                   << ((UP == direction) ? "+" : "-")
                   << CoreRegName(lsh->RmNum())
                   << std::endl;
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
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
  print_code(Inst const inst)
  {
    mInst = inst;
    
    if (LOAD == is_load)
    {
      g_log_file << "LDR"
                 << CondName(mInst);
      
      if (HALF_WORD == data_width_enum)
      {
        if (SIGNED == is_signed)
        {
          // LDRSH
          
          g_log_file << "SH ";
        }
        else
        {
          // LDRH
          
          g_log_file << "H ";
        }
      }
      else
      {
        // LDRSB
        
        assert(BYTE == data_width_enum);
        assert(SIGNED == is_signed);
        
        g_log_file << "SB ";
      }
    }
    else
    {
      // STRH
      
      assert(HALF_WORD == data_width_enum);
      assert(UNSIGNED == is_signed);
      
      g_log_file << "STR"
                 << CondName(mInst)
                 << "H ";
    }
    
    g_log_file << " "
               << CoreRegName(RdNum())
               << ", ";
    
    print_code_helper(this);
  }
#endif
}
