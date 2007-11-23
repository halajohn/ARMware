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
#if ENABLE_THREADED_CODE
  template<IsLoad T_is_load>
  inline ExecResultEnum
  exec_CRT_TC(Inst const inst)
  {
#if CHECK_CHUNK_CODE
    assert(inst.mRawData == gp_core->get_curr_inst());
#endif
    
    return gp_core->template exec_CRT<T_is_load>(inst);
  }
#endif
}
