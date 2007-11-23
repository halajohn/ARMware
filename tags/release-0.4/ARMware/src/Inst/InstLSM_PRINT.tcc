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
    template<Direction direction,
             StartFrom start_from,
             DesiredBank desired_bank,
             IsWriteBack is_write_back,
             IsLoad is_load,
             UsageEnum T_usage>
    inline void
    print_code_helper(InstLSM<direction,
                      start_from,
                      desired_bank,
                      is_write_back,
                      is_load,
                      T_usage> const * const /* lsm */)
    {
      if (DOWN == direction)
      {
        if (AFTER == start_from)
        {
          g_log_file << "DA ";
        }
        else
        {
          g_log_file << "DB ";
        }
      }
      else
      {
        if (AFTER == start_from)
        {
          g_log_file << "IA ";
        }
        else
        {
          g_log_file << "IB ";
        }
      }
    }
  }
  
  template<Direction direction,
           StartFrom start_from,
           DesiredBank desired_bank,
           IsWriteBack is_write_back,
           IsLoad is_load,
           UsageEnum T_usage>
  inline void
  InstLSM<direction, start_from, desired_bank, is_write_back, is_load, T_usage>::
  print_code(Inst const inst)
  {
    mInst = inst;
    
    if (LOAD == is_load)
    {
      g_log_file << "LDM"
                << CondName(mInst);
      
      print_code_helper(this);
      
      if (CURRENT_BANK == desired_bank)
      {
        // LDM_1
        g_log_file << CoreRegName(RnNum())
                  << ((WRITE_BACK == is_write_back) ? "!" : "")
                  << ", {";
        
        for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
        {
          if (true == RegEnable(i))
          {
            g_log_file << CoreRegName(i)
                      << ", ";
          }
        }
        
        g_log_file << "}"
                  << std::endl;
      }
      else
      {
        if (false == RegEnable(CR_PC))
        {
          // LDM_2
          g_log_file << CoreRegName(RnNum())
                    << ", {";
          
          for (CoreRegNum i = CR_BEGIN; i != CR_PC; ++i)
          {
            if (true == RegEnable(i))
            {
              g_log_file << CoreRegName(i)
                        << ", ";
            }
          }
          
          g_log_file << "}^"
                    << std::endl;
        }
        else
        {
          // LDM_3
          g_log_file << CoreRegName(RnNum())
                    << ((WRITE_BACK == is_write_back) ? "!" : "")
                    << ", {";
          
          for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
          {
            if (true == RegEnable(i))
            {
              g_log_file << CoreRegName(i)
                        << ", ";
            }
          }
          
          g_log_file << "}^"
                    << std::endl;
        }
      }
    }
    else
    {
      g_log_file << "STM";
      
      print_code_helper(this);
      
      if (CURRENT_BANK == desired_bank)
      {
        // STM_1
        g_log_file << CoreRegName(RnNum())
                  << ((WRITE_BACK == is_write_back) ? "!" : "")
                  << ", {";
        
        for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
        {
          if (true == RegEnable(i))
          {
            g_log_file << CoreRegName(i)
                      << ", ";
          }
        }
        
        g_log_file << "}"
                  << std::endl;
      }
      else
      {
        // STM_2
        g_log_file << CoreRegName(RnNum())
                  << ", {";
        
        for (CoreRegNum i = CR_BEGIN; i != CR_END; ++i)
        {
          if (true == RegEnable(i))
          {
            g_log_file << CoreRegName(i)
                      << ", ";
          }
        }
        
        g_log_file << "}^"
                  << std::endl;
      }
    }
  }
#endif
}
