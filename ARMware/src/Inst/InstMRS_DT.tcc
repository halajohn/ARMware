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
  template<UseWhichStatus use_which_status>
  inline void
  InstMRS<use_which_status>::
  gen_dt_code(Inst const inst, bool const is_last)
  {
    m_inst = inst;
    
    // :ARM-DDI-0100E: p.A4-60: Wei 2004-Oct-31:
    //
    // if R == 1 then
    //   Rd = SPSR
    // else
    //   Rd = CPSR
    
    // :ARM-DDI-0100E: p.A4-60: Wei 2004-Oct-31:
    //
    // instruction. If R15 is specified for <Rd>, the result is UNPREDICTABLE.
    
    switch (use_which_status)
    {
    case USE_CPSR:
      gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRMov *>(0),
                gp_chunk->find_global_var(Variable::CPSR),
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(rd_num()))));
      break;
      
    case USE_CURRENT_SPSR:
      gp_chunk->append_mir<MIR_FOR_NORMAL>(
        new MIR(static_cast<MIRMov *>(0),
                gp_chunk->find_global_var(Variable::SPSR),
                gp_chunk->find_global_var(static_cast<Variable::GlobalVarEnum>(rd_num()))));
      break;
      
    default:
      assert(!"Should not reach here.");
      break;
    }
    
    gp_chunk->incre_PC_DT();
    
    if (true == is_last)
    {
      gp_chunk->append_mir<MIR_FOR_NORMAL>(new MIR(static_cast<MIRReturn *>(0),
                                                   gp_chunk->find_const_var(ER_NORMAL)));
    }
  }
#endif
}
