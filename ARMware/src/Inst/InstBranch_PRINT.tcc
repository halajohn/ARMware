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
  /////////////////////////////////////////////////////////////////////////////
  // print_code
  
  namespace
  {
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    print_code_helper(InstBranch<RegBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      // BX or BLX_2
      
      assert(EXCHANGE == exchange_to_thumb);
      
      switch (is_link)
      {
      case LINK:
        g_log_file << "BLX(2) ";
        break;
        
      case NO_LINK:
        g_log_file << "BX ";
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      g_log_file << CondName(branch->mInst)
                << " "
                << CoreRegName(branch->RmNum()) << std::endl;
    }
    
    template<IsLink is_link, ExchangeToThumb exchange_to_thumb, UsageEnum T_usage>
    inline void
    print_code_helper(InstBranch<ImmBranch, is_link, exchange_to_thumb, T_usage> const * const branch)
    {
      // B, BL, BLX_1
      
      switch (is_link)
      {
      case LINK:
        switch (exchange_to_thumb)
        {
        case EXCHANGE:
          g_log_file << "BLX(1)";
          break;
          
        case NO_EXCHANGE:
          g_log_file << "BL";
          break;
          
        default:
          assert(!"Should not reach here.");
          break;
        }
        break;
        
      case NO_LINK:
        g_log_file << "B";
        break;
        
      default:
        assert(!"Should not reach here.");
        break;
      }
      
      g_log_file << CondName(branch->mInst)
                << " "
                << std::dec << static_cast<int32_t>(branch->Imm())
                << std::endl;
    }
  }
  
  template<typename mode,
           IsLink is_link,
           ExchangeToThumb exchange_to_thumb,
           UsageEnum T_usage>
  inline void
  InstBranch<mode, is_link, exchange_to_thumb, T_usage>::
  print_code(Inst const inst)
  {
    mInst = inst;
    
    print_code_helper(this);
  }
#endif
}
