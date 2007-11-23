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
  template<UseWhichStatus use_which_status>
  inline void
  InstMRS<use_which_status>::
  print_code(Inst const inst)
  {
    m_inst = inst;
    
    g_log_file << "MRS"
              << CondName(m_inst)
              << " "
              << CoreRegName(rd_num())
              << ", "
              << ((USE_CPSR == use_which_status) ? "CPSR" : "SPSR")
              << std::endl;
  }
#endif
}
