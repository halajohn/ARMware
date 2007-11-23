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
  template<IsLong is_long, IsSigned is_signed, IsAccumulate is_accumulate, IsUpdateStatus is_update_status>
  inline void
  InstMulti<is_long, is_signed, is_accumulate, is_update_status>::
  print_code(Inst const inst)
  {
    mInst = inst;
    
    if (NORMAL == is_long)
    {
      if (NO_ACCUMULATE == is_accumulate)
      {
        // MUL or MULS
        
        g_log_file << "MUL"
                  << CondName(mInst)
                  << ((NO_UPDATE_STATUS == is_update_status) ? "" : "S")
                  << " "
                  << CoreRegName(RdNum())
                  << ", "
                  << CoreRegName(RmNum())
                  << ", "
                  << CoreRegName(RsNum())
                  << std::endl;
      }
      else
      {
        // MLA or MLAS
        
        g_log_file << "MLA"
                  << CondName(mInst)
                  << ((NO_UPDATE_STATUS == is_update_status) ? "" : "S")
                  << " "
                  << CoreRegName(RdNum())
                  << ", "
                  << CoreRegName(RmNum())
                  << ", "
                  << CoreRegName(RsNum())
                  << ", "
                  << CoreRegName(RnNum())
                  << std::endl;
      }
    }
    else
    {
      if (UNSIGNED == is_signed)
      {
        // UMULL{S} or UMLAL{S}
        
        g_log_file << ((NO_ACCUMULATE == is_accumulate) ? "UMULL" : "UMLAL")
                   << CondName(mInst)
                   << ((NO_UPDATE_STATUS == is_update_status) ? "" : "S")
                   << " "
                   << CoreRegName(RdLoNum())
                   << ", "
                   << CoreRegName(RdHiNum())
                   << ", "
                   << CoreRegName(RmNum())
                   << ", "
                   << CoreRegName(RsNum())
                   << std::endl;
      }
      else
      {
        // SMULL{S} or SMLAL{S}
        
        g_log_file << ((NO_ACCUMULATE == is_accumulate) ? "SMULL" : "SMLAL")
                  << CondName(mInst)
                  << ((NO_UPDATE_STATUS == is_update_status) ? "" : "S")
                  << " "
                  << CoreRegName(RdLoNum())
                  << ", "
                  << CoreRegName(RdHiNum())
                  << ", "
                  << CoreRegName(RmNum())
                  << ", "
                  << CoreRegName(RsNum())
                  << std::endl;
      }
    }
  }
#endif
}
