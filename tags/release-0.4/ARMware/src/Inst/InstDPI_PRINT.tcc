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
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    print_code_helper(InstDPI<opcode,
                      RegImmSftDPI,
                      is_update_status,
                      T_usage,
                      update_status_category_id,
                      operand_number_category_id> const * const dpi)
    {
      g_log_file << CoreRegName(dpi->RmNum())
                << ", "
                << SftTypeName(dpi->WhichSftType())
                << " "
                << dpi->SftImm()
                << std::endl;
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    print_code_helper(InstDPI<opcode,
                      RegRegSftDPI,
                      is_update_status,
                      T_usage,
                      update_status_category_id,
                      operand_number_category_id> const * const dpi)
    {
      g_log_file << CoreRegName(dpi->RmNum())
                << ", "
                << SftTypeName(dpi->WhichSftType())
                << " "
                << CoreRegName(dpi->RsNum())
                << std::endl;
    }
    
    template<typename opcode,
             IsUpdateStatus is_update_status,
             UsageEnum T_usage,
             typename update_status_category_id,
             typename operand_number_category_id>
    inline void
    print_code_helper(InstDPI<opcode,
                      ImmDPI,
                      is_update_status,
                      T_usage,
                      update_status_category_id,
                      operand_number_category_id> const * const dpi)
    {
      g_log_file << dpi->Imm()
                << " ROR "
                << dpi->RotImm()
                << " "
                << std::endl;;
    }
  }
  
  template<typename opcode,
           typename mode,
           IsUpdateStatus is_update_status,
           UsageEnum T_usage,
           typename update_status_category_id,
           typename operand_number_category_id>
  inline void
  InstDPI<opcode,
          mode,
          is_update_status,
          T_usage,
          update_status_category_id,
          operand_number_category_id>::
  print_code(Inst const inst)
  {
    m_inst = inst;
    
    g_log_file << opcode::NAME
               << CondName(m_inst)
               << ((UPDATE_STATUS == is_update_status) ? "S" : "")
               << " "
               << CoreRegName(RdNum())
               << ", "
               << CoreRegName(RnNum())
               << ", ";
    
    print_code_helper(this);
  }
#endif
}
